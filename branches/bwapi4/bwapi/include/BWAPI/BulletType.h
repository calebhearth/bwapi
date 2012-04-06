#pragma once
#include "Type.h"

#define BWAPI_BULLETDEF(x) static const BulletType x(Enum::x) /** ref x */

namespace BWAPI
{
  namespace BulletTypes
  {
    namespace Enum
    {
      enum Enum
      {
        Melee = 0,

        Fusion_Cutter_Hit = 141,
        Gauss_Rifle_Hit,
        C_10_Canister_Rifle_Hit,
        Gemini_Missiles,
        Fragmentation_Grenade,
        Longbolt_Missile,
        Unused_Lockdown,
        ATS_ATA_Laser_Battery,
        Burst_Lasers,
        Arclite_Shock_Cannon_Hit,
        EMP_Missile,
        Dual_Photon_Blasters_Hit,
        Particle_Beam_Hit,
        Anti_Matter_Missile,
        Pulse_Cannon,
        Psionic_Shockwave_Hit,
        Psionic_Storm,
        Yamato_Gun,
        Phase_Disruptor,
        STA_STS_Cannon_Overlay,
        Sunken_Colony_Tentacle,
        Venom_Unused,
        Acid_Spore,
        Plasma_Drip_Unused,
        Glave_Wurm,
        Seeker_Spores,
        Queen_Spell_Carrier,
        Plague_Cloud,
        Consume,
        Ensnare,
        Needle_Spine_Hit,
        Invisible,

        Optical_Flare_Grenade = 201,
        Halo_Rockets,
        Subterranean_Spines,
        Corrosive_Acid_Shot,
        Corrosive_Acid_Hit,
        Neutron_Flare,

        None = 209,
        Unknown,
        MAX
      };
    }
  }
  class BulletType : public Type<BulletType, BulletTypes::Enum::Unknown>
  {
    public:
      BulletType(int id = BulletTypes::Enum::None);

      /** Returns the name of this bullet type. */
      const std::string &getName() const;
      const char *c_str() const;
  };

  namespace BulletTypes
  {
    /** Given the name of an bullet type, getBulletType() will return the corresponding BulletType object. */
    BulletType getBulletType(std::string name);

    /** Returns the set of all the BulletTypes. */
    const BulletType::set& allBulletTypes();

    void init();

    BWAPI_BULLETDEF(Melee);
    BWAPI_BULLETDEF(Fusion_Cutter_Hit);
    BWAPI_BULLETDEF(Gauss_Rifle_Hit);
    BWAPI_BULLETDEF(C_10_Canister_Rifle_Hit);
    BWAPI_BULLETDEF(Gemini_Missiles);
    BWAPI_BULLETDEF(Fragmentation_Grenade);
    BWAPI_BULLETDEF(Longbolt_Missile);
    BWAPI_BULLETDEF(ATS_ATA_Laser_Battery);
    BWAPI_BULLETDEF(Burst_Lasers);
    BWAPI_BULLETDEF(Arclite_Shock_Cannon_Hit);
    BWAPI_BULLETDEF(EMP_Missile);
    BWAPI_BULLETDEF(Dual_Photon_Blasters_Hit);
    BWAPI_BULLETDEF(Particle_Beam_Hit);
    BWAPI_BULLETDEF(Anti_Matter_Missile);
    BWAPI_BULLETDEF(Pulse_Cannon);
    BWAPI_BULLETDEF(Psionic_Shockwave_Hit);
    BWAPI_BULLETDEF(Psionic_Storm);
    BWAPI_BULLETDEF(Yamato_Gun);
    BWAPI_BULLETDEF(Phase_Disruptor);
    BWAPI_BULLETDEF(STA_STS_Cannon_Overlay);
    BWAPI_BULLETDEF(Sunken_Colony_Tentacle);
    BWAPI_BULLETDEF(Acid_Spore);
    BWAPI_BULLETDEF(Glave_Wurm);
    BWAPI_BULLETDEF(Seeker_Spores);
    BWAPI_BULLETDEF(Queen_Spell_Carrier);
    BWAPI_BULLETDEF(Plague_Cloud);
    BWAPI_BULLETDEF(Consume);
    BWAPI_BULLETDEF(Ensnare);
    BWAPI_BULLETDEF(Needle_Spine_Hit);
    BWAPI_BULLETDEF(Invisible);
    BWAPI_BULLETDEF(Optical_Flare_Grenade);
    BWAPI_BULLETDEF(Halo_Rockets);
    BWAPI_BULLETDEF(Subterranean_Spines);
    BWAPI_BULLETDEF(Corrosive_Acid_Shot);
    BWAPI_BULLETDEF(Neutron_Flare);
    BWAPI_BULLETDEF(None);
    BWAPI_BULLETDEF(Unknown);
  };
}
