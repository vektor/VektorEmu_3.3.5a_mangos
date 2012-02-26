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

/* Lightwell Support */

DELETE FROM `creature_template_addon` WHERE `entry` IN (31897, 31896, 31895, 31894, 31893, 31883);
INSERT INTO `creature_template_addon`
    (`entry`, `auras`)
VALUES 
    (31897,59907),
    (31896,59907),
    (31895,59907),
    (31894,59907),
    (31893,59907),
    (31883,59907);

UPDATE `creature_template` SET `unit_flags`='4', `flags_extra`='2' WHERE `entry` IN (31897, 31896, 31895, 31894, 31893, 31883);

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (31897, 31896, 31895, 31894, 31893, 31883);
INSERT INTO `npc_spellclick_spells`
    (`npc_entry`, `spell_id`, `quest_start`, `cast_flags`)
VALUES 
    (31897,60123,0,2),
    (31896,60123,0,2),
    (31895,60123,0,2),
    (31894,60123,0,2),
    (31893,60123,0,2),
    (31883,60123,0,2);
