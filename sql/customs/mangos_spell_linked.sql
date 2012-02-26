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

CREATE TABLE IF NOT EXISTS `spell_linked` (
    `entry`            int(10) unsigned NOT NULL COMMENT 'Spell entry',
    `linked_entry`     int(10) unsigned NOT NULL COMMENT 'Linked spell entry',
    `type`             int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Type of link',
    `effect_mask`      int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'mask of effect (NY)',
    `comment`          varchar(255) NOT NULL DEFAULT '',
     PRIMARY KEY (`entry`,`linked_entry`,`type`)
) DEFAULT CHARSET=utf8 PACK_KEYS=0 COMMENT='Linked spells storage';

DELETE FROM `spell_linked` WHERE `entry` = 25780;
INSERT INTO `spell_linked` (`entry`, `linked_entry`, `type`, `effect_mask`, `comment`) VALUES
(25780, 57339, 1, 0, 'Paladin - threat addon after 3.0');

DELETE FROM `spell_linked` WHERE `entry` = 21178;
INSERT INTO `spell_linked` (`entry`, `linked_entry`, `type`, `effect_mask`, `comment`) VALUES
(21178, 57339, 1, 0, 'Druid Bear form - threat addon after 3.0');

DELETE FROM `spell_linked` WHERE `entry` = 7376;
INSERT INTO `spell_linked` (`entry`, `linked_entry`, `type`, `effect_mask`, `comment`) VALUES
(7376, 57339, 1, 0, 'Warrior Defensive Stance Passive - threat addon after 3.0');

DELETE FROM `spell_linked` WHERE `entry` = 64382;
INSERT INTO `spell_linked` (`entry`, `linked_entry`, `type`, `effect_mask`, `comment`) VALUES
(64382, 64380, 4, 0, 'Shattering Throw - Immunity dispell');

DELETE FROM `spell_linked` WHERE `entry` = 32375;
INSERT INTO `spell_linked` (`entry`, `linked_entry`, `type`, `effect_mask`, `comment`) VALUES
(32375, 43241, 4, 0, 'Mass Dispel - Cosmetic');
