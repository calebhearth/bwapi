#pragma once

namespace BW
{
  /** Bitmap specifying certain unit states */
  namespace StatusFlags
  {
  enum Enum : u32
    {
      Completed           = 1 <<  0,
      OnGround            = 1 <<  1,
      InAir               = 1 <<  2,
      Disabled            = 1 <<  3,  /**< Protoss Unpowered */
      Burrowed            = 1 <<  4,
      InBuilding          = 1 <<  5,
      InTransport         = 1 <<  6,
      UNKNOWN1            = 1 <<  7,  /**< @todo Unknown */
      RequiresDetection   = 1 <<  8,
      Cloaked             = 1 <<  9,
      DoodadStatesThing   = 1 << 10,  /**< @todo Unknown */
      CloakingForFree     = 1 << 11,  /**< Requires no energy to cloak */
      CanNotReceiveOrders = 1 << 12,
      NoBrkCodeStart      = 1 << 13,  /**< Unbreakable code section in iscript */
      UNKNOWN2            = 1 << 14,  /**< @todo Unknown */
      CanNotAttack        = 1 << 15,  /**< @todo Unknown */
      UNKNOWN3            = 1 << 16,  /**< @todo Unknown */
      IsABuilding         = 1 << 17,
      Turret              = 1 << 18,

      IsNormal            = 1 << 20,  /**< 1 for "normal" units, 0 for hallucinated units */
      NoCollide           = 1 << 21,

      IsGathering         = 1 << 23,

      Invincible          = 1 << 26,

      SpeedUpgrade        = 1 << 28,
      CooldownUpgrade     = 1 << 29,
      IsHallucination     = 1 << 30,  /**< 1 for hallucinated units, 0 for "normal" units */
    };
  };
};

