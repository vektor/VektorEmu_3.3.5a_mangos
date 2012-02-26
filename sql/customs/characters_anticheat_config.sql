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
 
 ALTER TABLE `anticheat_config`
    ADD COLUMN `disabledzones` varchar(255) NOT NULL DEFAULT '' 
    COMMENT 'List of zones, in which check disabled.'
    AFTER `actionparam2`;

REPLACE INTO `anticheat_config`
    (`checktype`, `description`, `check_period`, `alarmscount`, `disableoperation`, `messagenum`, `intparam1`, `intparam2`, `floatparam1`, `floatparam2`, `action1`, `actionparam1`, `action2`, `actionparam2`)
VALUES
    -- Main
    (0, "Null check",         0, 1, 0, 11000, 0, 0,    0, 0, 1, 0, 0, 0),
    (1, "Movement cheat",     0, 1, 0, 11000, 0, 0,    0, 0, 2, 1, 0, 0),
    (2, "Spell cheat",        0, 1, 0, 11000, 0, 0,    0, 0, 2, 1, 0, 0),
    (3, "Quest cheat",        0, 1, 0, 11000, 0, 0,    0, 0, 2, 1, 0, 0),
    (4, "Transport cheat",    0, 3, 0, 11000, 0, 0,  300,50, 2, 1, 0, 0),
    (5, "Damage cheat",       0, 1, 0, 11000, 0, 0,    0, 0, 2, 1, 0, 0),
    (6, "Item cheat",         0, 1, 0, 11000, 0, 0,    0, 0, 2, 1, 0, 0),
    (7, "Warden check",       0, 1, 0, 11000, 0, 0,    0, 0, 2, 1, 0, 0);
	-- Sub
REPLACE INTO `anticheat_config`
    (`checktype`, `description`, `check_period`, `alarmscount`, `disableoperation`, `messagenum`, `intparam1`, `intparam2`, `floatparam1`, `floatparam2`, `action1`, `actionparam1`, `action2`, `actionparam2`)
VALUES
    (101, "Speed hack",              500, 5, 0, 11000,    10000, 0, 0.0012,    0, 2, 1, 6, 20000),
    (102, "Fly hack",                500, 5, 0, 11000,    20000, 0,   10.0,    0, 2, 1, 0, 0),
    (103, "Wall climb hack",         500, 2, 0, 11000,    10000, 0, 0.0015, 2.37, 2, 1, 0, 0),
    (104, "Waterwalking hack",      1000, 3, 0, 11000,    20000, 0,      0,    0, 2, 1, 0, 0),
    (105, "Teleport to plane hack",  500, 1, 0, 11000,        0, 0, 0.0001,  0.1, 2, 1, 0, 0),
    (106, "AirJump hack" ,           500, 3, 0, 11000,    30000, 0,   10.0, 25.0, 2, 1, 0, 0),
    (107, "Teleport hack" ,            0, 1, 0, 11000,        0, 0,   50.0,    0, 2, 1, 0, 0),
    (108, "Fall hack" ,                0, 3, 0, 11000,    10000, 0,      0,    0, 2, 1, 0, 0),
    (109, "Z Axis hack" ,              0, 1, 0, 11000,        0, 0,0.00001, 10.0, 2, 1, 0, 0),
    (201, "Spell invalid",             0, 1, 0, 11000,        0, 0,      0,    0, 2, 1, 0, 0),
    (202, "Spellcast in dead state",   0, 1, 0, 11000,        0, 0,      0,    0, 2, 1, 0, 0),
    (203, "Spell not valid for player",0, 1, 0, 11000,        0, 0,      0,    0, 2, 1, 0, 0),
    (204, "Spell not in player book",  0, 1, 0, 11000,        0, 0,      0,    0, 2, 1, 0, 0),
    (501, "Spell damage hack",         0, 1, 0, 11000,        0, 50000,  0,    0, 2, 1, 0, 0),
    (502, "Melee damage hack",         0, 1, 0, 11000,        0, 50000,  0,    0, 2, 1, 0, 0),
    (601, "Item dupe hack",            0, 1, 0, 11000,        0,     0,  0,    0, 2, 1, 0, 0),
    (701, "Warden memory check",       0, 1, 0, 11000,        0,     0,  0,    0, 2, 1, 0, 0),
    (702, "Warden key check",          0, 1, 0, 11000,        0,     0,  0,    0, 2, 1, 0, 0),
    (703, "Warden checksum check",     0, 1, 0, 11000,        0,     0,  0,    0, 2, 1, 0, 0),
    (704, "Warden timeout check",      0, 1, 0, 11000,        0,     0,  0,    0, 2, 1, 0, 0);
