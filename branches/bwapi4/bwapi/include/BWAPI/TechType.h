#pragma once
#include <BWAPI/Type.h>
#include <BWAPI/UnitType.h>

namespace BWAPI
{
  class WeaponType;
  class Order;
  class Race;

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
      int energyCost() const;

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
      const UnitType::const_set& whatUses() const;

      /** Returns the order used to execute this tech type as an action. */
      Order getOrder() const;
  };
  /// Namespace containing tech types
  namespace TechTypes
  {
    /** Returns the set of all the TechTypes. */
    const TechType::const_set& allTechTypes();

#ifdef BWAPI_DECL
#undef BWAPI_DECL
#endif
#define BWAPI_DECL(x) /** x */ extern const TechType x
    BWAPI_DECL(Stim_Packs);
    BWAPI_DECL(Lockdown);
    BWAPI_DECL(EMP_Shockwave);
    BWAPI_DECL(Spider_Mines);
    BWAPI_DECL(Scanner_Sweep);
    BWAPI_DECL(Tank_Siege_Mode);
    BWAPI_DECL(Defensive_Matrix);
    BWAPI_DECL(Irradiate);
    BWAPI_DECL(Yamato_Gun);
    BWAPI_DECL(Cloaking_Field);
    BWAPI_DECL(Personnel_Cloaking);
    BWAPI_DECL(Burrowing);
    BWAPI_DECL(Infestation);
    BWAPI_DECL(Spawn_Broodlings);
    BWAPI_DECL(Dark_Swarm);
    BWAPI_DECL(Plague);
    BWAPI_DECL(Consume);
    BWAPI_DECL(Ensnare);
    BWAPI_DECL(Parasite);
    BWAPI_DECL(Psionic_Storm);
    BWAPI_DECL(Hallucination);
    BWAPI_DECL(Recall);
    BWAPI_DECL(Stasis_Field);
    BWAPI_DECL(Archon_Warp);
    BWAPI_DECL(Restoration);
    BWAPI_DECL(Disruption_Web);
    BWAPI_DECL(Mind_Control);
    BWAPI_DECL(Dark_Archon_Meld);
    BWAPI_DECL(Feedback);
    BWAPI_DECL(Optical_Flare);
    BWAPI_DECL(Maelstrom);
    BWAPI_DECL(Lurker_Aspect);
    BWAPI_DECL(Healing);
    BWAPI_DECL(None);
    BWAPI_DECL(Nuclear_Strike);
    BWAPI_DECL(Unknown);
#undef BWAPI_DECL
  };
}
