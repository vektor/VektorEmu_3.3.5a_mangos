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

UPDATE battleground_template SET AllianceStartO = 3.6, HordeStartO = 1.24 WHERE id = 1;
UPDATE battleground_template SET HordeStartO = 0 WHERE id = 2;
UPDATE battleground_template SET AllianceStartO = 5.21, HordeStartO = 2.06 WHERE id = 4;
UPDATE battleground_template SET AllianceStartO = 4, HordeStartO = 0.903205 WHERE id = 5;
UPDATE battleground_template SET AllianceStartO = 4.83, HordeStartO = 1.7 WHERE id = 8;
UPDATE battleground_template SET HordeStartO = 3.14 WHERE id = 10;
UPDATE battleground_template SET AllianceStartO = 4.72, HordeStartO = 1.6 WHERE id = 11;
UPDATE battleground_template SET AllianceStartO = 0, HordeStartO = 3.14159 WHERE id = 30;
