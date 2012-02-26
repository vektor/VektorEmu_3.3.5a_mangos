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
 
/* Shaman's Spell Fix Support */

DELETE FROM spell_chain WHERE first_spell = 45284;
INSERT INTO spell_chain (spell_id, prev_spell, first_spell, rank) VALUES
(45284,     0, 45284,  1),
(45286, 45284, 45284,  2),
(45287, 45286, 45284,  3),
(45288, 45287, 45284,  4),
(45289, 45288, 45284,  5),
(45290, 45289, 45284,  6),
(45291, 45290, 45284,  7),
(45292, 45291, 45284,  8),
(45293, 45292, 45284,  9),
(45294, 45293, 45284, 10),
(45295, 45294, 45284, 11),
(45296, 45295, 45284, 12),
(49239, 45296, 45284, 13),
(49240, 49239, 45284, 14);

DELETE FROM spell_bonus_data WHERE entry = 45284;
INSERT INTO spell_bonus_data (entry, direct_bonus, comments) VALUES
(45284, 0.3572, 'Shaman - Lightning Bolt Overload Proc');

DELETE FROM spell_chain WHERE first_spell = 45297;
INSERT INTO spell_chain (spell_id, prev_spell, first_spell, rank) VALUES
(45297,     0, 45297, 1),
(45298, 45297, 45297, 2),
(45299, 45298, 45297, 3),
(45300, 45299, 45297, 4),
(45301, 45300, 45297, 5),
(45302, 45301, 45297, 6),
(49268, 45302, 45297, 7),
(49269, 49268, 45297, 8);

DELETE FROM spell_bonus_data WHERE entry = 45297;
INSERT INTO spell_bonus_data (entry, direct_bonus, comments) VALUES
(45297, 0.2857, 'Shaman - Chain Lightning Overload Proc');
