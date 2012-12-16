#include "PlayerImpl.h"
#include <Util/clamp.h>

#include <BWAPI/Color.h>
#include <BWAPI/WeaponType.h>

namespace BWAPI
{
  //--------------------------------------------- GET ID -----------------------------------------------------
  int PlayerImpl::getID() const
  {
    return id;
  }
  //--------------------------------------------- GET UNITS --------------------------------------------------
  const Unitset &PlayerImpl::getUnits() const
  {
    return this->units;
  }
  //--------------------------------------------- MINERALS ---------------------------------------------------
  int PlayerImpl::minerals() const
  {
    return self->minerals;
  }
  //--------------------------------------------- GAS --------------------------------------------------------
  int PlayerImpl::gas() const
  {
    return self->gas;
  }
  //--------------------------------------------- GATHERED MINERALS ------------------------------------------
  int PlayerImpl::gatheredMinerals() const
  {
    return self->gatheredMinerals;
  }
  //--------------------------------------------- GATHERED GAS -----------------------------------------------
  int PlayerImpl::gatheredGas() const
  {
    return self->gatheredGas;
  }
  //--------------------------------------------- REPAIRED MINERALS ------------------------------------------
  int PlayerImpl::repairedMinerals() const
  {
    return self->repairedMinerals;
  }
  //--------------------------------------------- REPAIRED GAS -----------------------------------------------
  int PlayerImpl::repairedGas() const
  {
    return self->repairedGas;
  }
  //--------------------------------------------- REFUNDED MINERALS ------------------------------------------
  int PlayerImpl::refundedMinerals() const
  {
    return self->refundedMinerals;
  }
  //--------------------------------------------- REFUNDED GAS -----------------------------------------------
  int PlayerImpl::refundedGas() const
  {
    return self->refundedGas;
  }
  //--------------------------------------------- SPENT MINERALS ---------------------------------------------
  int PlayerImpl::spentMinerals() const
  {
    return self->gatheredMinerals + self->refundedMinerals - self->minerals - self->repairedMinerals;
  }
  //--------------------------------------------- SPENT GAS --------------------------------------------------
  int PlayerImpl::spentGas() const
  {
    return self->gatheredGas + self->refundedGas - self->gas - self->repairedGas;
  }
  //--------------------------------------------- SUPPLY TOTAL -----------------------------------------------
  int PlayerImpl::supplyTotal(Race race) const
  {
    if ( race == Races::None )  // Get current race's supply if None is specified
      race = this->getRace();

    if ( race < 0 || race >= 3 )  // bounds checking
      return 0;
    return self->supplyTotal[race];
  }
  //--------------------------------------------- SUPPLY USED ------------------------------------------------
  int PlayerImpl::supplyUsed(Race race) const
  {
    if ( race == Races::None )  // Get current race's supply if None is specified
      race = this->getRace();

    if ( race < 0 || race >= 3 )  // bounds checking
      return 0;
    return self->supplyUsed[race];
  }
  //--------------------------------------------- ALL UNIT COUNT ---------------------------------------------
  int PlayerImpl::allUnitCount(UnitType unit) const
  {
    return unit.isValid() ? self->allUnitCount[unit] : 0;
  }
  //--------------------------------------------- VISIBLE UNIT COUNT -----------------------------------------
  int PlayerImpl::visibleUnitCount(UnitType unit) const
  {
    return unit.isValid() ? self->visibleUnitCount[unit] : 0;
  }
  //--------------------------------------------- COMPLETED UNIT COUNT ---------------------------------------
  int PlayerImpl::completedUnitCount(UnitType unit) const
  {
    return unit.isValid() ? self->completedUnitCount[unit] : 0;
  }
  //--------------------------------------------- INCOMPLETE UNIT COUNT --------------------------------------
  int PlayerImpl::incompleteUnitCount(UnitType unit) const
  {
    return unit.isValid() ? self->allUnitCount[unit] - self->completedUnitCount[unit] : 0;
  }
  //--------------------------------------------- DEAD UNIT COUNT --------------------------------------------
  int PlayerImpl::deadUnitCount(UnitType unit) const
  {
    return unit.isValid() ? self->deadUnitCount[unit] : 0;
  }
  //--------------------------------------------- KILLED UNIT COUNT ------------------------------------------
  int PlayerImpl::killedUnitCount(UnitType unit) const
  {
    return unit.isValid() ? self->killedUnitCount[unit] : 0;
  }
  //--------------------------------------------------- SCORE ------------------------------------------------
  int PlayerImpl::getUnitScore() const
  {
    return self->totalUnitScore;
  }
  int PlayerImpl::getKillScore() const
  {
    return self->totalKillScore;
  }
  int PlayerImpl::getBuildingScore() const
  {
    return self->totalBuildingScore;
  }
  int PlayerImpl::getRazingScore() const
  {
    return self->totalRazingScore;
  }
  int PlayerImpl::getCustomScore() const
  {
    return self->customScore;
  }
  //--------------------------------------------- GET UPGRADE LEVEL ------------------------------------------
  int PlayerImpl::getUpgradeLevel(UpgradeType upgrade) const
  {
    return upgrade.isValid() ? self->upgradeLevel[upgrade] : 0;
  }
  //--------------------------------------------- HAS RESEARCHED ---------------------------------------------
  bool PlayerImpl::hasResearched(TechType tech) const
  {
    return tech.isValid() ? self->hasResearched[tech] : false;
  }
  //--------------------------------------------- IS RESEARCHING ---------------------------------------------
  bool PlayerImpl::isResearching(TechType tech) const
  {
    return tech.isValid() ? self->isResearching[tech] : false;
  }
  //--------------------------------------------- IS UPGRADING -----------------------------------------------
  bool PlayerImpl::isUpgrading(UpgradeType upgrade) const
  {
    return upgrade.isValid() ? self->isUpgrading[upgrade] : false;
  }
  //--------------------------------------------- MAX ENERGY -------------------------------------------------
  int PlayerImpl::maxEnergy(UnitType unit) const
  {
    int energy = unit.maxEnergy();
    if ((unit == UnitTypes::Protoss_Arbiter       && getUpgradeLevel(UpgradeTypes::Khaydarin_Core)    > 0) ||
        (unit == UnitTypes::Protoss_Corsair       && getUpgradeLevel(UpgradeTypes::Argus_Jewel)       > 0) ||
        (unit == UnitTypes::Protoss_Dark_Archon   && getUpgradeLevel(UpgradeTypes::Argus_Talisman)    > 0) ||
        (unit == UnitTypes::Protoss_High_Templar  && getUpgradeLevel(UpgradeTypes::Khaydarin_Amulet)  > 0) ||
        (unit == UnitTypes::Terran_Ghost          && getUpgradeLevel(UpgradeTypes::Moebius_Reactor)   > 0) ||
        (unit == UnitTypes::Terran_Battlecruiser  && getUpgradeLevel(UpgradeTypes::Colossus_Reactor)  > 0) ||
        (unit == UnitTypes::Terran_Science_Vessel && getUpgradeLevel(UpgradeTypes::Titan_Reactor)     > 0) ||
        (unit == UnitTypes::Terran_Wraith         && getUpgradeLevel(UpgradeTypes::Apollo_Reactor)    > 0) ||
        (unit == UnitTypes::Terran_Medic          && getUpgradeLevel(UpgradeTypes::Caduceus_Reactor)  > 0) ||
        (unit == UnitTypes::Zerg_Defiler          && getUpgradeLevel(UpgradeTypes::Metasynaptic_Node) > 0) ||
        (unit == UnitTypes::Zerg_Queen            && getUpgradeLevel(UpgradeTypes::Gamete_Meiosis)    > 0) )
      energy += 50;
    return energy;
  }
  //--------------------------------------------- TOP SPEED --------------------------------------------------
  double PlayerImpl::topSpeed(UnitType unit) const
  {
    double speed = unit.topSpeed();
    if ((unit == UnitTypes::Terran_Vulture   && getUpgradeLevel(UpgradeTypes::Ion_Thrusters)        > 0) ||
        (unit == UnitTypes::Zerg_Overlord    && getUpgradeLevel(UpgradeTypes::Pneumatized_Carapace) > 0) ||
        (unit == UnitTypes::Zerg_Zergling    && getUpgradeLevel(UpgradeTypes::Metabolic_Boost)      > 0) ||
        (unit == UnitTypes::Zerg_Hydralisk   && getUpgradeLevel(UpgradeTypes::Muscular_Augments)    > 0) ||
        (unit == UnitTypes::Protoss_Zealot   && getUpgradeLevel(UpgradeTypes::Leg_Enhancements)     > 0) ||
        (unit == UnitTypes::Protoss_Shuttle  && getUpgradeLevel(UpgradeTypes::Gravitic_Drive)       > 0) ||
        (unit == UnitTypes::Protoss_Observer && getUpgradeLevel(UpgradeTypes::Gravitic_Boosters)    > 0) ||
        (unit == UnitTypes::Protoss_Scout    && getUpgradeLevel(UpgradeTypes::Gravitic_Thrusters)   > 0) ||
        (unit == UnitTypes::Zerg_Ultralisk   && getUpgradeLevel(UpgradeTypes::Anabolic_Synthesis)   > 0))
    {
      if ( unit == UnitTypes::Protoss_Scout )
        speed += 427/256.0;
      else
        speed = speed * 1.5;
      if ( speed < 853/256.0 )
        speed = 853/256.0;
      //acceleration *= 2;
      //turnRadius *= 2;
    }
    return speed;
  }
  //----------------------------------------------- WEAPON MAX RANGE -----------------------------------------
  int PlayerImpl::weaponMaxRange(WeaponType weapon) const
  {
    int range = weapon.maxRange();
    if ( (weapon == WeaponTypes::Gauss_Rifle   && getUpgradeLevel(UpgradeTypes::U_238_Shells)   > 0) ||
         (weapon == WeaponTypes::Needle_Spines && getUpgradeLevel(UpgradeTypes::Grooved_Spines) > 0) )
      range += 1*32;
    else if ( weapon == WeaponTypes::Phase_Disruptor       && getUpgradeLevel(UpgradeTypes::Singularity_Charge) > 0 )
      range += 2*32;
    else if ( weapon == WeaponTypes::Hellfire_Missile_Pack && getUpgradeLevel(UpgradeTypes::Charon_Boosters)    > 0 )
      range += 3*32;
    return range;
  }
  //--------------------------------------------- SIGHT RANGE ------------------------------------------------
  int PlayerImpl::sightRange(UnitType unit) const
  {
    int range = unit.sightRange();
    if ((unit == UnitTypes::Terran_Ghost     && getUpgradeLevel(UpgradeTypes::Ocular_Implants) > 0) ||
        (unit == UnitTypes::Zerg_Overlord    && getUpgradeLevel(UpgradeTypes::Antennae)        > 0) ||
        (unit == UnitTypes::Protoss_Observer && getUpgradeLevel(UpgradeTypes::Sensor_Array)    > 0) ||
        (unit == UnitTypes::Protoss_Scout    && getUpgradeLevel(UpgradeTypes::Apial_Sensors)   > 0))
      range = 11*32;
    return range;
  }
  //--------------------------------------------- GROUND WEAPON DAMAGE COOLDOWN ------------------------------
  int PlayerImpl::groundWeaponDamageCooldown(UnitType unit) const
  {
    int cooldown = unit.groundWeapon().damageCooldown();
    if (unit == UnitTypes::Zerg_Zergling && getUpgradeLevel(UpgradeTypes::Adrenal_Glands) > 0)
    {
      // Divide cooldown by 2
      cooldown /= 2;
      // Prevent cooldown from going out of bounds
      cooldown = clamp<int>(cooldown, 5, 250);
    }
    return cooldown;
  }
  //--------------------------------------------- ARMOR ------------------------------------------------------
  int PlayerImpl::armor(UnitType unit) const
  {
    int armor = unit.armor();
    armor += getUpgradeLevel(unit.armorUpgrade());
    if ( unit == UnitTypes::Zerg_Ultralisk && getUpgradeLevel(UpgradeTypes::Chitinous_Plating) > 0 )
      armor += 2;
    else if ( unit == UnitTypes::Hero_Torrasque )
      armor += 2;
    return armor;
  }
  //-------------------------------------------------- COLOUR ------------------------------------------------
  BWAPI::Color PlayerImpl::getColor() const
  {
    return BWAPI::Color(self->color);
  }
  char PlayerImpl::getTextColor() const
  {
    return (char)self->colorByte;
  }
  //------------------------------------------------- OBSERVER -----------------------------------------------
  bool PlayerImpl::isObserver() const
  {
    return !self->isParticipating;
  }
  //------------------------------------------------ AVAILABILITY --------------------------------------------
  int PlayerImpl::getMaxUpgradeLevel(UpgradeType upgrade) const
  {
    return upgrade.isValid() ? self->maxUpgradeLevel[upgrade] : 0;
  }
  bool PlayerImpl::isResearchAvailable(TechType tech) const
  {
    return tech.isValid() ? self->isResearchAvailable[tech] : false;
  }
  bool PlayerImpl::isUnitAvailable(UnitType unit) const
  {
    return unit.isValid() ? self->isUnitAvailable[unit] : false;
  }
  //--------------------------------------------- LEFT GAME --------------------------------------------------
  bool PlayerImpl::leftGame() const
  {
    return self->leftGame;
  }
}

