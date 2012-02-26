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

DELETE FROM spell_script_target WHERE spell_script_target.entry = 60430;
INSERT INTO spell_script_target VALUES
(60430, 1, 30643);

DELETE FROM spell_script_target WHERE spell_script_target.entry = 61632;
INSERT INTO spell_script_target VALUES
(61632, 1, 28860),
(61632, 1, 30452),
(61632, 1, 30451),
(61632, 1, 30449);

DELETE FROM spell_script_target WHERE spell_script_target.entry = 53110;
INSERT INTO spell_script_target VALUES
(53110, 1, 29102),
(53110, 1, 29103);

DELETE FROM spell_script_target WHERE spell_script_target.entry = 61920;
INSERT INTO spell_script_target VALUES 
(61920 ,1,32857),
(61920 ,1,32927),
(61920 ,1,32867);

DELETE FROM spell_script_target WHERE spell_script_target.entry = 54897;
INSERT INTO spell_script_target VALUES
(54897, 1, 29351),
(54897, 1, 29358);

