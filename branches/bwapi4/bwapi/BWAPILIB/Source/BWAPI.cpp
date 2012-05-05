#include <BWAPI.h>
#include "../../svnrev.h"

#include "../../Debug.h"
void BWAPI::BWAPI_init()
{
  BWAPI::UpgradeTypes::init();
  BWAPI::WeaponTypes::init();
  BWAPI::UnitTypes::init();
}
int BWAPI::BWAPI_getRevision()
{
  return SVN_REV;
}
bool BWAPI::BWAPI_isDebug()
{
  return BUILD_DEBUG == 1;
}