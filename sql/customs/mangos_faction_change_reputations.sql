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

SET FOREIGN_KEY_CHECKS=0;
-- ---------------
-- Table structure 
-- ---------------
DROP TABLE IF EXISTS `player_factionchange_reputations`;

CREATE TABLE `player_factionchange_reputations` (
    `race_A` int(8) NOT NULL DEFAULT '0',
    `alliance_id` int(8) NOT NULL,
    `commentA` varchar(255) DEFAULT NULL,
    `race_H` int(8) NOT NULL DEFAULT '0',
    `horde_id` int(8) NOT NULL,
    `commentH` varchar(255) DEFAULT NULL,
    PRIMARY KEY (`alliance_id`, `horde_id`)
) DEFAULT CHARSET=UTF8;


INSERT INTO `player_factionchange_reputations` VALUES ('0', '47', 'Ironforge', '0', '530', 'Darkspear Trolls');
INSERT INTO `player_factionchange_reputations` VALUES ('0', '509', 'The League of Arathor', '0', '510', 'The Defilers');
INSERT INTO `player_factionchange_reputations` VALUES ('0', '730', 'Stormpike Guard', '0', '729', 'Frostwolf Clan');
INSERT INTO `player_factionchange_reputations` VALUES ('0', '890', 'Silverwing Sentinels', '0', '889', 'Warsong Outriders');
INSERT INTO `player_factionchange_reputations` VALUES ('0', '946', 'Honor Hold', '0', '947', 'Thrallmar');
INSERT INTO `player_factionchange_reputations` VALUES ('0', '978', 'Kurenai', '0', '941', 'The Maghar');
INSERT INTO `player_factionchange_reputations` VALUES ('0', '1037', 'Alliance Vanguard', '0', '1052', 'Horde Expedition');
INSERT INTO `player_factionchange_reputations` VALUES ('0', '1050', 'Valiance Expedition', '0', '1085', 'Warsong Offensive');
INSERT INTO `player_factionchange_reputations` VALUES ('0', '1068', 'Explorers League', '0', '1064', 'The Taunka');
INSERT INTO `player_factionchange_reputations` VALUES ('0', '1126', 'The Frostborn', '0', '1067', 'The Hand of Vengeance');
INSERT INTO `player_factionchange_reputations` VALUES ('0', '54', 'Gnomeregan Exiles', '0', '81', 'Thunder Bluff');
INSERT INTO `player_factionchange_reputations` VALUES ('0', '69', 'Darnassus', '0', '68', 'Undercity');
INSERT INTO `player_factionchange_reputations` VALUES ('0', '72', 'Stormwind', '0', '76', 'Orgrimmar');
INSERT INTO `player_factionchange_reputations` VALUES ('0', '930', 'Exodar', '0', '911', 'Silvermoon City');
