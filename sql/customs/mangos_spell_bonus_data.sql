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

/* Spell bonus data Support */

UPDATE `spell_bonus_data` SET `direct_bonus`= 0.8 WHERE `entry`= 33763;

DELETE FROM `spell_bonus_data` WHERE entry IN (30451, 51963, 52042, 52752, 54158, 56161, 58621, 64844, 64085);
INSERT INTO `spell_bonus_data`
    (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`)
VALUES
    (51963, 0, 0, 0.33, 0,'Gargoyle Strike'),
    (52042, 0, 0, 0, 0,'Healing Stream Totem'),
    (52752, 0, 0, 0, 0,'Ancestral Awakening'),
    (54158, 0, 0, 0, 0,'Judgement'),
    (56161, 0, 0, 0, 0,'Glyph of Prayer of Healing'),
    (58621, 0, 0, 0.08, 0, 'Glyph of Chains of Ice'),
    (64844, 0.564, 0, 0, 0, 'Priest - Divine Hymn'),
    (64085, 2.4, 0, 0, 0, 'Priest - Vampiric Touch Dispel');

DELETE FROM `spell_bonus_data` WHERE `entry` IN (7294, 467, 2947);
INSERT INTO `spell_bonus_data`(`entry`,`direct_bonus`,`dot_bonus`,`ap_bonus`,`ap_dot_bonus`,`comments`) VALUES
    ( 467, 0.033, 0, 0, 0,'Druid - Thorns'),
    (7294, 0.033, 0, 0, 0,'Paladin - Retribution Aura'),
    (2947, 0.033, 0, 0, 0,'Warlock - Fire Shield (Imp)');

DELETE FROM `spell_bonus_data` WHERE `entry` IN (16614, 45055, 60488);
INSERT INTO `spell_bonus_data`(`entry`,`direct_bonus`,`dot_bonus`,`ap_bonus`,`ap_dot_bonus`,`comments`) VALUES
(16614, 0, 0, 0, 0, 'Item - Storm Gauntlets - Lightning Strike'),
(45055, 0, 0, 0, 0, 'Item - Timbal''s Focusing Crystal'),
(60488, 0, 0, 0, 0, 'Item - Extract of Necromatic Power');

DELETE FROM `spell_bonus_data` WHERE `entry` IN (17253,16827,49966);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
    (17253, 0, 0, 0.08, 0, 'Bite (Rank 1) - Hunter pet'),
    (16827, 0, 0, 0.08, 0, 'Claw (Rank 1) - Hunter pet'),
    (49966, 0, 0, 0.08, 0, 'Smack (Rank 1) - Hunter pet');
