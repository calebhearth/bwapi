#include "TerranTest.h"
#include "TrainTest.h"
#include "BuildTest.h"
#include "ResearchTest.h"
#include "UpgradeTest.h"
#include "UseTechTest.h"
using namespace std;
using namespace BWAPI;
void TerranTest::onStart()
{
  BWAssert(Broodwar->isMultiplayer()==false);
  BWAssert(Broodwar->isReplay()==false);
  Broodwar->enableFlag(Flag::UserInput);
  Broodwar->sendText("show me the money");
  Broodwar->sendText("show me the money");

  Broodwar->setLocalSpeed(0);
  /*
  this->addTestCase(new TrainTest(UnitTypes::Terran_SCV));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Barracks));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Academy));
  this->addTestCase(new ResearchTest(TechTypes::Stim_Packs));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Marine));
  this->addTestCase(new UseTechTest(TechTypes::Stim_Packs));
  return;
  */


  this->addTestCase(new TrainTest(UnitTypes::Terran_SCV));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Command_Center));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Supply_Depot));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Supply_Depot));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Refinery));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Barracks));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Engineering_Bay));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Missile_Turret));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Academy));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Bunker));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Factory));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Starport));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Science_Facility));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Science_Facility));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Armory));

  this->addTestCase(new BuildTest(UnitTypes::Terran_Comsat_Station));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Machine_Shop));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Control_Tower));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Physics_Lab));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Covert_Ops));
  this->addTestCase(new BuildTest(UnitTypes::Terran_Nuclear_Silo));

  this->addTestCase(new TrainTest(UnitTypes::Terran_Marine));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Firebat));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Ghost));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Medic));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Vulture));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Siege_Tank_Tank_Mode));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Goliath));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Wraith));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Dropship));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Science_Vessel));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Battlecruiser));
  this->addTestCase(new TrainTest(UnitTypes::Terran_Valkyrie));

  this->addTestCase(new TrainTest(UnitTypes::Terran_Nuclear_Missile));

  this->addTestCase(new UpgradeTest(UpgradeTypes::U_238_Shells));
  this->addTestCase(new ResearchTest(TechTypes::Stim_Packs));
  this->addTestCase(new ResearchTest(TechTypes::Restoration));
  this->addTestCase(new ResearchTest(TechTypes::Optical_Flare));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Caduceus_Reactor));

  this->addTestCase(new UpgradeTest(UpgradeTypes::Ion_Thrusters));
  this->addTestCase(new ResearchTest(TechTypes::Spider_Mines));
  this->addTestCase(new ResearchTest(TechTypes::Tank_Siege_Mode));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Charon_Boosters));

  this->addTestCase(new ResearchTest(TechTypes::Cloaking_Field));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Apollo_Reactor));

  this->addTestCase(new ResearchTest(TechTypes::EMP_Shockwave));
  this->addTestCase(new ResearchTest(TechTypes::Irradiate));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Titan_Reactor));

  this->addTestCase(new ResearchTest(TechTypes::Lockdown));
  this->addTestCase(new ResearchTest(TechTypes::Personnel_Cloaking));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Ocular_Implants));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Moebius_Reactor));
  this->addTestCase(new ResearchTest(TechTypes::Yamato_Gun));
  this->addTestCase(new UpgradeTest(UpgradeTypes::Colossus_Reactor));

  for(int l=0;l<3;l++)
  {
    this->addTestCase(new UpgradeTest(UpgradeTypes::Terran_Infantry_Weapons));
    this->addTestCase(new UpgradeTest(UpgradeTypes::Terran_Infantry_Armor));
    this->addTestCase(new UpgradeTest(UpgradeTypes::Terran_Vehicle_Weapons));
    this->addTestCase(new UpgradeTest(UpgradeTypes::Terran_Ship_Weapons));
    this->addTestCase(new UpgradeTest(UpgradeTypes::Terran_Vehicle_Plating));
    this->addTestCase(new UpgradeTest(UpgradeTypes::Terran_Ship_Plating));
  }
}