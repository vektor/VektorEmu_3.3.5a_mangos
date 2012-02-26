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
 
/* Spell Proc Event Support */

DELETE FROM spell_chain WHERE spell_ID IN (57529, 57531);
INSERT INTO spell_chain (spell_id, prev_spell, first_spell, rank, req_spell) VALUES
(57529, 0, 57529, 1, 0),
(57531,57529, 57529, 2,0);

DELETE FROM spell_proc_event WHERE entry IN (57529, 57531);
INSERT INTO spell_proc_event VALUES (57529, 0x00, 3, 0x61400035 | 0x200000 | 0x20000 | 0x0000040 | 0x000080 | 0x00000002 | 0x00000200 | 0x00001000 | 0x00800000, 0, 0, 0x00001000 | 0x00008000 | 0x00000002 | 0x00001000 | 0x00000040, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0, 0, 0);
DELETE FROM spell_proc_event WHERE entry IN (44543, 44545);
DELETE FROM `spell_proc_event` WHERE `entry` IN (74396);
INSERT INTO `spell_proc_event` VALUES (74396, 0x00, 0x03, 685904631, 685904631, 685904631, 1151048, 1151048, 1151048, 0, 0, 0, 65536, 0, 0, 0, 0);
DELETE FROM spell_proc_event WHERE entry = 51682; 
INSERT INTO spell_proc_event VALUES (51682, 0x00, 8, 0x10014000, 0x10014000, 0x10014000, 0x00080000, 0x00080000, 0x00080000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0, 0, 0);
DELETE FROM spell_proc_event WHERE entry IN (26888, 32612);
DELETE FROM spell_proc_event WHERE entry = 56636;
INSERT INTO spell_proc_event VALUES (56636, 0x00, 0x04, 0x00000020, 0x00000020, 0x00000020, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00100000, 0, 0, 6);
DELETE FROM spell_proc_event WHERE entry = 588;
INSERT INTO spell_proc_event VALUES (588, 0x00, 0x06, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00100000, 0, 0, 0);
DELETE FROM spell_proc_event WHERE entry = 37483;
INSERT INTO spell_proc_event VALUES (37483, 0x00, 0x09, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00004000, 0x00000000, 0, 0, 0);
DELETE FROM `spell_proc_event` WHERE `entry` IN (47535);
INSERT INTO `spell_proc_event` VALUES (47535, 0x02, 0x06, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x08000000, 0x00600000, 0, 0, 0);
