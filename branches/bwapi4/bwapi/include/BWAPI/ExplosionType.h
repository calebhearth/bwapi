#pragma once
#include "Type.h"

#define BWAPI_EXPLOTYPEDEF(x) static const ExplosionType x(Enum::x) /** ref x */

namespace BWAPI
{
  namespace ExplosionTypes
  {
    namespace Enum
    {
      enum Enum
      {
        None = 0,
        Normal,
        Radial_Splash,
        Enemy_Splash,
        Lockdown,
        Nuclear_Missile,
        Parasite,
        Broodlings,
        EMP_Shockwave,
        Irradiate,
        Ensnare,
        Plague,
        Stasis_Field,
        Dark_Swarm,
        Consume,
        Yamato_Gun,
        Restoration,
        Disruption_Web,
        Corrosive_Acid,
        Mind_Control,
        Feedback,
        Optical_Flare,
        Maelstrom,
        Unused,
        Air_Splash,
        Unknown,
        MAX
      };
    };
  };
  class ExplosionType : public Type<ExplosionType, ExplosionTypes::Enum::Unknown>
  {
    public:
      ExplosionType(int id = ExplosionTypes::Enum::None);

      /** Returns the name of this explosion type. */
      const std::string &getName() const;
      const char *c_str() const;
  };
  namespace ExplosionTypes
  {
    /** Given a name of an explosion type, this will return the corresponding ExplosionType  object. */
    ExplosionType getExplosionType(std::string name);

    /** Returns the set of all ExplosionTypes. */
    const ExplosionType::set& allExplosionTypes();

    void init();

    BWAPI_EXPLOTYPEDEF(None);
    BWAPI_EXPLOTYPEDEF(Normal);
    BWAPI_EXPLOTYPEDEF(Radial_Splash);
    BWAPI_EXPLOTYPEDEF(Enemy_Splash);
    BWAPI_EXPLOTYPEDEF(Lockdown);
    BWAPI_EXPLOTYPEDEF(Nuclear_Missile);
    BWAPI_EXPLOTYPEDEF(Parasite);
    BWAPI_EXPLOTYPEDEF(Broodlings);
    BWAPI_EXPLOTYPEDEF(EMP_Shockwave);
    BWAPI_EXPLOTYPEDEF(Irradiate);
    BWAPI_EXPLOTYPEDEF(Ensnare);
    BWAPI_EXPLOTYPEDEF(Plague);
    BWAPI_EXPLOTYPEDEF(Stasis_Field);
    BWAPI_EXPLOTYPEDEF(Dark_Swarm);
    BWAPI_EXPLOTYPEDEF(Consume);
    BWAPI_EXPLOTYPEDEF(Yamato_Gun);
    BWAPI_EXPLOTYPEDEF(Restoration);
    BWAPI_EXPLOTYPEDEF(Disruption_Web);
    BWAPI_EXPLOTYPEDEF(Corrosive_Acid);
    BWAPI_EXPLOTYPEDEF(Mind_Control);
    BWAPI_EXPLOTYPEDEF(Feedback);
    BWAPI_EXPLOTYPEDEF(Optical_Flare);
    BWAPI_EXPLOTYPEDEF(Maelstrom);
    BWAPI_EXPLOTYPEDEF(Air_Splash);
    BWAPI_EXPLOTYPEDEF(Unknown);
  }
}
