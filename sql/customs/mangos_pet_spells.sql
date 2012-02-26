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

/* Pet Spells Support */

-- UPDATE creature_template SET spell1 = 12470, spell2 = 57984 WHERE entry = 15438;
-- UPDATE creature_template SET spell1 = 36213 WHERE entry = 15352;
UPDATE `creature_template` SET `spell1`='0', `spell2`='0' WHERE `entry`='15438';
UPDATE `creature_template` SET `spell1`= '0', `spell2`='0' WHERE `entry`='15352';
UPDATE `creature_template` SET `spell1`='40133' WHERE `entry`='15439';
UPDATE `creature_template` SET `spell1`='40132' WHERE `entry`='15430';

DELETE FROM `creature_template_addon` WHERE `entry`='89';
INSERT INTO `creature_template_addon`
    (`entry`, `auras`)
VALUES
    (89, 39007);

DELETE FROM `event_scripts` WHERE `id` IN (14859,14858);
INSERT INTO `event_scripts`
    (`id`, `delay`, `command`, `datalong`, `datalong2`, `datalong3`, `datalong4`, `data_flags`, `dataint`, `dataint2`, `dataint3`, `dataint4`, `x`, `y`, `z`, `o`, `comments`)
VALUES
    (14858, 1, 15, 33663, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Summon greater Earth elemental'),
    (14859, 1, 15, 32982, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Summon greater Fire  elemental');

DELETE FROM `creature_template_addon` WHERE `entry` IN ('15438','15352');
INSERT INTO `creature_template_addon`(`entry`, `auras`)
    VALUES (15438, 7942),(15352, 7941);

UPDATE `creature_template` SET `minmana` = '6500', `maxmana` = '6500', `spell1` = '71841' WHERE `entry` =38391;
REPLACE INTO `creature_spell` (`guid`,`spell`,`index`) VALUES (38391, 71841, 0);
