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

/* Pet scaling data table from rsa */

DELETE FROM `pet_scaling_data` WHERE `creature_entry` = 0;
INSERT INTO `pet_scaling_data` (`creature_entry`, `aura`, `healthbase`, `health`, `powerbase`, `power`, `str`, `agi`, `sta`, `inte`, `spi`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `apbase`, `apbasescale`, `attackpower`, `damage`, `spelldamage`, `spellhit`, `hit`, `expertize`, `attackspeed`, `crit`, `regen`) VALUES
(0, 0, 0, 1000, 0, 1500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 200, 0, 0, 0, 100, 100, 100, 100, 0, 0);

DELETE FROM `pet_scaling_data` WHERE `creature_entry` = 1;
INSERT INTO `pet_scaling_data` (`creature_entry`, `aura`, `healthbase`, `health`, `powerbase`, `power`, `str`, `agi`, `sta`, `inte`, `spi`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `apbase`, `apbasescale`, `attackpower`, `damage`, `spelldamage`, `spellhit`, `hit`, `expertize`, `attackspeed`, `crit`, `regen`) VALUES
(1,     0, 0, 1050, 0, 1500, 0, 0, 45, 0, 0, 35, 40, 40, 40, 40, 40, 40, 20, 200, 22, 0, 13, 100, 100, 100, 100, 0, 0),
(1, 62758, 0,    0, 0,    0, 0, 0,  9, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,   0,  6, 0,  3,   0,   0,   0,   0, 0, 0),
(1, 62762, 0,    0, 0,    0, 0, 0, 18, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,   0, 12, 0,  6,   0,   0,   0,   0, 0, 0);

DELETE FROM `pet_scaling_data` WHERE `creature_entry` = 29264;
INSERT INTO `pet_scaling_data` (`creature_entry`, `aura`, `healthbase`, `health`, `powerbase`, `power`, `str`, `agi`, `sta`, `inte`, `spi`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `apbase`, `apbasescale`, `attackpower`, `damage`, `spelldamage`, `spellhit`, `hit`, `expertize`, `attackspeed`, `crit`, `regen`) VALUES
(29264,     0, 0, 1000, 0, 1500, 0, 0, 30, 0, 0, 35, 40, 40, 40, 40, 40, 40, 20, 200, 22, 0, 13, 100, 100, 100, 100, 0, 0),
(29264, 63271, 0,    0, 0,    0, 0, 0,  0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,   0, 30, 0,  0,   0,   0,   0,   0, 0, 0);

DELETE FROM `pet_scaling_data` WHERE `creature_entry` = 26125;
INSERT INTO `pet_scaling_data` (`creature_entry`, `aura`, `healthbase`, `health`, `powerbase`, `power`, `str`, `agi`, `sta`, `inte`, `spi`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `apbase`, `apbasescale`, `attackpower`, `damage`, `spelldamage`, `spellhit`, `hit`, `expertize`, `attackspeed`, `crit`, `regen`) VALUES
(26125,     0, 0, 1000, 0, 0, 70, 0, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 100, 0, 83, 0, 0, 100, 100, 100, 0, 0),
(26125, 48965, 0,    0, 0, 0, 14, 0,  6, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,   0, 0,  0, 0, 0,   0,   0,   0, 0, 0),
(26125, 49571, 0,    0, 0, 0, 28, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,   0, 0,  0, 0, 0,   0,   0,   0, 0, 0),
(26125, 49572, 0,    0, 0, 0, 42, 0, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,   0, 0,  0, 0, 0,   0,   0,   0, 0, 0),
(26125, 58686, 0,    0, 0, 0, 40, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,   0, 0,  0, 0, 0,   0,   0,   0, 0, 0);

DELETE FROM `pet_scaling_data` WHERE `creature_entry` = 416;
INSERT INTO `pet_scaling_data` (`creature_entry`, `aura`, `healthbase`, `health`, `powerbase`, `power`, `str`, `agi`, `sta`, `inte`, `spi`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `apbase`, `apbasescale`, `attackpower`, `damage`, `spelldamage`, `spellhit`, `hit`, `expertize`, `attackspeed`, `crit`, `regen`) VALUES
(416, 0, 0, 840, 0, 495, 0, 0, 75, 30, 0, 35, 40, 40, 40, 40, 40, 40, 10, 100, 57, 0, 15, 100, 100, 100, 100, 0, 20);

DELETE FROM `pet_scaling_data` WHERE `creature_entry` = 417;
INSERT INTO `pet_scaling_data` (`creature_entry`, `aura`, `healthbase`, `health`, `powerbase`, `power`, `str`, `agi`, `sta`, `inte`, `spi`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `apbase`, `apbasescale`, `attackpower`, `damage`, `spelldamage`, `spellhit`, `hit`, `expertize`, `attackspeed`, `crit`, `regen`) VALUES
(417, 0, 0, 950, 0, 1150, 0, 0, 75, 30, 0, 35, 40, 40, 40, 40, 40, 40, 20, 200, 57, 0, 15, 100, 100, 100, 100, 0, 20);

DELETE FROM `pet_scaling_data` WHERE `creature_entry` = 1860;
INSERT INTO `pet_scaling_data` (`creature_entry`, `aura`, `healthbase`, `health`, `powerbase`, `power`, `str`, `agi`, `sta`, `inte`, `spi`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `apbase`, `apbasescale`, `attackpower`, `damage`, `spelldamage`, `spellhit`, `hit`, `expertize`, `attackspeed`, `crit`, `regen`) VALUES
(1860,     0, 0, 1100, 0, 1150, 0, 0, 75, 30, 0, 35, 40, 40, 40, 40, 40, 40, 20, 200, 57, 0, 15, 100, 100, 100, 100, 0, 20),
(1860, 57277, 0,    0, 0,    0, 0, 0, 20,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,   0,  0, 0,  0,   0,   0,   0,   0, 0,  0);

DELETE FROM `pet_scaling_data` WHERE `creature_entry` = 1863;
INSERT INTO `pet_scaling_data` (`creature_entry`, `aura`, `healthbase`, `health`, `powerbase`, `power`, `str`, `agi`, `sta`, `inte`, `spi`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `apbase`, `apbasescale`, `attackpower`, `damage`, `spelldamage`, `spellhit`, `hit`, `expertize`, `attackspeed`, `crit`, `regen`) VALUES
(1863, 0, 0, 910, 0, 1150, 0, 0, 75, 30, 0, 35, 40, 40, 40, 40, 40, 40, 20, 200, 57, 0, 15, 100, 100, 100, 100, 0, 20);

DELETE FROM `pet_scaling_data` WHERE `creature_entry` = 17252;
INSERT INTO `pet_scaling_data` (`creature_entry`, `aura`, `healthbase`, `health`, `powerbase`, `power`, `str`, `agi`, `sta`, `inte`, `spi`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `apbase`, `apbasescale`, `attackpower`, `damage`, `spelldamage`, `spellhit`, `hit`, `expertize`, `attackspeed`, `crit`, `regen`) VALUES
(17252,     0, 0, 1100, 0, 1150, 0, 0, 75, 30, 0, 35, 40, 40, 40, 40, 40, 40, 20, 200, 57, 0, 15, 100, 100, 100, 100, 0, 20),
(17252, 56246, 0,    0, 0,    0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,   0, 20, 0,  0,   0,   0,   0,   0, 0,  0);

DELETE FROM `pet_scaling_data` WHERE `creature_entry` = 37994;
INSERT INTO `pet_scaling_data` (`creature_entry`, `aura`, `healthbase`, `health`, `powerbase`, `power`, `str`, `agi`, `sta`, `inte`, `spi`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `apbase`, `apbasescale`, `attackpower`, `damage`, `spelldamage`, `spellhit`, `hit`, `expertize`, `attackspeed`, `crit`, `regen`) VALUES
(37994, 0, 0, 1000, 0, 1500, 0, 0, 30, 30, 0, 35, 0, 0, 0, 0, 0, 0, 20, 200, 0, 0, 40, 100, 100, 100, 100, 0, 0);

DELETE FROM `pet_scaling_data` WHERE `creature_entry` = 19668;
INSERT INTO `pet_scaling_data` (`creature_entry`, `aura`, `healthbase`, `health`, `powerbase`, `power`, `str`, `agi`, `sta`, `inte`, `spi`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `apbase`, `apbasescale`, `attackpower`, `damage`, `spelldamage`, `spellhit`, `hit`, `expertize`, `attackspeed`, `crit`, `regen`) VALUES
(19668, 0, 0, 1000, 0, 1500, 0, 0, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 200, 400, 0, 67, 100, 100, 100, 100, 0, 0);

DELETE FROM `pet_scaling_data` WHERE `creature_entry` = 31216;
INSERT INTO `pet_scaling_data` (`creature_entry`, `aura`, `healthbase`, `health`, `powerbase`, `power`, `str`, `agi`, `sta`, `inte`, `spi`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `apbase`, `apbasescale`, `attackpower`, `damage`, `spelldamage`, `spellhit`, `hit`, `expertize`, `attackspeed`, `crit`, `regen`) VALUES
(31216, 0, 0, 1000, 0, 1500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 200, 0, 0, 33, 100, 100, 100, 100, 0, 0);

DELETE FROM `pet_scaling_data` WHERE `creature_entry` = 27829;
INSERT INTO `pet_scaling_data` (`creature_entry`, `aura`, `healthbase`, `health`, `powerbase`, `power`, `str`, `agi`, `sta`, `inte`, `spi`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `apbase`, `apbasescale`, `attackpower`, `damage`, `spelldamage`, `spellhit`, `hit`, `expertize`, `attackspeed`, `crit`, `regen`) VALUES
(27829, 0, 0, 1000, 0, 1500, 100, 0, 100, 0, 0, 35, 0, 0, 0, 0, 0, 0, 20, 200, 0, 0, 50, 100, 100, 100, 100, 0, 0);

DELETE FROM `pet_scaling_data` WHERE `creature_entry` = 15352;
INSERT INTO `pet_scaling_data` (`creature_entry`, `aura`, `healthbase`, `health`, `powerbase`, `power`, `str`, `agi`, `sta`, `inte`, `spi`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `apbase`, `apbasescale`, `attackpower`, `damage`, `spelldamage`, `spellhit`, `hit`, `expertize`, `attackspeed`, `crit`, `regen`) VALUES
(15352, 0, 0, 1000, 0, 1500, 0, 0, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 200, 40, 0, 30, 100, 100, 100, 100, 0, 0);

DELETE FROM `pet_scaling_data` WHERE `creature_entry` = 15438;
INSERT INTO `pet_scaling_data` (`creature_entry`, `aura`, `healthbase`, `health`, `powerbase`, `power`, `str`, `agi`, `sta`, `inte`, `spi`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `apbase`, `apbasescale`, `attackpower`, `damage`, `spelldamage`, `spellhit`, `hit`, `expertize`, `attackspeed`, `crit`, `regen`) VALUES
(15438, 0, 0, 1000, 0, 1500, 0, 0, 20, 10, 0, 0, 0, 0, 0, 0, 0, 0, 20, 200, 80, 0, 40, 100, 100, 100, 100, 0, 0);

DELETE FROM `pet_scaling_data` WHERE `creature_entry` = 510;
INSERT INTO `pet_scaling_data` (`creature_entry`, `aura`, `healthbase`, `health`, `powerbase`, `power`, `str`, `agi`, `sta`, `inte`, `spi`, `armor`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `apbase`, `apbasescale`, `attackpower`, `damage`, `spelldamage`, `spellhit`, `hit`, `expertize`, `attackspeed`, `crit`, `regen`) VALUES
(510, 0, 0, 1000, 0, 1500, 0, 0, 30, 30, 0, 35, 0, 0, 0, 0, 0, 0, 20, 200, 0, 0, 40, 100, 100, 100, 100, 0, 0);

