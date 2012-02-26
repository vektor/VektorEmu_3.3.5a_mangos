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

/* Creature template Support */


UPDATE `creature_template` SET `spell1`='55328' WHERE `entry`='3579';
UPDATE `creature_template` SET `spell1`='55329' WHERE `entry`='3911';
UPDATE `creature_template` SET `spell1`='55330' WHERE `entry`='3912';
UPDATE `creature_template` SET `spell1`='55332' WHERE `entry`='3913';
UPDATE `creature_template` SET `spell1`='55333' WHERE `entry`='7398';
UPDATE `creature_template` SET `spell1`='55335' WHERE `entry`='7399';
UPDATE `creature_template` SET `spell1`='55278' WHERE `entry`='15478';
UPDATE `creature_template` SET `spell1`='58589' WHERE `entry`='31120';
UPDATE `creature_template` SET `spell1`='58590' WHERE `entry`='31121';
UPDATE `creature_template` SET `spell1`='58591' WHERE `entry`='31122';

UPDATE creature_template SET flags_extra = flags_extra|8192 WHERE rank = 3;
UPDATE creature_template SET flags_extra = flags_extra&~8192 WHERE entry IN (
16061, 29940,
38433, 38462,
34796, 35438, 35439, 35440,
36855, 38106, 38296, 38297
);