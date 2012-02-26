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
 
/* Pet Spell Auras Support */

DELETE FROM `spell_pet_auras` WHERE `aura`='54566';
DELETE FROM `spell_pet_auras` WHERE `spell` IN (34455,34459,34460);
DELETE FROM `spell_pet_auras` WHERE `spell` IN (54037,54038);
DELETE FROM `spell_pet_auras` WHERE `spell`='0' AND `pet` IN (1,89,416,417,510,1860,1863,15352,15438,17252,19668,24207,26125,27829,28017,29264,31216,37994);
INSERT INTO `spell_pet_auras`
    (`spell`, `effectId`, `pet`, `aura`)
VALUES
    (0,0,1,34902),
    (0,0,1,34903),
    (0,0,1,34904),
    (0,0,1,61017),
    (0,0,89,34947),
    (0,0,89,34956),
    (0,0,89,34957),
    (0,0,89,34958),
    (0,0,89,61013),
    (0,0,416,34947),
    (0,0,416,34956),
    (0,0,416,34957),
    (0,0,416,34958),
    (0,0,416,61013),
    (0,0,417,34947),
    (0,0,417,34956),
    (0,0,417,34957),
    (0,0,417,34958),
    (0,0,417,61013),
    (54037,0,417,54037),
    (54038,0,417,54038),
    (0,0,510,34947),
    (0,0,510,34956),
    (0,0,1860,34947),
    (0,0,1860,34956),
    (0,0,1860,34957),
    (0,0,1860,34958),
    (0,0,1860,61013),
    (0,0,1863,34947),
    (0,0,1863,34956),
    (0,0,1863,34957),
    (0,0,1863,34958),
    (0,0,1863,61013),
    (0,0,15352,34947),
    (0,0,15438,34956),
    (0,0,15438,34947),
    (0,0,17252,34947),
    (0,0,17252,34956),
    (0,0,17252,34957),
    (0,0,17252,34958),
    (0,0,17252,61013),
    (0,0,19668,34947),
    (0,0,24207,51996),
    (0,0,24207,54566),
    (0,0,24207,61697),
    (0,0,24207,50142),
    (0,0,26125,51996),
    (0,0,26125,54566),
    (0,0,26125,61697),
    (0,0,26125,50142),
    (0,0,27829,54566),
    (0,0,28017,50453),
    (0,0,29264,58877),
    (0,0,29264,34902),
    (0,0,29264,34903),
    (0,0,29264,34904),
    (0,0,29264,61783),
    (0,0,31216,34947),
    (0,0,37994,34947),
    (0,0,37994,34956),
    (34455,0,0,75593),
    (34459,0,0,75446),
    (34460,0,0,75447);

