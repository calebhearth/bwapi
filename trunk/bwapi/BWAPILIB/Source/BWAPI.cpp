#include <BWAPI.h>
void BWAPI::BWAPI_init()
{
  BWAPI::Races::init();
  BWAPI::Orders::init();
  BWAPI::TechTypes::init();
  BWAPI::UpgradeTypes::init();
}