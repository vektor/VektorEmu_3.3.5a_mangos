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

/* Frostmourne AOE Support */

DELETE FROM spell_target_position WHERE id IN (70860, 72546, 73655);
INSERT INTO spell_target_position VALUES
    (70860, 631, 529.302, -2124.49, 1040.857, 3.1765),
    (72546, 631, 514.000, -2523.00, 1250.990, 3.1765),
    (73655, 631, 495.708, -2523.76, 1250.990, 3.1765);

DELETE FROM spell_area WHERE spell = 74276;
INSERT INTO spell_area (spell, area, autocast) VALUES (74276, 4910, 1);
