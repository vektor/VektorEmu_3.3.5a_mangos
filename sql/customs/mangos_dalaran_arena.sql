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

SET @GAMEOBJECT := 540000;

UPDATE gameobject_template SET faction=114, flags=32, size=1.5 WHERE entry IN (192642, 192643);

DELETE FROM `gameobject` WHERE map=617;
DELETE FROM `gameobject` WHERE `guid` BETWEEN @GAMEOBJECT AND @GAMEOBJECT+999;

INSERT INTO gameobject (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state) VALUES
    (@GAMEOBJECT,184663,617,1,1,1291.7,813.424,7.11472,4.64562,0,0,0.730314,-0.683111,86400,100,1),
    (@GAMEOBJECT+1,184664,617,1,1,1291.7,768.911,7.11472,1.55194,0,0,0.700409,0.713742,86400,100,1),
    (@GAMEOBJECT+2,192642,617,1,1,1350.95,817.2,21.4096,3.15,0,0,0.99627,0.0862864,86400,100,1),
    (@GAMEOBJECT+3,192643,617,1,1,1232.65,764.913,21.4729,6.3,0,0,0.0310211,-0.999519,86400,100,1),
    (@GAMEOBJECT+4,194395,617,1,1,1291.6,791.05,7.11,3.22012,0,0,0.999229,-0.0392542,86400,100,1),
    (@GAMEOBJECT+5,191877,617,1,1,1291.6,791.05,7.11,3.22012,0,0,0.999229,-0.0392542,86400,100,1);

DELETE FROM gameobject_battleground WHERE guid IN (SELECT guid FROM gameobject WHERE map=617);
INSERT INTO gameobject_battleground (guid, event1, event2) VALUES
    (@GAMEOBJECT,253,0),
    (@GAMEOBJECT+1,253,0),
    (@GAMEOBJECT+2,254,0),
    (@GAMEOBJECT+3,254,0),
    (@GAMEOBJECT+4,250,0),
    (@GAMEOBJECT+5,250,0);

DELETE FROM battleground_events WHERE map=617;
INSERT INTO battleground_events (map, event1, event2, description) VALUES
    (617,253,0,'buffs'),
    (617,254,0,'doors'),
    (617,250,0,'waterfall');

UPDATE gameobject SET rotation0=0, rotation1=0, rotation2=SIN(orientation*0.5), rotation3=COS(orientation*0.5) WHERE map =  617;
