/*
 * Copyright (C) 2012-2013 Vektor Project <http://vektor.fr/>
 * Copyright (C) 2005-2012 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "WorldPvPImpl.h"

#ifndef World_PVP_WG_
#define World_PVP_WG_

#define ZONE_DALARAN            4395
#define ZONE_WINTERGRASP         4197
#define POS_X_CENTER             5100
#define MAX_VEHICLE_PER_WORKSHOP    4

/*
Not Working for the moment. I need more research to a correct implementation
All participation / help will be welcome :)
*/

enum WorldPvPWGSpell
{

    SPELL_RECRUIT = 37795,
    SPELL_CORPORAL = 33280,
    SPELL_LIEUTENANT = 55629,
    SPELL_TENACITY = 58549,
    SPELL_TENACITY_VEHICLE = 59911,
    SPELL_TOWER_CONTROL = 62064,
    SPELL_SPIRITUAL_IMMUNITY = 58729,

    // Reward spells
    SPELL_VICTORY_REWARD = 56902,
    SPELL_DEFEAT_REWARD = 58494,
    SPELL_DAMAGED_TOWER = 59135,
    SPELL_DESTROYED_TOWER = 59136,
    SPELL_DAMAGED_BUILDING = 59201,
    SPELL_INTACT_BUILDING = 59203,

};

const uint16 GameEventWintergraspDefender[2] = {50, 51};

enum OutdoorPvP_WG_Sounds
{
    OutdoorPvP_WG_SOUND_KEEP_CLAIMED = 8192,
    OutdoorPvP_WG_SOUND_KEEP_CAPTURED_ALLIANCE = 8173,
    OutdoorPvP_WG_SOUND_KEEP_CAPTURED_HORDE = 8213,
    OutdoorPvP_WG_SOUND_KEEP_ASSAULTED_ALLIANCE = 8212,
    OutdoorPvP_WG_SOUND_KEEP_ASSAULTED_HORDE = 8174,
    OutdoorPvP_WG_SOUND_NEAR_VICTORY = 8456
};

enum DataId
{
    DATA_ENGINEER_DIE,
};

enum OutdoorPvP_WG_KeepStatus
{
    OutdoorPvP_WG_KEEP_TYPE_NEUTRAL = 0,
    OutdoorPvP_WG_KEEP_TYPE_CONTESTED = 1,
    OutdoorPvP_WG_KEEP_STATUS_ALLY_CONTESTED = 1,
    OutdoorPvP_WG_KEEP_STATUS_HORDE_CONTESTED = 2,
    OutdoorPvP_WG_KEEP_TYPE_OCCUPIED = 3,
    OutdoorPvP_WG_KEEP_STATUS_ALLY_OCCUPIED = 3,
    OutdoorPvP_WG_KEEP_STATUS_HORDE_OCCUPIED = 4
};


enum OutdoorPvPWGWorldStates
{
WS_WG_BATTLEPANEL = 3710,
WS_WG_VEH_ALL = 3680,
WS_WG_VEH_ALL_TOT = 3681,
WS_WG_VEH_HOR = 3490,
WS_WG_VEH_HOR_TOT = 3491,
WS_WG_CAPTUREBAR = 3466,
WS_WG_CAPTURECOUNTER = 3467,
WS_WG_CAPTUREBAR2 = 3501,
WS_WG_CONTROL_ALLIANCE = 3803,
WS_WG_CONTROL_HORDE = 3802,
WS_WG_NEXT_BATTLE = 3801,
WS_WG_NEXT_BATTLE_TIMER = 4354,
};

enum OutdoorPvPWGCreType
{
    CREATURE_OTHER,
    CREATURE_SIEGE_VEHICLE,
    CREATURE_TURRET,
    CREATURE_ENGINEER,
    CREATURE_GUARD,
    CREATURE_SPECIAL,
    CREATURE_SPIRIT_GUIDE,
    CREATURE_SPIRIT_HEALER,
    CREATURE_QUESTGIVER,
};

enum OutdoorPvPWGBuildingType
{
    BUILDING_WALL,
    BUILDING_WORKSHOP,
    BUILDING_TOWER,
};

enum OutdoorPvPWGDamageState
{ 
    DAMAGE_INTACT,
    DAMAGE_DAMAGED,
    DAMAGE_DESTROYED,
};

typedef uint32 TeamPair[2];

enum OutdoorPvPWGQuest
{
    A_VICTORY_IN_WG = 13181,
    H_VICTORY_IN_WG = 13183,
    CRE_PVP_KILL = 31086,
    CRE_PVP_KILL_V = 31093,
};

enum OutdoorPvPWGCreEntry
{
    CRE_ENG_A = 30499,
    CRE_ENG_H = 30400,
    CRE_SPI_A = 31842,
    CRE_SPI_H = 31841,
};

const TeamPair OutdoorPvPWGCreEntryPair[] =
{
    {32307, 32308}, // Guards
    {30739, 30740}, // Champions
    {32296, 32294}, // Quartermaster
    {32615, 32626}, // Warbringer & Brigadier General
    {0,0} // Do not delete Used in LoadTeamPair
};

const TeamPair OutdoorPvPWGGODisplayPair[] =
{
    {5651, 5652},
    {8256, 8257},
    {0,0} // Do not delete Used in LoadTeamPair
};

const uint32 AreaPOIIconId[3][3] = {{7,8,9},{4,5,6},{1,2,3}};

void UpdateEngineerData();

#endif
