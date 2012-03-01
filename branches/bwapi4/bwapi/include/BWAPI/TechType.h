#pragma once
#include <string>
#include <BWAPI/Race.h>
#include <BWAPI/Order.h>
#include <BWAPI/UnitType.h>
#include "Type.h"

#define BWAPI_TECHTYPEDEF(x) static const TechType x(Enum::x) /** ref x */

namespace BWAPI
{
  class WeaponType;

  namespace TechTypes
  {
    /// Enumeration of Tech Types
    namespace Enum
    {
      enum Enum
      {
        Stim_Packs = 0,
        Lockdown,
        EMP_Shockwave,
        Spider_Mines,
        Scanner_Sweep,
        Tank_Siege_Mode,
        Defensive_Matrix,
        Irradiate,
        Yamato_Gun,
        Cloaking_Field,
        Personnel_Cloaking,
        Burrowing,
        Infestation,
        Spawn_Broodlings,
        Dark_Swarm,
        Plague,
        Consume,
        Ensnare,
        Parasite,
        Psionic_Storm,
        Hallucination,
        Recall,
        Stasis_Field,
        Archon_Warp,
        Restoration,
        Disruption_Web,
        Unused_26,
        Mind_Control,
        Dark_Archon_Meld,
        Feedback,
        Optical_Flare,
        Maelstrom,
        Lurker_Aspect,
        Unused_33,
        Healing,

        None = 44,
        Nuclear_Strike,
        Unknown,
        MAX
      };
    };
  }
  class TechType : public Type<TechType, TechTypes::Enum::Unknown>
  {
    public:
      TechType(int id = TechTypes::Enum::None);

      /** Returns the name of the tech type. */
      const std::string &getName() const;
      const char *c_str() const;

      /** Returns the race that uses the TechType. For example, TechTypes::Scanner_Sweep?.getRace() will
       * return Races::Terran. */
      Race getRace() const;

      /** Returns the mineral cost of the tech type. */
      int mineralPrice() const;

      /** Returns the vespene gas price of the tech type. */
      int gasPrice() const;

      /** Returns the number of frames needed to research the tech type. */
      int researchTime() const;

      /** Returns the amount of energy used each time this tech type is used. */
      int energyUsed() const;

      /** Returns the type of unit that researches this tech type. If this tech type is available for free
       * (does not need to be researched), then this method will return UnitTypes::None. */
      UnitType whatResearches() const;

      /** Returns the corresponding weapon for this tech type, or TechTypes::None if no corresponding weapon
       * exists. For example, TechTypes::Dark_Swarm.getWeapon() will return a pointer to
       * WeaponTypes::Dark_Swarm. */
      WeaponType getWeapon() const;

      /** Returns true if this tech type must be used on another unit (i.e. Irradiate) */
      bool targetsUnit() const;

      /** Returns true if this tech type must be specified a position (i.e. Dark Swarm) */
      bool targetsPosition() const;

      /** Returns the set of units that can use this tech type. Usually this will just be a set of one unit
       * type, however in some cases, such as TechTypes::Burrowing, several unit types will be returned. */
      const UnitType::set& whatUses() const;

      /** Returns the order used to execute this tech type as an action. */
      Order getOrder() const;
  };
  namespace TechTypes
  {
    /** Given a string, this will return the tech type. */
    TechType getTechType(std::string name);

    /** Returns the set of all the TechTypes. */
    const TechType::set& allTechTypes();
    void init();

    BWAPI_TECHTYPEDEF(Stim_Packs);
    BWAPI_TECHTYPEDEF(Lockdown);
    BWAPI_TECHTYPEDEF(EMP_Shockwave);
    BWAPI_TECHTYPEDEF(Spider_Mines);
    BWAPI_TECHTYPEDEF(Scanner_Sweep);
    BWAPI_TECHTYPEDEF(Tank_Siege_Mode);
    BWAPI_TECHTYPEDEF(Defensive_Matrix);
    BWAPI_TECHTYPEDEF(Irradiate);
    BWAPI_TECHTYPEDEF(Yamato_Gun);
    BWAPI_TECHTYPEDEF(Cloaking_Field);
    BWAPI_TECHTYPEDEF(Personnel_Cloaking);
    BWAPI_TECHTYPEDEF(Burrowing);
    BWAPI_TECHTYPEDEF(Infestation);
    BWAPI_TECHTYPEDEF(Spawn_Broodlings);
    BWAPI_TECHTYPEDEF(Dark_Swarm);
    BWAPI_TECHTYPEDEF(Plague);
    BWAPI_TECHTYPEDEF(Consume);
    BWAPI_TECHTYPEDEF(Ensnare);
    BWAPI_TECHTYPEDEF(Parasite);
    BWAPI_TECHTYPEDEF(Psionic_Storm);
    BWAPI_TECHTYPEDEF(Hallucination);
    BWAPI_TECHTYPEDEF(Recall);
    BWAPI_TECHTYPEDEF(Stasis_Field);
    BWAPI_TECHTYPEDEF(Archon_Warp);
    BWAPI_TECHTYPEDEF(Restoration);
    BWAPI_TECHTYPEDEF(Disruption_Web);
    BWAPI_TECHTYPEDEF(Mind_Control);
    BWAPI_TECHTYPEDEF(Dark_Archon_Meld);
    BWAPI_TECHTYPEDEF(Feedback);
    BWAPI_TECHTYPEDEF(Optical_Flare);
    BWAPI_TECHTYPEDEF(Maelstrom);
    BWAPI_TECHTYPEDEF(Lurker_Aspect);
    BWAPI_TECHTYPEDEF(Healing);
    BWAPI_TECHTYPEDEF(None);
    BWAPI_TECHTYPEDEF(Unknown);
    BWAPI_TECHTYPEDEF(Nuclear_Strike);
  };
}
