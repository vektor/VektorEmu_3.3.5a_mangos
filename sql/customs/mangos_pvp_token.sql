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
 
/* PvP Token System /dev/PixelPirate for MaNGOSRev2 & VektorEmu Project */

DELETE FROM `mangos_string` WHERE `entry` = 1605;
INSERT INTO `mangos_string` VALUES ('1605', 'You have been awarded a token for slaying another player.', null, null, null, null, null, null, null, null);

DELETE FROM `mangos_string` WHERE `entry` = 1606;
INSERT INTO `mangos_string` VALUES ('1606', 'You dont have any space in your bags for a token.', null, null, null, null, null, null, null, null);