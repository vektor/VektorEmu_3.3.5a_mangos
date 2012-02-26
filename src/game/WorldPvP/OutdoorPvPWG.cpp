#include "OutdoorPvPWG.h"
#include "../SpellAuras.h"
#include "../Vehicle.h"
#include "../ObjectMgr.h"
#include "../World.h"
#include "../Chat.h"
#include "../MapManager.h"
#include "../Language.h"
#include "../World.h"

void _LoadTeamPair(TeamPairMap &pairMap, const TeamPair *pair)
{
    while((*pair)[0])
    {
        pairMap[(*pair)[0]] = (*pair)[1];
        pairMap[(*pair)[1]] = (*pair)[0];
        ++pair;
    }
}

void _RespawnCreatureIfNeeded(Creature *cr, uint32 entry)
{
    if (cr)
    {
		cr->SetEntry(entry);
        if (entry != cr->GetEntry() || !cr->isAlive())
            cr->Respawn();
        cr->SetVisibility(VISIBILITY_ON);
    }
}

OutdoorPvPWG::OutdoorPvPWG()
{
    m_TypeId = OUTDOOR_PVP_WG;
	m_WGGroup[BG_TEAM_ALLIANCE] = NULL;
	m_WGGroup[BG_TEAM_HORDE] = NULL;

	FortressTurrets_Spawns.clear();
	Horde_Spawns.clear();
	Alliance_Spawns.clear();
	FortressLargeBanners.clear();
	FortressPicBanners.clear();
	BuildingDestroyables.clear();
	NESpawnsCr.clear();
	SESpawnsCr.clear();
	NWSpawnsCr.clear();
	SWSpawnsCr.clear();
	NESpawnsGo.clear();
	SESpawnsGo.clear();
	NWSpawnsGo.clear();
	SWSpawnsGo.clear();
	ToDespawnInWarCr.clear();
	FortressGuards.clear();
	FortressEngineer.clear();
	fortress_spirit[BG_TEAM_ALLIANCE] = 0;
	fortress_spirit[BG_TEAM_HORDE] = 0;

    m_LastResurrectTime = 0; // Temporal copy of BG system till 3.2
}

bool OutdoorPvPWG::SetupOutdoorPvP()
{
	if(QueryResult *result = CharacterDatabase.Query("SELECT value FROM needed_variables WHERE needed_variables.key = '1';"))
	{
		Field *fields = result->Fetch();
        m_defender = BattleGroundTeamId(fields[0].GetUInt32());
	}
	else
	{
		sLog.outError("Could not find last owner of wintergrasp, set to random");
		m_defender = BattleGroundTeamId(urand(0,1));
	}
    
    sWorld.setState(0, getDefenderTeam());
    m_changeDefender = false;
    m_workshopCount[BG_TEAM_ALLIANCE] = 0;
    m_workshopCount[BG_TEAM_HORDE] = 0;
    m_tenacityStack = 0;
    m_gate = NULL;

    std::list<uint32> engGuids;
    std::list<uint32> spiritGuids;

    // Store Eng, spirit guide guids and questgiver for later use
    QueryResult *result = WorldDatabase.PQuery("SELECT guid, id FROM creature WHERE creature.map=571"
        " AND creature.id IN (%u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u);",
        CRE_ENG_A, CRE_ENG_H, CRE_SPI_A, CRE_SPI_H, 31101, 31051, 31102, 31052,
        31107, 31109, 31151, 31153, 31106, 31108, 31053, 31054, 31091, 31036, 32615, 32626);
    if (!result)
        sLog.outError("Cannot find siege workshop master or spirit guides in creature!");
    else
    {
        do
        {
            Field *fields = result->Fetch();
            switch(fields[1].GetUInt32())
            {
                case CRE_ENG_A:
                case CRE_ENG_H:
                    engGuids.push_back(fields[0].GetUInt32());
                    break;
                case CRE_SPI_A:
                case CRE_SPI_H:
                    spiritGuids.push_back(fields[0].GetUInt32());
                    break;
                default:
                    break;
            }
        }
		while(result->NextRow());
        delete result;
    }

    // Select POI
    AreaPOIList areaPOIs;
    float minX = 9999, minY = 9999, maxX = -9999, maxY = -9999;
    for (uint32 i = 0; i < sAreaPOIStore.GetNumRows(); ++i)
    {
        const AreaPOIEntry * poiInfo = sAreaPOIStore.LookupEntry(i);
        if (poiInfo && poiInfo->zoneId == ZONE_WINTERGRASP)
        {
            areaPOIs.push_back(poiInfo);
            if (minX > poiInfo->x) minX = poiInfo->x;
            if (minY > poiInfo->y) minY = poiInfo->y;
            if (maxX < poiInfo->x) maxX = poiInfo->x;
            if (maxY < poiInfo->y) maxY = poiInfo->y;
        }
    }
    minX -= 20; minY -= 20; maxX += 20; maxY += 20;

    // Coords: 4290.330078, 1790.359985 - 5558.379883, 4048.889893
    result = WorldDatabase.PQuery("SELECT guid FROM gameobject,gameobject_template"
        " WHERE gameobject.map=571"
        " AND gameobject.position_x>%f AND gameobject.position_y>%f"
        " AND gameobject.position_x<%f AND gameobject.position_y<%f"
        " AND gameobject_template.type=33"
        " AND gameobject.id=gameobject_template.entry",
        minX, minY, maxX, maxY);
    if (!result)
        return false;

	do
    {
        Field *fields = result->Fetch();

        uint32 guid = fields[0].GetUInt32();
        GameObjectData const * goData = sObjectMgr.GetGOData(guid);
        if (!goData) // this should not happen
            continue;

        float x = goData->posX, y = goData->posY;
        float minDist = 100;
        AreaPOIList::iterator poi = areaPOIs.end();
        for (AreaPOIList::iterator itr = areaPOIs.begin(); itr != areaPOIs.end(); ++itr)
        {
            if (!(*itr)->icon[1]) // note: may for other use
                continue;

            float dist = (abs((*itr)->x - x) + abs((*itr)->y - y));
            if (minDist > dist)
            {
                minDist = dist;
                poi = itr;
            }
        }

        if (poi == areaPOIs.end())
            continue;

        // add building to the list
        BattleGroundTeamId teamId = x > POS_X_CENTER ? getDefenderTeam() : getAttackerTeam();
        m_buildingStates[guid] = new BuildingState((*poi)->worldState, teamId, getDefenderTeam() != BG_TEAM_ALLIANCE);
        if ((*poi)->id == 2246)
            m_gate = m_buildingStates[guid];
        areaPOIs.erase(poi);

        // add capture point
        uint32 capturePointEntry = 0;

        switch(goData->id)
        {
            case 192028: // NW
            case 192030: // W
            case 192032: // SW
                capturePointEntry = 190475;
                break;
            case 192029: // NE
            case 192031: // E
            case 192033: // SE
                capturePointEntry = 190487;
                break;
        }

        if (capturePointEntry)
        {
            uint32 engGuid = 0;
            uint32 spiritGuid = 0;
            // Find closest Eng to Workshop
            float minDist = 100;
            for (std::list<uint32>::iterator itr = engGuids.begin(); itr != engGuids.end(); ++itr)
            {
                const CreatureData *creData = sObjectMgr.GetCreatureData(*itr);
                if (!creData)
                    continue;

                float dist = (abs(creData->posX - x) + abs(creData->posY - y));
                if (minDist > dist)
                {
                    minDist = dist;
                    engGuid = *itr;
                }
            }

            if (!engGuid)
            {
                sLog.outError("Cannot find nearby siege workshop master!");
                continue;
            }
            else
                engGuids.remove(engGuid);
            // Find closest Spirit Guide to Workshop
            minDist = 255;
            for (std::list<uint32>::iterator itr = spiritGuids.begin(); itr != spiritGuids.end(); ++itr)
            {
                const CreatureData *creData = sObjectMgr.GetCreatureData(*itr);
                if (!creData)
                    continue;

                float dist = (abs(creData->posX - x) + abs(creData->posY - y));
                if (minDist > dist)
                {
                    minDist = dist;
                    spiritGuid = *itr;
                }
            }

            // Inside fortress won't be capturable
            OPvPCapturePointWG *workshop = new OPvPCapturePointWG(this, m_buildingStates[guid]);
            if (goData->posX < POS_X_CENTER && !workshop->SetCapturePointData(capturePointEntry, goData->mapid, goData->posX + 40 * cos(goData->orientation + M_PI / 2), goData->posY + 40 * sin(goData->orientation + M_PI / 2), goData->posZ))
            {
                delete workshop;
                sLog.outError("Cannot add capture point!");
                continue;
            }

            const CreatureData *creData = sObjectMgr.GetCreatureData(engGuid);
            if (!creData)
                continue;

			const_cast<CreatureData*>(creData)->modelid_override = 0;
            workshop->m_engGuid = engGuid;

            // Back spirit is linked to one of the inside workshops, 1 workshop wont have spirit
            if (spiritGuid)
            {
                spiritGuids.remove(spiritGuid);

                const CreatureData *spiritData = sObjectMgr.GetCreatureData(spiritGuid);
                if (!spiritData)
                    continue;

				const_cast<CreatureData*>(spiritData)->modelid_override = 0;
                workshop->m_spiGuid = spiritGuid;
            }
            else
                workshop->m_spiGuid = 0;
            workshop->m_workshopGuid = guid;
            AddCapturePoint(workshop);
            m_buildingStates[guid]->type = BUILDING_WORKSHOP;
            workshop->SetTeamByBuildingState();
        }
    }
	while(result->NextRow());
    delete result;

    engGuids.clear();
    spiritGuids.clear();

    /*if (!m_gate)
    {
        sLog.outError("Cannot find wintergrasp fortress gate!");
        return false;
    }*/

    // Load Graveyard
    GraveYardMap::const_iterator graveLow  = sObjectMgr.mGraveYardMap.lower_bound(ZONE_WINTERGRASP);
    GraveYardMap::const_iterator graveUp   = sObjectMgr.mGraveYardMap.upper_bound(ZONE_WINTERGRASP);
    for (AreaPOIList::iterator itr = areaPOIs.begin(); itr != areaPOIs.end();)
    {
        if ((*itr)->icon[1] == 8)
        {
            // find or create grave yard
            const WorldSafeLocsEntry *loc = sObjectMgr.GetClosestGraveYard((*itr)->x, (*itr)->y, (*itr)->z, (*itr)->mapId, 0);
            if (!loc)
            {
                ++itr;
                continue;
            }

            GraveYardMap::const_iterator graveItr;
            for (graveItr = graveLow; graveItr != graveUp; ++graveItr)
                if (graveItr->second.safeLocId == loc->ID)
                    break;
            if (graveItr == graveUp)
            {
                GraveYardData graveData;
                graveData.safeLocId = loc->ID;
                graveData.team = 0;
                graveItr = sObjectMgr.mGraveYardMap.insert(std::make_pair(ZONE_WINTERGRASP, graveData));
            }

            for (BuildingStateMap::iterator stateItr = m_buildingStates.begin(); stateItr != m_buildingStates.end(); ++stateItr)
            {
                if (stateItr->second->worldState == (*itr)->worldState)
                {
                    stateItr->second->graveTeam = const_cast<uint32*>(&graveItr->second.team);
                    break;
                }
            }
            areaPOIs.erase(itr++);
        }
        else
            ++itr;
    }

    //Titan Relic
    sObjectMgr.AddGOData(192829, 571, 5440, 2840.8, 420.43 + 10, 0);

    _LoadTeamPair(m_goDisplayPair, OutdoorPvPWGGODisplayPair);
    _LoadTeamPair(m_creEntryPair, OutdoorPvPWGCreEntryPair);

    m_wartime = false;

	uint32 savedTimer;
	
	if(QueryResult *result = CharacterDatabase.PQuery("SELECT value FROM needed_variables WHERE needed_variables.key = '0'"))
	{
		Field *fields = result->Fetch();
        savedTimer = fields[0].GetUInt32();
	}
	else
	{
		CharacterDatabase.Execute("INSERT INTO needed_variables VALUES ('0','30');");
		sLog.outError("Could not find wintergrasp timer in database. Set to 30");
		savedTimer = 30;
	}
	
	DBUpdate_Timer = 60020;
    m_timer = savedTimer * MINUTE * IN_MILLISECONDS;

    m_towerDamagedCount[BG_TEAM_ALLIANCE] = 0;
    m_towerDestroyedCount[BG_TEAM_ALLIANCE] = 0;
    m_towerDamagedCount[BG_TEAM_HORDE] = 0;
    m_towerDestroyedCount[BG_TEAM_HORDE] = 0;

    RemoveOfflinePlayerWGAuras();

    RegisterZone(ZONE_WINTERGRASP);
	ChangeFortressSpawns(m_defender);
    return true;
}

void OutdoorPvPWG::ChangeFortressSpawns(BattleGroundTeamId owner)
{
	if(!Horde_Spawns.empty())
	{
		for (std::vector<uint64>::iterator itr = Horde_Spawns.begin(); itr != Horde_Spawns.end(); ++itr)
		{
			if(owner == BG_TEAM_HORDE)
			{
				if(GetMap())
				{
					if(Creature* cr = GetMap()->GetCreatureOrPetOrVehicle(*itr))
					{
						cr->SetRespawnDelay(300);
						cr->Respawn();
						cr->SetPhaseMask(1,true);
					}
				}
			}
			else
			{
				if(GetMap())
				{
					if(Creature* cr = GetMap()->GetCreatureOrPetOrVehicle(*itr))
					{
						cr->SetRespawnDelay(1000*RESPAWN_ONE_DAY);
						cr->ForcedDespawn(1000);
						cr->SetPhaseMask(2,true);
					}
				}
			}
		}
	}

	if(!Alliance_Spawns.empty())
	{
		for (std::vector<uint64>::iterator itr = Alliance_Spawns.begin(); itr != Alliance_Spawns.end(); ++itr)
		{
			if(owner == BG_TEAM_ALLIANCE)
			{
				if(GetMap())
				{
					if(Creature* cr = GetMap()->GetCreatureOrPetOrVehicle(*itr))
					{
						cr->SetRespawnDelay(300);
						cr->Respawn();
						cr->SetPhaseMask(1,true);
					}
				}
			}
			else
			{
				if(GetMap())
				{
					if(Creature* cr = GetMap()->GetCreatureOrPetOrVehicle(*itr))
					{
						cr->SetRespawnDelay(1000*RESPAWN_ONE_DAY);
						cr->ForcedDespawn(1000);
						cr->SetPhaseMask(2,true);
					}
				}
			}
		}
	}

	if(!FortressPicBanners.empty())
	{
		for (std::vector<uint64>::iterator itr = FortressPicBanners.begin(); itr != FortressPicBanners.end(); ++itr)
		{
			if(owner == BG_TEAM_ALLIANCE)
			{
				if(GameObject* go = GetMap()->GetGameObject(*itr))
					go->SetUInt32Value(GAMEOBJECT_DISPLAYID,5651);
			}
			else
			{
				if(GameObject* go = GetMap()->GetGameObject(*itr))
					go->SetUInt32Value(GAMEOBJECT_DISPLAYID,5652);
			}
		}
	}

	if(!FortressLargeBanners.empty())
	{
		for (std::vector<uint64>::iterator itr = FortressLargeBanners.begin(); itr != FortressLargeBanners.end(); ++itr)
		{
			if(owner == BG_TEAM_ALLIANCE)
			{
				if(GameObject* go = GetMap()->GetGameObject(*itr))
					go->SetUInt32Value(GAMEOBJECT_DISPLAYID,8256);
			}
			else
			{
				if(GameObject* go = GetMap()->GetGameObject(*itr))
					go->SetUInt32Value(GAMEOBJECT_DISPLAYID,8257);
			}
		}
	}

	if(!FortressTurrets_Spawns.empty())
	{
		for (std::vector<uint64>::iterator itr = FortressTurrets_Spawns.begin(); itr != FortressTurrets_Spawns.end(); ++itr)
		{
			if(owner == BG_TEAM_ALLIANCE)
			{
				if(Creature* cr = GetMap()->GetCreatureOrPetOrVehicle(*itr))
					if(cr->IsVehicle())
					{
						cr->GetVehicleKit()->RemoveAllPassengers();
						cr->setFaction(1802);
						cr->SetVisibility(VISIBILITY_ON);
					}
			}
			else
			{
				if(Creature* cr = GetMap()->GetCreatureOrPetOrVehicle(*itr))
					if(cr->IsVehicle())
					{
						cr->GetVehicleKit()->RemoveAllPassengers();
						cr->setFaction(1801);
						cr->SetVisibility(VISIBILITY_ON);
					}
			}
		}
	}

	if(!FortressGuards.empty())
	{
		for (std::vector<uint64>::iterator itr = FortressGuards.begin(); itr != FortressGuards.end(); ++itr)
		{
			if(owner == BG_TEAM_HORDE)
			{
				if(GetMap())
				{
					if(Creature* cr = GetMap()->GetCreatureOrPetOrVehicle(*itr))
					{
						if(cr->GetPositionX() > 5073.0f && cr->GetPositionX() < 5392.0f && cr->GetPositionY() > 2550.0f && cr->GetPositionY() < 3100.0f)
						{
							cr->UpdateEntry(30739,HORDE);
							cr->setFaction(WintergraspFaction[owner]);
						}
					}
				}
			}
			else
			{
				if(GetMap())
				{
					if(Creature* cr = GetMap()->GetCreatureOrPetOrVehicle(*itr))
					{
						if(cr->GetPositionX() > 5073.0f && cr->GetPositionX() < 5392.0f && cr->GetPositionY() > 2550.0f && cr->GetPositionY() < 3100.0f)
						{
							cr->UpdateEntry(30740,ALLIANCE);
							cr->setFaction(WintergraspFaction[owner]);
						}
					}
				}
			}
		}
	}
	if(!FortressEngineer.empty())
	{
		for (std::vector<uint64>::iterator itr = FortressEngineer.begin(); itr != FortressEngineer.end(); ++itr)
		{
			if(owner == BG_TEAM_HORDE)
			{
				if(GetMap())
					if(Creature* cr = GetMap()->GetCreatureOrPetOrVehicle(*itr))
						if(cr->GetPositionX() > 5073.0f && cr->GetPositionX() < 5392.0f && cr->GetPositionY() > 2550.0f && cr->GetPositionY() < 3100.0f)
						{
							cr->UpdateEntry(CRE_ENG_H,HORDE);
							cr->setFaction(WintergraspFaction[owner]);
						}
			}
			else
			{
				if(GetMap())
					if(Creature* cr = GetMap()->GetCreatureOrPetOrVehicle(*itr))
						if(cr->GetPositionX() > 5073.0f && cr->GetPositionX() < 5392.0f && cr->GetPositionY() > 2550.0f && cr->GetPositionY() < 3100.0f)
						{
							cr->UpdateEntry(CRE_ENG_A,ALLIANCE);
							cr->setFaction(WintergraspFaction[owner]);
						}
			}
		}
	}
}

void OutdoorPvPWG::ProcessEvent(GameObject *obj, uint32 eventId, Player* user)
{
    if (obj->GetEntry() == 192829) // Titan Relic
    {
        if (/*obj->GetGOInfo()->goober.eventId == eventId && */isWarTime() && m_gate && m_gate->damageState == DAMAGE_DESTROYED)
        {
			if(user)
				sWorld.SendZoneText(ZONE_WINTERGRASP, fmtstring("%s a pris l'Orbe !", user->GetName()));
			m_defender = getAttackerTeam();
			ChangeFortressSpawns(m_defender);
			CharacterDatabase.PQuery("UPDATE needed_variables set needed_variables.value = %u where needed_variables.key = '1';",m_defender);
            m_changeDefender = true;
			EndBattle();
        }
    }
    else if (obj->GetGoType() == GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING)
    {
        BuildingStateMap::const_iterator itr = m_buildingStates.find(obj->GetDBTableGUIDLow());
        if (itr == m_buildingStates.end())
            return;

        std::string msgStr = "";
        switch(eventId)
        { // TODO - Localized msgs of GO names
            case 19672: case 19675: // Flamewatch Tower
                msgStr = "de Guetteflame";
                break;
            case 19674: case 19677: // Shadowsight Tower
                msgStr = "d'Ombrevue";
                break;
            case 19673: case 19676: // Winter's Edge Tower
                msgStr = "Bornehiver";
                break;
            case 19776: case 19778: // E Workshop damaged
                msgStr = "Sunken Ring";
                break;
            case 19777: case 19779: // W Workshop damaged
                msgStr = "Temple Brise";
                break;
            case 19782: case 19786: // NW Workshop damaged
                msgStr = "Nord-Ouest";
                break;
            case 19783: case 19787: // NE Workshop damaged
                msgStr = "Nord-Est";
                break;
            case 19784: case 19788: // SW Workshop damaged
                msgStr = "Sud-Ouest";
                break;
            case 19785: case 19789: // SE Workshop damaged
                msgStr = "Sud-Est";
                break;
            case 19657: case 19661: // NW Wintergrasp Keep Tower damaged
                msgStr = "north-western";
                break;
            case 19658: case 19663: // NE Wintergrasp Keep Tower damaged
                msgStr = "north-eastern";
                break;
            case 19659: case 19662: // SW Wintergrasp Keep Tower damaged
                msgStr = "south-western";
                break;
            case 19660: case 19664: // SE Wintergrasp Keep Tower damaged
                msgStr = "south-eastern";
                break;
            default:
                msgStr = "";
        }

        BuildingState *state = itr->second;
        if (eventId == obj->GetGOInfo()->building.damagedEvent)
        {
            state->damageState = DAMAGE_DAMAGED;
			switch(obj->GetEntry())
            {
				case 192030:
				case 192031:
				case 192032:
				case 192033:
                    msgStr = fmtstring(sObjectMgr.GetMangosStringForDBCLocale(LANG_BG_WG_WORKSHOP_DAMAGED), msgStr.c_str(), sObjectMgr.GetMangosStringForDBCLocale(getDefenderTeam() == BG_TEAM_ALLIANCE ? LANG_BG_ALLY : LANG_BG_HORDE));
                    sWorld.SendZoneText(ZONE_WINTERGRASP, msgStr.c_str());
                    break;
				case 190356:
				case 190357:
				case 190358:
                    ++m_towerDamagedCount[state->GetTeam()];
                    msgStr = fmtstring(sObjectMgr.GetMangosStringForDBCLocale(LANG_BG_WG_TOWER_DAMAGED), msgStr.c_str());
                    sWorld.SendZoneText(ZONE_WINTERGRASP, msgStr.c_str());
                    break;
            }

			if(obj->GetGOInfo()->displayId == 7877)
				sWorld.SendZoneText(ZONE_WINTERGRASP, sObjectMgr.GetMangosStringForDBCLocale(LANG_BG_WG_FORTRESS_UNDER_ATTACK));

        }
        else if (eventId == obj->GetGOInfo()->building.destroyedEvent)
        {
            state->damageState = DAMAGE_DESTROYED;

			switch(obj->GetEntry())
			{
				case 192810:
					sWorld.SendZoneText(ZONE_WINTERGRASP, fmtstring("La porte du joug d'hiver est detruite !"));
					m_defender = getAttackerTeam();
					ChangeFortressSpawns(m_defender);
					CharacterDatabase.PQuery("UPDATE needed_variables set needed_variables.value = %u where needed_variables.key = '1';",m_defender);
					m_changeDefender = true;
					EndBattle();
					break;
				case 192030:
				case 192031:
				case 192032:
				case 192033:
                    ModifyWorkshopCount(state->GetTeam(), false);
                    msgStr = fmtstring(sObjectMgr.GetMangosStringForDBCLocale(LANG_BG_WG_WORKSHOP_DESTROYED), msgStr.c_str(), sObjectMgr.GetMangosStringForDBCLocale(getDefenderTeam() == BG_TEAM_ALLIANCE ? LANG_BG_ALLY : LANG_BG_HORDE));
                    sWorld.SendZoneText(ZONE_WINTERGRASP, msgStr.c_str());
                    break;
				case 190356:
				case 190357:
				case 190358:
                    --m_towerDamagedCount[state->GetTeam()];
                    ++m_towerDestroyedCount[state->GetTeam()];
                    if (state->GetTeam() == getAttackerTeam())
                    {
						// Update Tower stacks
                        uint32 attStack = 3 - m_towerDestroyedCount[getAttackerTeam()];
                        if (m_towerDestroyedCount[state->GetTeam()])
                        {
                            for (PlayerSet::iterator itr = m_players[getDefenderTeam()].begin(); itr != m_players[getDefenderTeam()].end(); ++itr)
                                if ((*itr)->getLevel() > 69)
                                    (*itr)->SetAuraStack(SPELL_TOWER_CONTROL, (*itr), m_towerDestroyedCount[getAttackerTeam()]);
                        }

                        if (attStack)
                        {
                            for (PlayerSet::iterator itr = m_players[getAttackerTeam()].begin(); itr != m_players[getAttackerTeam()].end(); ++itr)
                                if ((*itr)->getLevel() > 69)
                                    (*itr)->SetAuraStack(SPELL_TOWER_CONTROL, (*itr), attStack);
                        }
                        else
                        {
                            if (m_timer < 600000)
                                m_timer = 0;
                            else
                                m_timer = m_timer - 600000; // - 10 mins
                        }
                    }
                    msgStr = fmtstring(sObjectMgr.GetMangosStringForDBCLocale(LANG_BG_WG_TOWER_DESTROYED), msgStr.c_str());
                    sWorld.SendZoneText(ZONE_WINTERGRASP, msgStr.c_str());
                    break;
            }
			if(obj->GetGOInfo()->displayId == 7877)
				sWorld.SendZoneText(ZONE_WINTERGRASP, sObjectMgr.GetMangosStringForDBCLocale(LANG_BG_WG_FORTRESS_UNDER_ATTACK));
            BroadcastStateChange(state);
        }
    }
}

void OutdoorPvPWG::RemoveOfflinePlayerWGAuras()
{
    // if server crashed while in battle there could be players with rank or tenacity
    CharacterDatabase.PExecute("DELETE FROM character_aura WHERE spell IN (%u, %u, %u, %u, %u)",
        SPELL_RECRUIT, SPELL_CORPORAL, SPELL_LIEUTENANT, SPELL_TENACITY, SPELL_TOWER_CONTROL);
}

void OutdoorPvPWG::ModifyWorkshopCount(BattleGroundTeamId team, bool add)
{
    if (team == BG_TEAM_NEUTRAL)
        return;

    if (add)
        ++m_workshopCount[team];
    else if (m_workshopCount[team])
        --m_workshopCount[team];
    else
        sLog.outError("OutdoorPvPWG::ModifyWorkshopCount: negative workshop count!");

    SendUpdateWorldState(MaxVehNumWorldState[team], m_workshopCount[team] * MAX_VEHICLE_PER_WORKSHOP);
}

uint32 OutdoorPvPWG::GetCreatureEntry(uint32 guidlow, const CreatureData *data)
{
    if (getDefenderTeam() == BG_TEAM_ALLIANCE)
    {
        TeamPairMap::const_iterator itr = m_creEntryPair.find(data->id);
        if (itr != m_creEntryPair.end())
        {
			const_cast<CreatureData*>(data)->modelid_override = 0;
            return itr->second;
        }
    }
    return data->id;
}

OutdoorPvPWGCreType OutdoorPvPWG::GetCreatureType(uint32 entry) const
{
    // VEHICLES, GUARDS and TURRETS gives kill credit
    // OTHER Not in wartime
    // TURRET Only during wartime
    // SPECIAL like "OTHER" but no despawn conditions
    // Entries like Case A: Case: B have their own despawn function
    switch(entry)
    {
        case 27881: // Catapult
        case 28094: // Demolisher
        case 28312: // Alliance Siege Engine
        case 32627: // Horde Siege Engine
        case 28319: // Siege turret
        case 32629: // Siege turret
            return CREATURE_SIEGE_VEHICLE;
        case 28366: // Wintergrasp Tower cannon
            return CREATURE_TURRET;
        case CRE_ENG_A: // Alliance Engineer
        case CRE_ENG_H: // Horde Engineer
            return CREATURE_ENGINEER;
        case 30739:case 30740: // Champions
        case 32307:case 32308: // Guards
            return CREATURE_GUARD;
        case CRE_SPI_A: // Dwarven Spirit Guide
        case CRE_SPI_H: // Taunka Spirit Guide
            return CREATURE_SPIRIT_GUIDE;
        case 6491: // Spirit Healers
            return CREATURE_SPIRIT_HEALER;
        case 31101:case 31051: // Hoodoo Master & Sorceress
        case 31102:case 31052: // Vieron Blazefeather & Bowyer
        case 31107:case 31109: // Lieutenant & Senior Demolitionist
        case 31151:case 31153: // Tactical Officer
        case 31106:case 31108: // Siegesmith & Siege Master
        case 31053:case 31054: // Primalist & Anchorite
        case 31091:case 31036: // Commander
            return CREATURE_QUESTGIVER;
        case 32615:case 32626: // Warbringer && Brigadier General
        case 32296:case 32294: // Quartermaster
        case 30870:case 30869: // Flight Masters
            return CREATURE_SPECIAL;
        default:
            return CREATURE_OTHER; // Revenants, Elementals, etc
    }
}

void OutdoorPvPWG::OnCreatureCreate(Creature *creature, bool add)
{
	uint32 entry = creature->GetEntry();
    switch(GetCreatureType(entry))
    {
        case CREATURE_SIEGE_VEHICLE:
        {
            /*if (!creature->isSummon())
                return;

            BattleGroundTeamId team;
            if (add)
            {
                if (creature->getFaction() == WintergraspFaction[BG_TEAM_ALLIANCE])
                    team = BG_TEAM_ALLIANCE;
                else if (creature->getFaction() == WintergraspFaction[BG_TEAM_HORDE])
                    team = BG_TEAM_HORDE;
                else
                    return;

                /*if (uint32 engLowguid = GUID_LOPART(((TempSummon*)creature)->GetSummonerGUID()))
                {
                    if (OPvPCapturePointWG *workshop = GetWorkshopByEngGuid(engLowguid))
                    {
                        if (CanBuildVehicle(workshop))
                        {
                            m_vehicles[team].insert(creature);
                            //workshop->m_vehicles.insert(creature);
                        }
                        else
                        {
                            creature->setDeathState(DEAD);
                            creature->SetRespawnTime(DAY);
                            return;
                        }
                    }
                }

                if (m_tenacityStack > 0 && team == BG_TEAM_ALLIANCE)
                    creature->SetAuraStack(SPELL_TENACITY_VEHICLE, creature, m_tenacityStack);
                else if (m_tenacityStack < 0 && team == BG_TEAM_HORDE)
                    creature->SetAuraStack(SPELL_TENACITY_VEHICLE, creature, -m_tenacityStack);
            }
            else // the faction may be changed in uncharm
            {
                // TODO: now you have to wait until the corpse of vehicle disappear to build a new one
                if (m_vehicles[BG_TEAM_ALLIANCE].erase(creature))
                    team = BG_TEAM_ALLIANCE;
                else if (m_vehicles[BG_TEAM_HORDE].erase(creature))
                    team = BG_TEAM_HORDE;
                else
                    return;
            }
            SendUpdateWorldState(VehNumWorldState[team], m_vehicles[team].size());*/
            break;
        }
        case CREATURE_QUESTGIVER:
            if (add)
				m_questgivers[creature->GetDBTableGUIDLow()] = creature;
			else
				m_questgivers.erase(creature->GetDBTableGUIDLow());
            break;
        case CREATURE_ENGINEER:
            for (OutdoorPvP::OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
            {
                if (OPvPCapturePointWG *workshop = dynamic_cast<OPvPCapturePointWG*>(itr->second))
                    if (workshop->m_engGuid == creature->GetDBTableGUIDLow())
                    {
                        workshop->m_engineer = add ? creature : NULL;
						workshop->UpdateEngineerData();
                        break;
                    }
            }
            break;
        case CREATURE_SPIRIT_GUIDE:
            for (OutdoorPvP::OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
            {
                if (OPvPCapturePointWG *workshop = dynamic_cast<OPvPCapturePointWG*>(itr->second))
                    if (workshop->m_spiGuid == creature->GetDBTableGUIDLow())
                    {
                        workshop->m_spiritguide = add ? creature : NULL;
                        break;
                    }
            }
            creature->CastSpell(creature, SPELL_SPIRITUAL_IMMUNITY, true);
        case CREATURE_SPIRIT_HEALER:
        case CREATURE_TURRET:
        case CREATURE_OTHER:
            if (add)
                UpdateCreatureInfo(creature);
        default:
            if (add)
                m_creatures.insert(creature->GetGUID());
            else
                m_creatures.erase(creature->GetGUID());
            break;
    }

	switch(entry)
    {
		case 32615:
		case 31107:
		case 31102:
		case 31106:
		case 31101:
		case 31151:
		case 31091:
		case 32296:
		case 31053:
		case 39173:
			if(m_defender == BG_TEAM_ALLIANCE)
			{
				creature->SetPhaseMask(2,true);
				creature->ForcedDespawn(500);
			}
			else
				creature->Respawn();
			Horde_Spawns.push_back(creature->GetGUID());
			break;
		case 32626:
		case 31109:
		case 31052:
		case 31108:
		case 31036:
		case 31153:
		case 32294:
		case 31054:
		case 31051:
		case 30488:
		case 39172:
			if(m_defender == BG_TEAM_HORDE)
			{
				creature->SetPhaseMask(2,true);
				creature->ForcedDespawn(500);
			}
			else
				creature->Respawn();
			Alliance_Spawns.push_back(creature->GetGUID());
			break;
		case 30499:
		case 30400:
			creature->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);
			FortressEngineer.push_back(creature->GetGUID());
			break;
		case 30872:
		case 30873:
		case 30875:
		case 30876:
		case 30877:
		case 30842:
		case 30845:
		case 30846:
		case 30847:
		case 30848:
		case 30849:
		case 34300:
			ToDespawnInWarCr.push_back(creature->GetGUID());
			break;
		case 27881:
		case 28094:
			if(creature->GetCreatureInfo())
			{
				if(creature->GetCreatureInfo()->faction_A == creature->getFaction())
					m_vehicles[BG_TEAM_ALLIANCE].insert(creature->GetGUID());
				else if(creature->GetCreatureInfo()->faction_H == creature->getFaction())
					m_vehicles[BG_TEAM_HORDE].insert(creature->GetGUID());
			}
			SendInitWorldStatesTo();
			UpdateTenacityStack();
			break;
		case 28312:
			m_vehicles[BG_TEAM_ALLIANCE].insert(creature->GetGUID());
			SendInitWorldStatesTo();
			UpdateTenacityStack();
			break;		
		case 32627:
			m_vehicles[BG_TEAM_HORDE].insert(creature->GetGUID());
			SendInitWorldStatesTo();
			UpdateTenacityStack();
			break;
		case 28366:
			FortressTurrets_Spawns.push_back(creature->GetGUID());
			break;
		case 30739:
		case 30740:
			{
				FortressGuards.push_back(creature->GetGUID());
				/*if(getDefenderTeam() == BG_TEAM_ALLIANCE)
					creature->UpdateEntry(30740,ALLIANCE);
				else
					creature->UpdateEntry(30739,HORDE);
				creature->setFaction(WintergraspFaction[getDefenderTeam()]);*/
			}
			break;
		case 31842:
		case 31841:
			if(const CreatureData* creData = sObjectMgr.GetCreatureData(creature->GetDBTableGUIDLow()))
			{
				if(creData->posX < 5460.0f && creData->posX > 5450.0f && creData->posY < 2850.0f && creData->posY > 2830.0f)
				{
					if(creData->id == 31842)
						fortress_spirit[BG_TEAM_ALLIANCE] = creature->GetGUID();
					else if(creData->id == 31841)
						fortress_spirit[BG_TEAM_HORDE] = creature->GetGUID();
				}
			}
        default:
            break;
  }
}

void OutdoorPvPWG::OnKilledCreature(Creature* cr)
{
	switch(cr->GetEntry())
	{
		case 27881:
		case 28094:
		case 28312:
		case 32627:
			m_vehicles[BG_TEAM_ALLIANCE].erase(cr->GetGUID());
			m_vehicles[BG_TEAM_HORDE].erase(cr->GetGUID());
			SendInitWorldStatesTo();
			break;
	}
}

void OutdoorPvPWG::OnGameObjectCreate(GameObject *go, bool add)
{
    OutdoorPvP::OnGameObjectCreate(go, add);

    if (UpdateGameObjectInfo(go))
    {
        if (add) m_gobjects.insert(go);
        else m_gobjects.erase(go);
    }
    //do we need to store building?
    else if (go->GetGoType() == GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING)
    {
        BuildingStateMap::const_iterator itr = m_buildingStates.find(go->GetDBTableGUIDLow());
        if (itr != m_buildingStates.end())
        {
            itr->second->building = add ? go : NULL;
            if (go->GetGOInfo()->displayId == 7878 || go->GetGOInfo()->displayId == 7900)
                itr->second->type = BUILDING_TOWER;
            if (!add || itr->second->damageState == DAMAGE_INTACT && !itr->second->health)
                itr->second->health = go->GetGOValue()->building.health;
            else
            {
                go->GetGOValue()->building.health = itr->second->health;
                if (itr->second->damageState == DAMAGE_DAMAGED)
                    go->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_DAMAGED);
                else if (itr->second->damageState == DAMAGE_DESTROYED)
                    go->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_DESTROYED);
            }
        }
    }

	if(go->GetGoType() == GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING)
		BuildingDestroyables.push_back(go->GetGUID());

	switch(go->GetEntry())
	{
		case 192829:
			TitanRelic = go;
			break;
		case 192254:
		case 192255:
		case 192284:
		case 192285:
			if(go->GetPositionX() > 5073.0f && go->GetPositionX() < 5392.0f && go->GetPositionY() > 2550.0f && go->GetPositionY() < 3100.0f)
				FortressPicBanners.push_back(go->GetGUID());
			if(go->GetDistance2d(4945.7f,2389.8f) < 70.0f)
				NESpawnsGo.push_back(go->GetGUID());
			else if(go->GetDistance2d(4954.2f,3384.1f) < 70.0f)
				NWSpawnsGo.push_back(go->GetGUID());
			else if(go->GetDistance2d(4354.6f,3301.6f) < 70.0f)
				SWSpawnsGo.push_back(go->GetGUID());
			else if(go->GetDistance2d(4359.9f,2347.7f) < 70.0f)
				SESpawnsGo.push_back(go->GetGUID());
			break;
		// TOWERS
		case 190356:
		case 190357:
		case 190358:
			break;
		case 194162:
			DoorColisionGUID = go->GetGUID();
			break;
	}
	if(go->GetGOInfo()->displayId == 8257)
	{
		if(go->GetPositionX() > 5073.0f && go->GetPositionX() < 5392.0f && go->GetPositionY() > 2550.0f && go->GetPositionY() < 3100.0f)
			FortressLargeBanners.push_back(go->GetGUID());
		if(go->GetDistance2d(4945.7f,2389.8f) < 70.0f)
			NESpawnsGo.push_back(go->GetGUID());
		else if(go->GetDistance2d(4954.2f,3384.1f) < 70.0f)
			NWSpawnsGo.push_back(go->GetGUID());
		else if(go->GetDistance2d(4354.6f,3301.6f) < 70.0f)
			SWSpawnsGo.push_back(go->GetGUID());
		else if(go->GetDistance2d(4359.9f,2347.7f) < 70.0f)
			SESpawnsGo.push_back(go->GetGUID());
	}

}

void OutdoorPvPWG::UpdateAllWorldObject()
{
    // update cre and go factions
    for (GameObjectSet::iterator itr = m_gobjects.begin(); itr != m_gobjects.end(); ++itr)
        UpdateGameObjectInfo(*itr);
    /*for (CreatureSet::iterator itr = m_creatures.begin(); itr != m_creatures.end(); ++itr)
        UpdateCreatureInfo(*itr);*/
    for (QuestGiverMap::iterator itr = m_questgivers.begin(); itr != m_questgivers.end(); ++itr)
        UpdateQuestGiverPosition((*itr).first, (*itr).second);

    // rebuild and update building states
    RebuildAllBuildings();

    // update capture points
    for (OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
        if (OPvPCapturePointWG *workshop = dynamic_cast<OPvPCapturePointWG*>(itr->second))
            workshop->SetTeamByBuildingState();
}

void OutdoorPvPWG::RebuildAllBuildings()
{
    for (BuildingStateMap::const_iterator itr = m_buildingStates.begin(); itr != m_buildingStates.end(); ++itr)
    {
        if (itr->second->building)
        {
            UpdateGameObjectInfo(itr->second->building);
            itr->second->building->Rebuild();
            itr->second->health = itr->second->building->GetGOValue()->building.health;
        }
        else
            itr->second->health = 0;

        if (itr->second->damageState == DAMAGE_DESTROYED)
        {
            if (itr->second->type == BUILDING_WORKSHOP)
                ModifyWorkshopCount(itr->second->GetTeam(), true);
        }

        itr->second->damageState = DAMAGE_INTACT;
        itr->second->SetTeam(getDefenderTeam() == BG_TEAM_ALLIANCE ? OTHER_TEAM(itr->second->defaultTeam) : itr->second->defaultTeam);
    }
    m_towerDamagedCount[BG_TEAM_ALLIANCE] = 0;
    m_towerDestroyedCount[BG_TEAM_ALLIANCE] = 0;
    m_towerDamagedCount[BG_TEAM_HORDE] = 0;
    m_towerDestroyedCount[BG_TEAM_HORDE] = 0;
}

void OutdoorPvPWG::SendInitWorldStatesTo(Player *player) const
{
    WorldPacket data(SMSG_INIT_WORLD_STATES, (4+4+4+2+(m_buildingStates.size()*8)));
    data << uint32(571);
    data << uint32(ZONE_WINTERGRASP);
    data << uint32(0);
    data << uint16(4+2+4+m_buildingStates.size());

    data << uint32(WS_WG_CONTROL_ALLIANCE) << uint32(getDefenderTeam() == BG_TEAM_ALLIANCE ? 1 : 0);
    data << uint32(WS_WG_CONTROL_HORDE) << uint32(getDefenderTeam() != BG_TEAM_ALLIANCE ? 1 : 0);
    data << uint32(WS_WG_NEXT_BATTLE) << uint32(isWarTime() ? 0 : 1);
    data << uint32(WS_WG_BATTLEPANEL) << uint32(isWarTime() ? 1 : 0);

    for (uint32 i = 0; i < 2; ++i)
        data << ClockWorldState[i] << m_clock[i];

    data << uint32(WS_WG_VEH_HOR) << uint32(m_vehicles[BG_TEAM_HORDE].size());
    data << uint32(WS_WG_VEH_HOR_TOT) << m_workshopCount[BG_TEAM_HORDE] * MAX_VEHICLE_PER_WORKSHOP;
    data << uint32(WS_WG_VEH_ALL) << uint32(m_vehicles[BG_TEAM_ALLIANCE].size());
    data << uint32(WS_WG_VEH_ALL_TOT) << m_workshopCount[BG_TEAM_ALLIANCE] * MAX_VEHICLE_PER_WORKSHOP;

    for (BuildingStateMap::const_iterator itr = m_buildingStates.begin(); itr != m_buildingStates.end(); ++itr)
        itr->second->FillData(data);

    if (player)
        player->GetSession()->SendPacket(&data);
    else
        BroadcastPacket(data);
}

bool OutdoorPvPWG::CanCreateVehicle(BattleGroundTeamId team)
{
	if(m_vehicles[team].size() < m_workshopCount[team] * MAX_VEHICLE_PER_WORKSHOP)
		return true;

	return false;
}

void OutdoorPvPWG::BroadcastStateChange(BuildingState *state) const
{
    if (m_sendUpdate)
        for (uint32 team = 0; team < 2; ++team)
            for (PlayerSet::const_iterator p_itr = m_players[team].begin(); p_itr != m_players[team].end(); ++p_itr)
                state->SendUpdate(*p_itr);
}

// Called at Start and Battle End
bool OutdoorPvPWG::UpdateCreatureInfo(Creature *creature)
{
    if (!creature)
        return false;
    uint32 entry = creature->GetEntry();
    switch(GetCreatureType(entry))
    {
        case CREATURE_TURRET:
            if (isWarTime())
            {
                if (!creature->isAlive())
                    creature->Respawn();
                creature->setFaction(WintergraspFaction[getDefenderTeam()]);
                creature->SetVisibility(VISIBILITY_ON);
            }
            else
            {
                if (creature->isVehicle() && creature->GetVehicleKit())
                    creature->GetVehicleKit()->RemoveAllPassengers();
                creature->SetVisibility(VISIBILITY_OFF);
                creature->setFaction(35);
            }
            return false;
        case CREATURE_OTHER:
            if (isWarTime())
            {
                creature->SetVisibility(VISIBILITY_OFF);
                creature->setFaction(35);
            }
            else
            {
				creature->setFaction(creature->GetCreatureInfo()->faction_A);
                creature->SetVisibility(VISIBILITY_ON);
            }
            return false;
        case CREATURE_SPIRIT_GUIDE:
            if (isWarTime())
            {
                creature->SetVisibility(VISIBILITY_ON);
                //creature->setDeathState(ALIVE);
            }
            else
            {
                creature->SetVisibility(VISIBILITY_OFF);
                //creature->setDeathState(DEAD);
            }
            return false;
        case CREATURE_SPIRIT_HEALER:
            creature->SetVisibility(isWarTime() ? VISIBILITY_OFF : VISIBILITY_ON);
            return false;
        case CREATURE_ENGINEER:
            return false;
        case CREATURE_SIEGE_VEHICLE:
            //creature->DisappearAndDie();
            return false;
        case CREATURE_GUARD:
        case CREATURE_SPECIAL:
        {
			TeamPairMap::const_iterator itr = m_creEntryPair.find(creature->GetCreatureInfo()->Entry);
            if (itr != m_creEntryPair.end())
            {
                entry = getDefenderTeam() == BG_TEAM_ALLIANCE ? itr->second : itr->first;
                _RespawnCreatureIfNeeded(creature, entry);
            }
            return false;
        }
        default:
            return false;
    }
}

bool OutdoorPvPWG::UpdateQuestGiverPosition(uint32 guid, Creature *creature)
{
    ASSERT(guid);
    /*Position pos = m_qgPosMap[std::pair<uint32, bool>(guid, getDefenderTeam() == BG_TEAM_HORDE)];

    if (creature && creature->IsInWorld())
    {
        // if not questgiver or position is the same, do nothing
        if (creature->GetPositionX() == pos.GetPositionX() &&
            creature->GetPositionY() == pos.GetPositionY() &&
            creature->GetPositionZ() == pos.GetPositionZ())
            return false;

        if (creature->isAlive() && creature->isInCombat())
        {
            creature->CombatStop();
            creature->getHostileRefManager().deleteReferences();
        }
		//creature->GetMap()->CreatureRelocation(creature,pos.GetPositionX(),pos.GetPositionY(),pos.GetPositionZ(),pos.GetOrientation());
        //creature->DestroyForNearbyPlayers();
        /*if (!creature->GetMap()->IsLoaded(pos.GetPositionX(), pos.GetPositionY()))
            creature->GetMap()->LoadGrid(pos.GetPositionX(), pos.GetPositionY());
        creature->GetMap()->CreatureRelocation(creature, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation());
        if (!creature->isAlive())
            creature->Respawn();
    }
    else
        sObjectMgr.MoveCreData(guid, 571, pos);*/

    return true;
}

// Return false = Need to rebuild at battle End/Start
//        true  = no need to rebuild (ie: Banners or teleporters)
bool OutdoorPvPWG::UpdateGameObjectInfo(GameObject *go) const
{
	if(!go || !go->GetGOInfo())
		return false;
    uint32 attFaction = 14;
    uint32 defFaction = 14;

    if (isWarTime())
    {
        attFaction = WintergraspFaction[getAttackerTeam()];
        defFaction = WintergraspFaction[getDefenderTeam()];
    }

    switch(go->GetGOInfo()->displayId)
    {
        case 8244: // Defender's Portal - Vehicle Teleporter
            go->SetUInt32Value(GAMEOBJECT_FACTION, WintergraspFaction[getDefenderTeam()]);
            return true;
        case 7967: // Titan relic
            go->SetUInt32Value(GAMEOBJECT_FACTION, WintergraspFaction[getAttackerTeam()]);
            return true;
        case 8165: // Wintergrasp Keep Door
        case 7877: // Wintergrasp Fortress Wall
        case 7878: // Wintergrasp Keep Tower
        case 7906: // Wintergrasp Fortress Gate
        case 7909: // Wintergrasp Wall
            go->SetUInt32Value(GAMEOBJECT_FACTION, defFaction);
            return false;
        case 7900: // Flamewatch Tower - Shadowsight Tower - Winter's Edge Tower
		case 7898:
            go->SetUInt32Value(GAMEOBJECT_FACTION, attFaction);
            return false;
        case 8208: // Goblin Workshop
            OPvPCapturePointWG *workshop = GetWorkshopByGOGuid(go->GetGUID());
            if (workshop)
                go->SetUInt32Value(GAMEOBJECT_FACTION, WintergraspFaction[workshop->m_buildingState->GetTeam()]);
            return false;
    }

    // Note: this is only for test, still need db support
    TeamPairMap::const_iterator itr = m_goDisplayPair.find(go->GetGOInfo()->displayId);
    if (itr != m_goDisplayPair.end())
    {
        go->SetUInt32Value(GAMEOBJECT_DISPLAYID, getDefenderTeam() == BG_TEAM_ALLIANCE ?
            itr->second : itr->first);
        return true;
    }
    return false;
}

void OutdoorPvPWG::HandlePlayerEnterZone(Player * plr, uint32 zone)
{
	if(plr->isGameMaster())
		return;

    if (isWarTime())
    {
        if (plr->getLevel() > 69)
        {
            if (!plr->HasAura(SPELL_RECRUIT) && !plr->HasAura(SPELL_CORPORAL)
                && !plr->HasAura(SPELL_LIEUTENANT))
                plr->CastSpell(plr, SPELL_RECRUIT, true);
			if (BattleGround::GetTeamIndexByTeamId(plr->GetTeam()) == getAttackerTeam())
            {
                if (m_towerDestroyedCount[getAttackerTeam()] < 3)
                    plr->SetAuraStack(SPELL_TOWER_CONTROL, plr, 3 - m_towerDestroyedCount[getAttackerTeam()]);
            }
            else
            {
                if (m_towerDestroyedCount[getAttackerTeam()])
                    plr->SetAuraStack(SPELL_TOWER_CONTROL, plr, m_towerDestroyedCount[getAttackerTeam()]);
            }
        }

		BattleGroundTeamId bgTeam = BattleGround::GetTeamIndexByTeamId(plr->GetTeam());
		if(Group* grp = plr->GetGroup())
			if(!grp->IsWGGroup())
				grp->RemoveMember(plr->GetGUID(),0);

		if(!m_WGGroup[bgTeam])
		{
			m_WGGroup[bgTeam] = new Group;
			m_WGGroup[bgTeam]->Create(plr->GetGUID(),plr->GetName());
			m_WGGroup[bgTeam]->SetWGGroup();
			m_WGGroup[bgTeam]->ConvertToRaid();
		}
		else if(m_WGGroup[bgTeam])
		{
			if(m_WGGroup[bgTeam]->GetMemberSlots().empty())
			{
				m_WGGroup[bgTeam] = new Group;
				m_WGGroup[bgTeam]->Create(plr->GetGUID(),plr->GetName());
				m_WGGroup[bgTeam]->SetWGGroup();
				m_WGGroup[bgTeam]->ConvertToRaid();
			}
			else
				m_WGGroup[bgTeam]->AddMember(plr->GetGUID(),plr->GetName());
		}
    }

    SendInitWorldStatesTo(plr);
    OutdoorPvP::HandlePlayerEnterZone(plr, zone);
    UpdateTenacityStack();
}

// Reapply Auras if needed
void OutdoorPvPWG::HandlePlayerResurrects(Player * plr, uint32 zone)
{
    if (isWarTime())
    {
        if (plr->getLevel() > 69)
        {
            // Tenacity
            if (BattleGround::GetTeamIndexByTeamId(plr->GetTeam()) == BG_TEAM_ALLIANCE && m_tenacityStack > 0 ||
                BattleGround::GetTeamIndexByTeamId(plr->GetTeam()) == BG_TEAM_HORDE && m_tenacityStack < 0)
            {
                if (plr->HasAura(SPELL_TENACITY))
                    plr->RemoveAurasDueToSpell(SPELL_TENACITY);

                int32 newStack = m_tenacityStack < 0 ? -m_tenacityStack : m_tenacityStack;
                if (newStack > 20)
                    newStack = 20;
                plr->SetAuraStack(SPELL_TENACITY, plr, newStack);
            }

            // Tower Control
            if (BattleGround::GetTeamIndexByTeamId(plr->GetTeam()) == getAttackerTeam())
            {
                if (m_towerDestroyedCount[getAttackerTeam()] < 3)
                    plr->SetAuraStack(SPELL_TOWER_CONTROL, plr, 3 - m_towerDestroyedCount[getAttackerTeam()]);
            }
            else
            {
                if (m_towerDestroyedCount[getAttackerTeam()])
                    plr->SetAuraStack(SPELL_TOWER_CONTROL, plr, m_towerDestroyedCount[getAttackerTeam()]);
            }
        }
    }
    OutdoorPvP::HandlePlayerResurrects(plr, zone);
}

void OutdoorPvPWG::HandlePlayerLeaveZone(Player * plr, uint32 zone)
{
    if (!plr->GetSession()->PlayerLogout())
    {
		if(plr->isGameMaster())
			return;

		if(Unit *vehUnit = Unit::GetUnit(*plr,plr->GetVehicleGUID()))
		{
			if(vehUnit->isAlive())
				vehUnit->DealDamage(vehUnit,vehUnit->GetMaxHealth(),NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
		}

        plr->RemoveAurasDueToSpell(SPELL_RECRUIT);
        plr->RemoveAurasDueToSpell(SPELL_CORPORAL);
        plr->RemoveAurasDueToSpell(SPELL_LIEUTENANT);
        plr->RemoveAurasDueToSpell(SPELL_TOWER_CONTROL);
        plr->RemoveAurasDueToSpell(SPELL_SPIRITUAL_IMMUNITY);
    }
    plr->RemoveAurasDueToSpell(SPELL_TENACITY);

	if(isWarTime())
	{
		BattleGroundTeamId bgTeam = BattleGround::GetTeamIndexByTeamId(plr->GetTeam());
		if(Group* grp = plr->GetGroup())
			grp->RemoveMember(plr->GetGUID(),0);
	}
    OutdoorPvP::HandlePlayerLeaveZone(plr, zone);
    UpdateTenacityStack();
}

void OutdoorPvPWG::PromotePlayer(Player *killer) const
{
    Aura *aur;
    if (aur = killer->GetAura(SPELL_RECRUIT,0))
    {
        if (aur->GetStackAmount() >= 5)
        {
            killer->RemoveAurasDueToSpell(SPELL_RECRUIT);
            killer->CastSpell(killer, SPELL_CORPORAL, true);
            ChatHandler(killer).PSendSysMessage(LANG_BG_WG_RANK1);
        }
        else
            killer->CastSpell(killer, SPELL_RECRUIT, true);
    }
    else if (aur = killer->GetAura(SPELL_CORPORAL,0))
    {
        if (aur->GetStackAmount() >= 5)
        {
            killer->RemoveAurasDueToSpell(SPELL_CORPORAL);
            killer->CastSpell(killer, SPELL_LIEUTENANT, true);
            ChatHandler(killer).PSendSysMessage(LANG_BG_WG_RANK2);
        }
        else
            killer->CastSpell(killer, SPELL_CORPORAL, true);
    }
}

void OutdoorPvPWG::HandleKill(Player *killer, Unit *victim)
{
    if (!isWarTime())
        return;

    bool ok = false;
    if (victim->GetTypeId() == TYPEID_PLAYER)
    {
        if (victim->getLevel() >= 70)
            ok = true;
        killer->RewardPlayerAndGroupAtEvent(CRE_PVP_KILL, victim);
    }
    else
    {
        switch(GetCreatureType(victim->GetEntry()))
        {
            case CREATURE_SIEGE_VEHICLE:
                killer->RewardPlayerAndGroupAtEvent(CRE_PVP_KILL_V, victim);
                ok = true;
                break;
            case CREATURE_GUARD:
                killer->RewardPlayerAndGroupAtEvent(CRE_PVP_KILL, victim);
                ok = true;
                break;
            case CREATURE_TURRET:
                ok = true;
                break;
        }
    }

    if (ok)
    {
        if (Group *pGroup = killer->GetGroup())
        {
            for (GroupReference *itr = pGroup->GetFirstMember(); itr != NULL; itr = itr->next())
                if (itr->getSource()->IsAtGroupRewardDistance(killer) && itr->getSource()->getLevel() > 69)
                    PromotePlayer(itr->getSource());
        }
        else if (killer->getLevel() > 69)
            PromotePlayer(killer);
    }
}

// Recalculates Tenacity and applies it to Players / Vehicles
void OutdoorPvPWG::UpdateTenacityStack()
{
    if (!isWarTime())
        return;

    BattleGroundTeamId team = BG_TEAM_NEUTRAL;
    uint32 allianceNum = 0;
    uint32 hordeNum = 0;
    int32 newStack = 0;

    for (PlayerSet::iterator itr = m_players[BG_TEAM_ALLIANCE].begin(); itr != m_players[BG_TEAM_ALLIANCE].end(); ++itr)
        if ((*itr)->getLevel() > 69)
            ++allianceNum;

    for (PlayerSet::iterator itr = m_players[BG_TEAM_HORDE].begin(); itr != m_players[BG_TEAM_HORDE].end(); ++itr)
        if ((*itr)->getLevel() > 69)
            ++hordeNum;

    if (allianceNum && hordeNum)
    {
        if (allianceNum < hordeNum)
            newStack = int32((float(hordeNum) / float(allianceNum) - 1)*4); // positive, should cast on alliance
        else if (allianceNum > hordeNum)
            newStack = int32((1 - float(allianceNum) / float(hordeNum))*4); // negative, should cast on horde
    }

    if (newStack == m_tenacityStack)
        return;

    if (m_tenacityStack > 0 && newStack <= 0) // old buff was on alliance
        team = BG_TEAM_ALLIANCE;
    else if (m_tenacityStack < 0 && newStack >= 0) // old buff was on horde
        team = BG_TEAM_HORDE;

    m_tenacityStack = newStack;

    // Remove old buff
    if (team != BG_TEAM_NEUTRAL)
    {
        for (PlayerSet::const_iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
            if ((*itr)->getLevel() > 69)
                (*itr)->RemoveAurasDueToSpell(SPELL_TENACITY);

		for (std::set<uint64>::const_iterator itr = m_vehicles[team].begin(); itr != m_vehicles[team].end(); ++itr)
			if(GetMap())
				if(Creature* cr = GetMap()->GetCreatureOrPetOrVehicle(*itr))
					if(cr->isAlive())
						cr->RemoveAurasDueToSpell(SPELL_TENACITY_VEHICLE);
    }

    // Apply new buff
    if (newStack)
    {
        team = newStack > 0 ? BG_TEAM_ALLIANCE : BG_TEAM_HORDE;
        if (newStack < 0)
            newStack = -newStack;
        if (newStack > 20)
            newStack = 20;

        for (PlayerSet::const_iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
            if ((*itr)->getLevel() > 69)
			{
				float pLife = float((*itr)->GetHealth()) / float((*itr)->GetMaxHealth());
				uint32 Life = pLife * (*itr)->GetMaxHealth();
                (*itr)->SetAuraStack(SPELL_TENACITY, (*itr), newStack);
				(*itr)->SetHealth(Life > (*itr)->GetMaxHealth() / 10 ? Life : (*itr)->GetMaxHealth() / 10);
			}

			for (std::set<uint64>::const_iterator itr = m_vehicles[team].begin(); itr != m_vehicles[team].end(); ++itr)
				if(GetMap())
					if(Creature* cr = GetMap()->GetCreatureOrPetOrVehicle(*itr))
						if(cr->isAlive())
						{
							float pLife = float(cr->GetHealth()) / float(cr->GetMaxHealth());
							uint32 Life = pLife * cr->GetMaxHealth();
							cr->SetAuraStack(SPELL_TENACITY_VEHICLE, cr, newStack);
							cr->SetHealth(Life > cr->GetMaxHealth() / 10 ? Life : cr->GetMaxHealth() / 10);
						}
    }
}

void OutdoorPvPWG::UpdateClockDigit(uint32 &timer, uint32 digit, uint32 mod)
{
    uint32 value = timer%mod;
    if (m_clock[digit] != value)
    {
        m_clock[digit] = value;
        SendUpdateWorldState(ClockWorldState[digit], (timer + time(NULL)));
        sWorld.SetWintergrapsTimer(timer + time(NULL), digit);
    }
}

void OutdoorPvPWG::UpdateClock()
{
    uint32 timer = m_timer / 1000;
    if (!isWarTime())
        UpdateClockDigit(timer, 1, 10);
    else
        UpdateClockDigit(timer, 0, 10);
}

bool OutdoorPvPWG::Update(uint32 diff)
{
	if(DBUpdate_Timer < diff)
	{
		CharacterDatabase.PExecute("UPDATE needed_variables set value = '%u' where needed_variables.key = '0'", m_timer/60000);
    	DBUpdate_Timer = 60000;
	}
	else
		DBUpdate_Timer -= diff;

    if (m_timer > diff)
    {
        m_timer -= diff;

        if (isWarTime())
        {
            OutdoorPvP::Update(diff); // update capture points

            /*********************************************************/
            /***        BATTLEGROUND RESSURECTION SYSTEM           ***/
            /*********************************************************/

            //this should be handled by spell system
            m_LastResurrectTime += diff;
            if (m_LastResurrectTime >= RESURRECTION_INTERVAL)
            {
                if (GetReviveQueueSize())
                {
                    for (std::map<uint64, std::vector<uint64> >::iterator itr = m_ReviveQueue.begin(); itr != m_ReviveQueue.end(); ++itr)
                    {
                        Creature *sh = NULL;
                        for (std::vector<uint64>::const_iterator itr2 = (itr->second).begin(); itr2 != (itr->second).end(); ++itr2)
                        {
                            Player *plr = sObjectMgr.GetPlayer(*itr2);
                            if (!plr)
                                continue;

                            if (!sh && plr->IsInWorld())
                            {
                                sh = plr->GetMap()->GetCreature(itr->first);
                                // only for visual effect
                                if (sh)
								{
                                    // Spirit Heal, effect 117
									sh->CastStop();
                                    sh->CastSpell(plr, 22012, true);
								}
                            }

                            // Resurrection visual
                            plr->CastSpell(plr, 24171, true);
                            m_ResurrectQueue.push_back(*itr2);
                        }
                        (itr->second).clear();
                    }

                    m_ReviveQueue.clear();
                    m_LastResurrectTime = 0;
                }
                else
                    // queue is clear and time passed, just update last resurrection time
                    m_LastResurrectTime = 0;
            }
            else if (m_LastResurrectTime > 500)    // Resurrect players only half a second later, to see spirit heal effect on NPC
            {
                for (std::vector<uint64>::const_iterator itr = m_ResurrectQueue.begin(); itr != m_ResurrectQueue.end(); ++itr)
                {
                    Player *plr = sObjectMgr.GetPlayer(*itr);
                    if (!plr)
                        continue;
                    plr->ResurrectPlayer(1.0f);
                    plr->CastSpell(plr, 6962, true);
                    plr->CastSpell(plr, 44535, true);
                    ObjectAccessor::Instance().ConvertCorpseForPlayer(*itr);
                }
                m_ResurrectQueue.clear();
            }
        }
        UpdateClock();
    }
    else
    {
        m_sendUpdate = false;
        int32 entry = LANG_BG_WG_DEFENDED;

        if (m_changeDefender)
        {
            m_changeDefender = false;
            entry = LANG_BG_WG_CAPTURED;;
        }

        if (isWarTime())
        {
            if (m_timer != 1) // 1 = forceStopBattle
                sWorld.SendZoneText(ZONE_WINTERGRASP, fmtstring(sObjectMgr.GetMangosStringForDBCLocale(entry), sObjectMgr.GetMangosStringForDBCLocale(getDefenderTeam() == BG_TEAM_ALLIANCE ? LANG_BG_ALLY : LANG_BG_HORDE)));
            EndBattle();
        }
        else
        {
            if (m_timer != 1) // 1 = forceStartBattle
                sWorld.SendZoneText(ZONE_WINTERGRASP, sObjectMgr.GetMangosStringForDBCLocale(LANG_BG_WG_BATTLE_STARTS));
            StartBattle();
        }

        UpdateAllWorldObject();
        UpdateClock();

        SendInitWorldStatesTo();
        m_sendUpdate = true;
    }

    return false;
}

void OutdoorPvPWG::forceStartBattle()
{ // Uptime will do all the work
    m_wartime = false;

    if (m_timer != 1)
    {
        m_timer = 1;
        sWorld.SendZoneText(ZONE_WINTERGRASP, sObjectMgr.GetMangosStringForDBCLocale(LANG_BG_WG_BATTLE_FORCE_START));
    }
}

void OutdoorPvPWG::forceStopBattle()
{ // Uptime will do all the work.

    if (!isWarTime())
        m_wartime = true;

    if (m_timer != 1)
    {
        m_timer = 1;
		sWorld.SendZoneText(ZONE_WINTERGRASP, sObjectMgr.GetMangosStringForDBCLocale(LANG_BG_WG_BATTLE_FORCE_STOP));
    }
}

void OutdoorPvPWG::forceChangeTeam()
{
    m_changeDefender = true;
    m_timer = 1;
    sWorld.SendZoneText(ZONE_WINTERGRASP, fmtstring(sObjectMgr.GetMangosStringForDBCLocale(LANG_BG_WG_SWITCH_FACTION), sObjectMgr.GetMangosStringForDBCLocale(getAttackerTeam() == BG_TEAM_ALLIANCE ? LANG_BG_ALLY : LANG_BG_HORDE)));
    if (isWarTime())
        forceStartBattle();
    else
        forceStopBattle();
}

// Can be forced by gm's while in battle so have to reset in case it was wartime
void OutdoorPvPWG::StartBattle()
{
    m_wartime = true;
    m_timer = 30 * MINUTE * IN_MILLISECONDS;


    // Remove Essence of Wintergrasp to all players
    sWorld.setState(0, BG_TEAM_NEUTRAL);
    sWorld.UpdateAreaDependentAuras();
	sWorld.UpdateBuffForAll(false);
	sWorld.ForceLeaveArchavonVault();
	ChangeFortressSpawns(m_defender);

	for(std::vector<uint64>::iterator itr = BuildingDestroyables.begin(); itr != BuildingDestroyables.end();itr++)
	{
		if(GetMap())
			if(GameObject* go = GetMap()->GetGameObject(*itr))
			{
				go->Rebuild();
			}
	}

	for(std::vector<uint64>::iterator itr = ToDespawnInWarCr.begin(); itr != ToDespawnInWarCr.end();itr++)
	{
		if(GetMap())
			if(Creature* cr = (Creature*)GetMap()->GetCreatureOrPetOrVehicle(*itr))
			{
				cr->SetPhaseMask(2,true);
			}
	}

	

    // destroyed all vehicles
    for (uint32 team = 0; team < 2; ++team)
    {
        while(!m_vehicles[team].empty())
        {
            if(Creature *veh = (Creature*)GetMap()->GetCreatureOrPetOrVehicle(*m_vehicles[team].begin()))
				if(veh->GetVehicleKit())
					veh->GetVehicleKit()->Dismiss();
			m_vehicles[team].erase(m_vehicles[team].begin());
        }
    }

	if(m_WGGroup[BG_TEAM_ALLIANCE])
	{
		m_WGGroup[BG_TEAM_ALLIANCE]->Disband();
		m_WGGroup[BG_TEAM_ALLIANCE] = NULL;
	}
	
	if(m_WGGroup[BG_TEAM_HORDE])
	{
		m_WGGroup[BG_TEAM_HORDE]->Disband();
		m_WGGroup[BG_TEAM_HORDE] = NULL;
	}

    // Remove All Wintergrasp auras. Add Recruit rank and Tower Control
    for (PlayerSet::iterator itr = m_players[getAttackerTeam()].begin(); itr != m_players[getAttackerTeam()].end(); ++itr)
    {
        (*itr)->RemoveAurasDueToSpell(SPELL_RECRUIT);
        (*itr)->RemoveAurasDueToSpell(SPELL_CORPORAL);
        (*itr)->RemoveAurasDueToSpell(SPELL_LIEUTENANT);
        (*itr)->RemoveAurasDueToSpell(SPELL_TOWER_CONTROL);
        (*itr)->RemoveAurasDueToSpell(SPELL_SPIRITUAL_IMMUNITY);
        if ((*itr)->getLevel() > 69)
        {
            (*itr)->SetAuraStack(SPELL_TOWER_CONTROL, (*itr), 3);
            (*itr)->CastSpell(*itr, SPELL_RECRUIT, true);
        }
		
		if(Group* grp = (*itr)->GetGroup())
			if(!grp->IsWGGroup())
				grp->RemoveMember((*itr)->GetGUID(),0);

		if(!m_WGGroup[getAttackerTeam()])
		{
			m_WGGroup[getAttackerTeam()] = new Group;
			m_WGGroup[getAttackerTeam()]->Create((*itr)->GetGUID(),(*itr)->GetName());
			m_WGGroup[getAttackerTeam()]->SetWGGroup();
			m_WGGroup[getAttackerTeam()]->ConvertToRaid();
		}
		else
			m_WGGroup[getAttackerTeam()]->AddMember((*itr)->GetGUID(),(*itr)->GetName());
    }

    // Remove All Wintergrasp auras. Add Recruit rank
    for (PlayerSet::iterator itr = m_players[getDefenderTeam()].begin(); itr != m_players[getDefenderTeam()].end(); ++itr)
    {
        (*itr)->RemoveAurasDueToSpell(SPELL_RECRUIT);
        (*itr)->RemoveAurasDueToSpell(SPELL_CORPORAL);
        (*itr)->RemoveAurasDueToSpell(SPELL_LIEUTENANT);
        (*itr)->RemoveAurasDueToSpell(SPELL_TOWER_CONTROL);
        (*itr)->RemoveAurasDueToSpell(SPELL_SPIRITUAL_IMMUNITY);
        if ((*itr)->getLevel() > 69)
            (*itr)->CastSpell(*itr, SPELL_RECRUIT, true);

		if(Group* grp = (*itr)->GetGroup())
			if(!grp->IsWGGroup())
				grp->RemoveMember((*itr)->GetGUID(),0);

		if(!m_WGGroup[getDefenderTeam()])
		{
			m_WGGroup[getDefenderTeam()] = new Group;
			m_WGGroup[getDefenderTeam()]->Create((*itr)->GetGUID(),(*itr)->GetName());
			m_WGGroup[getDefenderTeam()]->SetWGGroup();
			m_WGGroup[getDefenderTeam()]->ConvertToRaid();
		}
		else
			m_WGGroup[getDefenderTeam()]->AddMember((*itr)->GetGUID(),(*itr)->GetName());
    }
    UpdateTenacityStack();
	UpdateFortressSpirits();
}

void OutdoorPvPWG::EndBattle()
{
    // Cast Essence of Wintergrasp to all players (CheckCast will determine who to cast)
    sWorld.setState(0, getDefenderTeam());
    sWorld.UpdateAreaDependentAuras();

	for(CreatureSet::iterator itr = m_vehicles[BG_TEAM_ALLIANCE].begin(); itr != m_vehicles[BG_TEAM_ALLIANCE].end();itr++)
		if(GetMap())
			if(Creature* cr = (Creature*)GetMap()->GetCreatureOrPetOrVehicle(*itr))
				if(cr->isAlive())
					cr->DealDamage(cr,cr->GetMaxHealth(),NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

	m_vehicles[BG_TEAM_ALLIANCE].clear();

	for(CreatureSet::iterator itr = m_vehicles[BG_TEAM_HORDE].begin(); itr != m_vehicles[BG_TEAM_HORDE].end();itr++)
		if(GetMap())
			if(Creature* cr = (Creature*)GetMap()->GetCreatureOrPetOrVehicle(*itr))
				if(cr->isAlive())
					cr->DealDamage(cr,cr->GetMaxHealth(),NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

	m_vehicles[BG_TEAM_HORDE].clear();

	    for (uint32 team = 0; team < 2; ++team)
    {
        if (m_players[team].empty())
            continue;

        for (PlayerSet::iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
        {
            // When WG ends the zone is cleaned including corpses, revive all players if dead
            if ((*itr)->isDead())
            {
                (*itr)->ResurrectPlayer(1.0f);
                ObjectAccessor::Instance().ConvertCorpseForPlayer((*itr)->GetGUID());
            }
            (*itr)->RemoveAurasDueToSpell(SPELL_TENACITY);
            (*itr)->CombatStop(true);
            (*itr)->getHostileRefManager().deleteReferences();
        }

        if (m_timer == 1) // Battle End was forced so no reward.
        {
            for (PlayerSet::iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
            {
                (*itr)->RemoveAurasDueToSpell(SPELL_RECRUIT);
                (*itr)->RemoveAurasDueToSpell(SPELL_CORPORAL);
                (*itr)->RemoveAurasDueToSpell(SPELL_LIEUTENANT);
                (*itr)->RemoveAurasDueToSpell(SPELL_TOWER_CONTROL);
                (*itr)->RemoveAurasDueToSpell(SPELL_SPIRITUAL_IMMUNITY);
            }
            continue;
        }

        // calculate rewards
        uint32 intactNum = 0;
        uint32 damagedNum = 0;
        for (OutdoorPvP::OPvPCapturePointMap::const_iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
            if (OPvPCapturePointWG *workshop = dynamic_cast<OPvPCapturePointWG*>(itr->second))
                if (workshop->m_buildingState->GetTeam() == team)
                    if (workshop->m_buildingState->damageState == DAMAGE_DAMAGED)
                        ++damagedNum;
                    else if (workshop->m_buildingState->damageState == DAMAGE_INTACT)
                        ++intactNum;

        uint32 spellRewardId = team == getDefenderTeam() ? SPELL_VICTORY_REWARD : SPELL_DEFEAT_REWARD;
        uint32 baseHonor = 0;
        uint32 marks = 0;
        uint32 playersWithRankNum = 0;
        uint32 honor = 0;

        for (PlayerSet::iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
        {

            if ((*itr)->getLevel() < 70)
                continue; // No rewards for level <70

            // give rewards
            if ((*itr)->HasAura(SPELL_LIEUTENANT) || (*itr)->HasAura(SPELL_CORPORAL))
            {
                // TODO - Honor from SpellReward should be shared by team players
                // TODO - Marks should be given depending on Rank but 3 are given
                // each time so Won't give any to recruits
                (*itr)->CastSpell(*itr, spellRewardId, true);
                for (uint32 i = 0; i < intactNum; ++i)
                    (*itr)->CastSpell(*itr, SPELL_INTACT_BUILDING, true);
                for (uint32 i = 0; i < damagedNum; ++i)
                    (*itr)->CastSpell(*itr, SPELL_DAMAGED_BUILDING, true);
                for (uint32 i = 0; i < m_towerDamagedCount[OTHER_TEAM(team)]; ++i)
                    (*itr)->CastSpell(*itr, SPELL_DAMAGED_TOWER, true);
                for (uint32 i = 0; i < m_towerDestroyedCount[OTHER_TEAM(team)]; ++i)
                    (*itr)->CastSpell(*itr, SPELL_DESTROYED_TOWER, true);
            }

            if (team == getDefenderTeam())
            {
                if ((*itr)->HasAura(SPELL_LIEUTENANT) || (*itr)->HasAura(SPELL_CORPORAL))
                {
                    (*itr)->AreaExploredOrEventHappens(A_VICTORY_IN_WG);
                    (*itr)->AreaExploredOrEventHappens(H_VICTORY_IN_WG);
                }
            }
            (*itr)->RemoveAurasDueToSpell(SPELL_RECRUIT);
            (*itr)->RemoveAurasDueToSpell(SPELL_CORPORAL);
            (*itr)->RemoveAurasDueToSpell(SPELL_LIEUTENANT);
            (*itr)->RemoveAurasDueToSpell(SPELL_TOWER_CONTROL);
            (*itr)->RemoveAurasDueToSpell(SPELL_SPIRITUAL_IMMUNITY);
        }
		if(m_WGGroup[team])
			m_WGGroup[team]->Disband();
    }

	for(std::vector<uint64>::iterator itr = ToDespawnInWarCr.begin(); itr != ToDespawnInWarCr.end();itr++)
	{
		if(GetMap())
			if(Creature* cr = (Creature*)GetMap()->GetCreatureOrPetOrVehicle(*itr))
			{
				cr->SetPhaseMask(1,true);
			}
	}

    m_wartime = false;
	CharacterDatabase.Execute("UPDATE needed_variables set value = '150' where needed_variables.key = '0'");
    m_timer = 150 * MINUTE * IN_MILLISECONDS;
    //3.2.0: TeamCastSpell(getAttackerTeam(), SPELL_TELEPORT_DALARAN);
    RemoveOfflinePlayerWGAuras();
	UpdateFortressSpirits();
	SendInitWorldStatesTo();
	sWorld.UpdateBuffForAll(true);
}

void OutdoorPvPWG::UpdateFortressSpirits()
{
	if(GetMap())
	{
		if(fortress_spirit[BG_TEAM_ALLIANCE])
			if(Creature* SpiritA = GetMap()->GetCreatureOrPetOrVehicle(fortress_spirit[BG_TEAM_ALLIANCE]))
			{	
				if(m_defender == BG_TEAM_HORDE)
				{
					SpiritA->SetRespawnTime(7*DAY);
					SpiritA->ForcedDespawn();
				}
				else
				{
					SpiritA->SetRespawnTime(300);
					SpiritA->Respawn();
					if(m_wartime)
						SpiritA->RemoveAurasDueToSpell(58729);
					else
						SpiritA->SetAuraStack(58729,SpiritA,1);
				}
			}
		if(fortress_spirit[BG_TEAM_HORDE])
			if(Creature* SpiritH = GetMap()->GetCreatureOrPetOrVehicle(fortress_spirit[BG_TEAM_HORDE]))
			{
				if(m_defender == BG_TEAM_ALLIANCE)
				{
					SpiritH->SetRespawnTime(7*DAY);
					SpiritH->ForcedDespawn();
				}
				else
				{
					SpiritH->SetRespawnTime(300);
					SpiritH->Respawn();
					if(m_wartime)
						SpiritH->RemoveAurasDueToSpell(58729);
					else
						SpiritH->SetAuraStack(58729,SpiritH,1);
				}
			}
	}
}

bool OutdoorPvPWG::CanBuildVehicle(OPvPCapturePointWG *workshop) const
{
    BattleGroundTeamId team = workshop->m_buildingState->GetTeam();
    if (team == BG_TEAM_NEUTRAL)
        return false;

    return isWarTime()
        && workshop->m_buildingState->damageState != DAMAGE_DESTROYED
        && m_vehicles[team].size() < m_workshopCount[team] * MAX_VEHICLE_PER_WORKSHOP;
}

uint32 OutdoorPvPWG::GetData(uint32 id)
{
    // if can build more vehicles
    if (OPvPCapturePointWG *workshop = GetWorkshopByEngGuid(id))
        return CanBuildVehicle(workshop) ? 1 : 0;

    return 0;
}

void OutdoorPvPWG::RewardMarkOfHonor(Player *plr, uint32 count)
{
    // 'Inactive' this aura prevents the player from gaining honor points and battleground tokens
    if (plr->HasAura(SPELL_AURA_PLAYER_INACTIVE))
        return;
    if (count == 0)
        return;

    ItemPosCountVec dest;
    uint32 no_space_count = 0;
    uint8 msg = plr->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, WG_MARK_OF_HONOR, count, &no_space_count);

    if (msg == EQUIP_ERR_ITEM_NOT_FOUND)
    {
        sLog.outErrorDb("Wintergrasp reward item (Entry %u) not exist in `item_template`.", WG_MARK_OF_HONOR);
        return;
    }

    if (msg != EQUIP_ERR_OK) // convert to possible store amount
        count -= no_space_count;

    if (count != 0 && !dest.empty()) // can add some
        if (Item* item = plr->StoreNewItem(dest, WG_MARK_OF_HONOR, true, 0))
            plr->SendNewItem(item, count, true, false);
}

WorldSafeLocsEntry const* OutdoorPvPWG::GetClosestGraveyard(Player* plr)
{
	if(!plr)
		return NULL;

	if(getDefenderTeam() == BG_TEAM_ALLIANCE && plr->GetTeam() == ALLIANCE ||
		getDefenderTeam() == BG_TEAM_HORDE && plr->GetTeam() == HORDE)
		return sWorldSafeLocsStore.LookupEntry(1328);
	else
	{
		if(plr->GetTeam() == HORDE)
			return sWorldSafeLocsStore.LookupEntry(1330);
		else
			return sWorldSafeLocsStore.LookupEntry(1329);
	}
}

void World::UpdateBuffForAll(bool apply)
{
	for(SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
    {
        if(!itr->second || !itr->second->GetPlayer() || !itr->second->GetPlayer()->IsInWorld() )
            continue;

		if(Player* plr = itr->second->GetPlayer())
		{
			if(!apply)
				plr->RemoveAurasDueToSpell(57940);
			else
				plr->AddWintergraspBuffIfCan(plr->GetMapId());
		}
    }
}

void World::ForceLeaveArchavonVault()
{
	for(SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
    {
        if(!itr->second || !itr->second->GetPlayer() || !itr->second->GetPlayer()->IsInWorld() )
            continue;

		if(Player* plr = itr->second->GetPlayer())
		{
			// Teleport players in archavon vault
			if(plr->GetMapId() == 624)
				plr->TeleportToHomebind();
			else if(plr->GetMapId() == 571)
			{
				// Also die all bad located players at starting WG
				if(!plr->IsWintergraspDefender() && !plr->isGameMaster())
					if(plr->GetPositionX() > 5073.0f && plr->GetPositionX() < 5392.0f && plr->GetPositionY() > 2550.0f && plr->GetPositionY() < 3100.0f)
						plr->DealDamage(plr,plr->GetMaxHealth(),NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

				if(plr->IsWintergraspDefender() && plr->GetZoneId() == 4197 && !(plr->GetPositionX() > 5073.0f && plr->GetPositionX() < 5392.0f && plr->GetPositionY() > 2550.0f && plr->GetPositionY() < 3100.0f))
					plr->TeleportTo(571,5348.66f,2842.69f,410.3f,3.14f);
			}
		}
    }
}
/*
void OutdoorPvPWG::LoadQuestGiverMap(uint32 guid, Position posHorde, Position posAlli)
{
    m_qgPosMap[std::pair<uint32, bool>(guid, true)] = posHorde,
    m_qgPosMap[std::pair<uint32, bool>(guid, false)] = posAlli,
    m_questgivers[guid] = NULL;
    if (getDefenderTeam() == BG_TEAM_ALLIANCE)
        sObjectMgr.MoveCreData(guid, 571, posAlli);
}
*/
OPvPCapturePointWG *OutdoorPvPWG::GetWorkshop(uint32 lowguid) const
{
    if (OPvPCapturePoint *cp = GetCapturePoint(lowguid))
        return dynamic_cast<OPvPCapturePointWG*>(cp);
    return NULL;
}

OPvPCapturePointWG *OutdoorPvPWG::GetWorkshopByEngGuid(uint32 lowguid) const
{
   for (OutdoorPvP::OPvPCapturePointMap::const_iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
       if (OPvPCapturePointWG *workshop = dynamic_cast<OPvPCapturePointWG*>(itr->second))
           if (workshop->m_engGuid == lowguid)
               return workshop;
    return NULL;
}

OPvPCapturePointWG *OutdoorPvPWG::GetWorkshopByGOGuid(uint32 lowguid) const
{
    for (OutdoorPvP::OPvPCapturePointMap::const_iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
        if (OPvPCapturePointWG *workshop = dynamic_cast<OPvPCapturePointWG*>(itr->second))
            if (workshop->m_workshopGuid == lowguid)
                return workshop;
    return NULL;
}

/*########################################################
 * Copy of Battleground system to make Spirit Guides Work
 *#######################################################*/
void OutdoorPvPWG::SendAreaSpiritHealerQueryOpcode(Player *pl, const uint64& guid)
{
    WorldPacket data(SMSG_AREA_SPIRIT_HEALER_TIME, 8+4);
    uint32 time_ = 30000 - GetLastResurrectTime();      // resurrect every 30 seconds
    if (time_ == uint32(-1))
        time_ = 0;
    data << guid << time_;
    pl->SendDirectMessage(&data);
}

void OutdoorPvPWG::AddPlayerToResurrectQueue(uint64 npc_guid, uint64 player_guid)
{
   m_ReviveQueue[npc_guid].push_back(player_guid);

    Player *plr = sObjectMgr.GetPlayer(player_guid);
    if (!plr)
        return;

    plr->CastSpell(plr, 2584, true);
}

void OutdoorPvPWG::RemovePlayerFromResurrectQueue(uint64 player_guid)
{
    for (std::map<uint64, std::vector<uint64> >::iterator itr = m_ReviveQueue.begin(); itr != m_ReviveQueue.end(); ++itr)
    {
        for (std::vector<uint64>::iterator itr2 =(itr->second).begin(); itr2 != (itr->second).end(); ++itr2)
        {
            if (*itr2 == player_guid)
            {
                (itr->second).erase(itr2);

                Player *plr = sObjectMgr.GetPlayer(player_guid);
                if (!plr)
                    return;

                plr->RemoveAurasDueToSpell(2584);

                return;
            }
        }
    }
}

void OutdoorPvPWG::RelocateDeadPlayers(Creature *cr)
{
    if (!cr || GetCreatureType(cr->GetEntry()) != CREATURE_SPIRIT_GUIDE)
        return;

    // Those who are waiting to resurrect at this node are taken to the closest own node's graveyard
    std::vector<uint64> ghost_list = m_ReviveQueue[cr->GetGUID()];
    if (!ghost_list.empty())
    {
        WorldSafeLocsEntry const *ClosestGrave = NULL;
        for (std::vector<uint64>::const_iterator itr = ghost_list.begin(); itr != ghost_list.end(); ++itr)
        {
            Player* plr = sObjectMgr.GetPlayer(*itr);
            if (!plr)
                continue;

            if (!ClosestGrave)
                ClosestGrave = sObjectMgr.GetClosestGraveYard(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), plr->GetMapId(), plr->GetTeam());

            if (ClosestGrave)
                plr->TeleportTo(plr->GetMapId(), ClosestGrave->x, ClosestGrave->y, ClosestGrave->z, plr->GetOrientation());
        }
    }
}

/*######
##OPvPCapturePointWG
######*/

OPvPCapturePointWG::OPvPCapturePointWG(OutdoorPvPWG *opvp, BuildingState *state)
: OPvPCapturePoint(opvp), m_buildingState(state), m_wintergrasp(opvp)
, m_engineer(NULL), m_engGuid(0), m_spiritguide(NULL), m_spiGuid(0)
{
}

void OPvPCapturePointWG::SetTeamByBuildingState()
{
    if (m_buildingState->GetTeam() == BG_TEAM_ALLIANCE)
    {
        m_value = m_maxValue;
        m_State = OBJECTIVESTATE_ALLIANCE;
    }
    else if (m_buildingState->GetTeam() == BG_TEAM_HORDE)
    {
        m_value = -m_maxValue;
        m_State = OBJECTIVESTATE_HORDE;
    }
    else
    {
        m_value = 0;
        m_State = OBJECTIVESTATE_NEUTRAL;
    }

    if (m_team != m_buildingState->GetTeam())
    {
        BattleGroundTeamId oldTeam = m_team;
        m_team = m_buildingState->GetTeam();
        ChangeTeam(oldTeam);
    }

    SendChangePhase();
}

void OPvPCapturePointWG::ChangeTeam(BattleGroundTeamId oldTeam)
{
    uint32 entry = 0;
    uint32 guide_entry = 0;

    if (oldTeam != BG_TEAM_NEUTRAL)
        m_wintergrasp->ModifyWorkshopCount(oldTeam, false);

    if (m_team != BG_TEAM_NEUTRAL)
    {
        entry = m_team == BG_TEAM_ALLIANCE ? CRE_ENG_A : CRE_ENG_H;
        guide_entry = m_team == BG_TEAM_ALLIANCE ? CRE_SPI_A : CRE_SPI_H;
        m_wintergrasp->ModifyWorkshopCount(m_team, true);
    }

    if (m_capturePoint)
		m_capturePoint->SetGoArtKit(CapturePointArtKit[m_team]);

    m_buildingState->SetTeam(m_team);
    // TODO: this may be sent twice
    m_wintergrasp->BroadcastStateChange(m_buildingState);

    if (m_buildingState->building)
        m_buildingState->building->SetUInt32Value(GAMEOBJECT_FACTION, WintergraspFaction[m_team]);

    if (entry)
    {
        if (m_engGuid)
        {
			UpdateEngineerData();
            _RespawnCreatureIfNeeded(m_engineer, entry);
        }
        if (m_spiGuid)
        {
            _RespawnCreatureIfNeeded(m_spiritguide, guide_entry);
            m_wintergrasp->RelocateDeadPlayers(m_spiritguide);
        }
    }
    else if (m_engineer)
        m_engineer->SetVisibility(VISIBILITY_OFF);

    sLog.outDebug("Wintergrasp workshop now belongs to %u.", (uint32)m_buildingState->GetTeam());
}

void OPvPCapturePointWG::UpdateEngineerData()
{
	if(m_engineer)
	{
		uint32 entry = m_team == BG_TEAM_ALLIANCE ? CRE_ENG_A : CRE_ENG_H;
		m_engineer->UpdateEntry(entry,m_team == BG_TEAM_ALLIANCE ? ALLIANCE : HORDE);
		m_engineer->setFaction(WintergraspFaction[m_team]);
	}
}