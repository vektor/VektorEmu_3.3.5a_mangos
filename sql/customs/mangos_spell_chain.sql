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

DELETE FROM spell_chain WHERE spell_id IN (61680, 61681, 52858);
INSERT INTO spell_chain VALUES
(61680, 0, 61680, 1, 0),
(61681, 61680, 61680, 2, 0),
(52858, 61681, 61680, 3, 0);

DELETE FROM spell_chain WHERE spell_id IN (15337, 15338);
INSERT INTO spell_chain VALUES
(15337, 0, 15337, 1, 0),
(15338, 15337, 15337, 2, 0);
