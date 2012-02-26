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

/* OPTIONNAL */

DELETE FROM `areatrigger_teleport` WHERE `id` BETWEEN 100000 AND 101000;
INSERT INTO `areatrigger_teleport` (`id`, `name`, `required_level`, `required_item`, `required_item2`, `heroic_key`, `heroic_key2`, `required_quest_done_A`, `required_quest_done_H`, `required_quest_done_heroic_A`, `required_quest_done_heroic_H`, `required_failed_text`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`, `minGS`, `maxGS`, `achiev_id_0`, `achiev_id_1`, `combat_mode`) VALUES
(100013, 'Map 13', 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(100025, 'Map 25', 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(100037, 'Map 37', 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 37, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(100042, 'Map 42', 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 42, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(100169, 'Map 169', 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(100451, 'Map 451', 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 451, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(100573, 'Map 573', 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 573, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(100597, 'Map 597', 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 597, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(100605, 'Map 605', 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 605, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(100606, 'Map 606', 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 606, 0, 0, 0, 0, 0, 0, 0, 0, 0);
