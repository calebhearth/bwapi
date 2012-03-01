#include <string>
#include <map>
#include <BWAPI/BulletType.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  std::map<std::string, BulletType> bulletTypeMap;
  namespace BulletTypes
  {
    static const std::string bulletTypeNames[Enum::MAX] =
    {
      "Melee", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "Fusion_Cutter_Hit",
      "Gauss_Rifle_Hit", "C_10_Canister_Rifle_Hit", "Gemini_Missiles", "Fragmentation_Grenade",
      "Longbolt_Missile", "", "ATS_ATA_Laser_Battery", "Burst_Lasers", "Arclite_Shock_Cannon_Hit",
      "EMP_Missile", "Dual_Photon_Blasters_Hit", "Particle_Beam_Hit", "Anti_Matter_Missile", "Pulse_Cannon",
      "Psionic_Shockwave_Hit", "Psionic_Storm", "Yamato_Gun", "Phase_Disruptor", "STA_STS_Cannon_Overlay",
      "Sunken_Colony_Tentacle", "", "Acid_Spore", "", "Glave_Wurm", "Seeker_Spores", "Queen_Spell_Carrier",
      "Plague_Cloud", "Consume", "Ensnare", "Needle_Spine_Hit", "Invisible", "", "", "", "", "", "", "",
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "Optical_Flare_Grenade",
      "Halo_Rockets", "Subterranean_Spines", "Corrosive_Acid_Shot", "Corrosive_Acid_Hit", "Neutron_Flare", 
      "", "", "None", "Unknown"
    };
    static const BulletType::set bulletTypeSet( Melee | Fusion_Cutter_Hit | Gauss_Rifle_Hit | C_10_Canister_Rifle_Hit |
                          Gemini_Missiles | Fragmentation_Grenade | Longbolt_Missile | ATS_ATA_Laser_Battery |
                          Burst_Lasers | Arclite_Shock_Cannon_Hit | EMP_Missile | Dual_Photon_Blasters_Hit |
                          Particle_Beam_Hit | Anti_Matter_Missile | Pulse_Cannon | Psionic_Shockwave_Hit | Psionic_Storm |
                          Yamato_Gun | Phase_Disruptor | STA_STS_Cannon_Overlay | Sunken_Colony_Tentacle | Acid_Spore |
                          Glave_Wurm | Seeker_Spores | Queen_Spell_Carrier | Plague_Cloud | Consume | Ensnare |
                          Needle_Spine_Hit | Invisible | Optical_Flare_Grenade | Halo_Rockets | Subterranean_Spines |
                          Corrosive_Acid_Shot | Neutron_Flare | None | Unknown);

    void init()
    {
      foreach(BulletType i, bulletTypeSet)
      {
        std::string name = i.getName();
        fixName(&name);
        bulletTypeMap.insert(std::make_pair(name, i));
      }
    }
  }

  BulletType::BulletType(int id) : Type( id )
  {
  }
  const std::string &BulletType::getName() const
  {
    return BulletTypes::bulletTypeNames[this->getID()];
  }
  const char *BulletType::c_str() const
  {
    return BulletTypes::bulletTypeNames[this->getID()].c_str();
  }
  BulletType BulletTypes::getBulletType(std::string name)
  {
    fixName(&name);
    std::map<std::string, BulletType>::iterator i = bulletTypeMap.find(name);
    if (i == bulletTypeMap.end())
      return BulletTypes::Unknown;
    return (*i).second;
  }
  const BulletType::set& BulletTypes::allBulletTypes()
  {
    return BulletTypes::bulletTypeSet;
  }
}
