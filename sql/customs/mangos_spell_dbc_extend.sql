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

ALTER TABLE `spell_dbc`
    ADD COLUMN `CasterAuraState` int(10) unsigned NOT NULL default '0' AFTER `RequiresSpellFocus`,
    ADD COLUMN `TargetAuraState` INT(10) NOT NULL DEFAULT '0' AFTER `CasterAuraState`,
    ADD COLUMN `ExcludeCasterAuraState` int(10) unsigned NOT NULL default '0' AFTER `TargetAuraState`,
    ADD COLUMN `ExcludeTargetAuraState` INT(10) NOT NULL DEFAULT '0' AFTER `ExcludeCasterAuraState`,
    ADD COLUMN `PowerType` INT(10) NOT NULL DEFAULT '0' AFTER `DurationIndex`,
    ADD COLUMN `ManaCost` INT(10) NOT NULL DEFAULT '0' AFTER `PowerType`,
    ADD COLUMN `ManaCostPerLevel` INT(10) NOT NULL DEFAULT '0' AFTER `ManaCost`,
    ADD COLUMN `ManaPerSecond` INT(10) NOT NULL DEFAULT '0' AFTER `ManaCostPerLevel`,
    ADD COLUMN `ManaPerSecondPerLevel` INT(10) NOT NULL DEFAULT '0' AFTER `ManaPerSecond`,
    ADD COLUMN `Speed` INT(10) NOT NULL DEFAULT '0' AFTER `RangeIndex`,
    ADD COLUMN `ManaCostPct` INT(10) NOT NULL DEFAULT '0' AFTER `SpellIconID`,
    ADD COLUMN `StartRecoveryCategory` INT(10) NOT NULL DEFAULT '0' AFTER `ManaCostPct`,
    ADD COLUMN `StartRecoveryTime` INT(10) NOT NULL DEFAULT '0' AFTER `StartRecoveryCategory`,
    ADD COLUMN `RuneCostID` INT(10) NOT NULL DEFAULT '0' AFTER `SchoolMask`;

ALTER TABLE `spell_dbc`
    ADD COLUMN `Category` int(10) unsigned NOT NULL default '0' AFTER `Id`,
    ADD COLUMN `AttributesEx6` int(10) unsigned NOT NULL DEFAULT '0' AFTER `AttributesEx5`,
    ADD COLUMN `AttributesEx7` int(10) unsigned NOT NULL DEFAULT '0' AFTER `AttributesEx6`,
    ADD COLUMN `CasterAuraSpell` int(10) unsigned NOT NULL DEFAULT '0' AFTER `Targets`,
    ADD COLUMN `TargetAuraSpell` int(10) unsigned NOT NULL DEFAULT '0' AFTER `CasterAuraSpell`,
    ADD COLUMN `ExcludeCasterAuraSpell` int(10) unsigned NOT NULL DEFAULT '0' AFTER `TargetAuraSpell`,
    ADD COLUMN `ExcludeTargetAuraSpell` int(10) unsigned NOT NULL DEFAULT '0' AFTER `ExcludeCasterAuraSpell`,
    ADD COLUMN `SpellIconID` int(10) unsigned NOT NULL DEFAULT '0' AFTER `EffectSpellClassMaskC3`;

ALTER TABLE `spell_dbc`
    ADD COLUMN `RecoveryTime` int(10) unsigned NOT NULL default '0' AFTER `CastingTimeIndex`,
    ADD COLUMN `CategoryRecoveryTime` int(10) unsigned NOT NULL DEFAULT '0' AFTER `RecoveryTime`,
    ADD COLUMN `InterruptFlags` int(10) unsigned NOT NULL DEFAULT '0' AFTER `CategoryRecoveryTime`;
	
ALTER TABLE `spell_dbc`
    ADD COLUMN `RequiresSpellFocus` int(10) unsigned NOT NULL default '0' AFTER `Targets`,
    ADD COLUMN `EffectItemType1` INT(11) NOT NULL DEFAULT '0' AFTER `EffectMultipleValue3`,
    ADD COLUMN `EffectItemType2` int(11) unsigned NOT NULL DEFAULT '0' AFTER `EffectItemType1`;




ALTER TABLE `spell_dbc`
    CHANGE `Stances` `Stances` int(10) unsigned NOT NULL default '0';


