#include <BWAPI/Vectorset.h>
#include <BWAPI/Unitset.h>
#include <BWAPI/UnitCommand.h>
#include <BWAPI/Position.h>
#include <BWAPI/Unit.h>
#include <BWAPI/Game.h>
#include <BWAPI/TechType.h>

namespace BWAPI
{
  ////////////////////////////////////////////////////////// ctors
  Unitset::Unitset(size_t initialSize) : Vectorset(initialSize) { };
  Unitset::Unitset(const Unitset &other) : Vectorset(other) { };

  ////////////////////////////////////////////////////////// Position
  Position Unitset::getPosition() const
  {
    // Declare the local position
    Position retPosition(0,0);
    unsigned int validPosCount = 0;

    // Add up the positions for all units in the set
    for ( Unitset::iterator i = this->begin(); i != this->end(); ++i )
    {
      Position pos(i->getPosition());
      if ( pos.isValid() )
      {
        retPosition += pos;
        ++validPosCount;
      }
    }

    // Divides the position by the size of the set and returns it
    retPosition /= validPosCount;
    return retPosition;
  }
  ////////////////////////////////////////////////////////// sets
  Unitset Unitset::getLoadedUnits() const
  {
    Unitset retSet;  // the return set
    // Iterate all units in the set
    for ( Unitset::iterator i = this->begin(); i != this->end(); ++i )
      retSet += i->getLoadedUnits();
    return retSet;
  }
  Unitset Unitset::getInterceptors() const
  {
    Unitset retSet;  // the return set
    // Iterate all units in the set
    for ( Unitset::iterator i = this->begin(); i != this->end(); ++i )
      retSet += i->getInterceptors();
    return retSet;
  }
  Unitset Unitset::getLarva() const
  {
    Unitset retSet;  // the return set
    // Iterate all units in the set
    for ( Unitset::iterator i = this->begin(); i != this->end(); ++i )
      retSet += i->getLarva();
    return retSet;
  }
  ////////////////////////////////////////////////////////// Misc
  void Unitset::setClientInfo(void *clientInfo)
  {
    // Assign the client info to all units in the set
    for ( Unitset::iterator i = this->begin(); i != this->end(); ++i )
      i->setClientInfo(clientInfo);
  }


  bool Unitset::issueCommand(UnitCommand command)
  {
    return Broodwar->issueCommand(*this, command);
  }
  //--------------------------------------------- ATTACK MOVE ------------------------------------------------
  bool Unitset::attack(PositionOrUnit target, bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::attack(NULL, target, shiftQueueCommand));
  }
  //--------------------------------------------- BUILD ------------------------------------------------------
  bool Unitset::build(UnitType type, TilePosition target)
  {
    if ( target == TilePositions::None )
      return this->train(type);
    return this->issueCommand(UnitCommand::build(NULL, target, type));
  }
  //--------------------------------------------- BUILD ADDON ------------------------------------------------
  bool Unitset::buildAddon(UnitType type)
  {
    return this->issueCommand(UnitCommand::buildAddon(NULL,type));
  }
  //--------------------------------------------- TRAIN ------------------------------------------------------
  bool Unitset::train(UnitType type)
  {
    return this->issueCommand(UnitCommand::train(NULL,type));
  }
  //--------------------------------------------- MORPH ------------------------------------------------------
  bool Unitset::morph(UnitType type)
  {
    return this->issueCommand(UnitCommand::morph(NULL,type));
  }
  //--------------------------------------------- SET RALLY POSITION -----------------------------------------
  bool Unitset::setRallyPoint(PositionOrUnit target)
  {
    return this->issueCommand( UnitCommand::setRallyPoint(NULL, target) );
  }
  //--------------------------------------------- MOVE -------------------------------------------------------
  bool Unitset::move(Position target, bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::move(NULL,target, shiftQueueCommand));
  }
  //--------------------------------------------- PATROL -----------------------------------------------------
  bool Unitset::patrol(Position target, bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::patrol(NULL,target, shiftQueueCommand));
  }
  //--------------------------------------------- HOLD POSITION ----------------------------------------------
  bool Unitset::holdPosition(bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::holdPosition(NULL, shiftQueueCommand));
  }
  //--------------------------------------------- STOP -------------------------------------------------------
  bool Unitset::stop(bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::stop(NULL, shiftQueueCommand));
  }
  //--------------------------------------------- FOLLOW -----------------------------------------------------
  bool Unitset::follow(Unit* target, bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::follow(NULL,target, shiftQueueCommand));
  }
  //--------------------------------------------- GATHER -----------------------------------------------------
  bool Unitset::gather(Unit* target, bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::gather(NULL,target, shiftQueueCommand));
  }
  //--------------------------------------------- RETURN CARGO -----------------------------------------------
  bool Unitset::returnCargo(bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::returnCargo(NULL, shiftQueueCommand));
  }
  //--------------------------------------------- REPAIR -----------------------------------------------------
  bool Unitset::repair(Unit* target, bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::repair(NULL,target, shiftQueueCommand));
  }
  //--------------------------------------------- BURROW -----------------------------------------------------
  bool Unitset::burrow()
  {
    return this->issueCommand(UnitCommand::burrow(NULL));
  }
  //--------------------------------------------- UNBURROW ---------------------------------------------------
  bool Unitset::unburrow()
  {
    return this->issueCommand(UnitCommand::unburrow(NULL));
  }
  //--------------------------------------------- CLOAK ------------------------------------------------------
  bool Unitset::cloak()
  {
    return this->issueCommand(UnitCommand::cloak(NULL));
  }
  //--------------------------------------------- DECLOAK ----------------------------------------------------
  bool Unitset::decloak()
  {
    return this->issueCommand(UnitCommand::decloak(NULL));
  }
  //--------------------------------------------- SIEGE ------------------------------------------------------
  bool Unitset::siege()
  {
    return this->issueCommand(UnitCommand::siege(NULL));
  }
  //--------------------------------------------- UNSIEGE ----------------------------------------------------
  bool Unitset::unsiege()
  {
    return this->issueCommand(UnitCommand::unsiege(NULL));
  }
  //--------------------------------------------- LIFT -------------------------------------------------------
  bool Unitset::lift()
  {
    return this->issueCommand(UnitCommand::lift(NULL));
  }
  //--------------------------------------------- LOAD -------------------------------------------------------
  bool Unitset::load(Unit* target, bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::load(NULL,target, shiftQueueCommand));
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool Unitset::unloadAll(bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::unloadAll(NULL, shiftQueueCommand));
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool Unitset::unloadAll(Position target, bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::unloadAll(NULL,target, shiftQueueCommand));
  }
  //--------------------------------------------- RIGHT CLICK ------------------------------------------------
  bool Unitset::rightClick(PositionOrUnit target, bool shiftQueueCommand)
  {
    return this->issueCommand(UnitCommand::rightClick(NULL,target, shiftQueueCommand));
  }
  //--------------------------------------------- HALT CONSTRUCTION ------------------------------------------
  bool Unitset::haltConstruction()
  {
    return this->issueCommand(UnitCommand::haltConstruction(NULL));
  }
  //--------------------------------------------- CANCEL CONSTRUCTION ----------------------------------------
  bool Unitset::cancelConstruction()
  {
    return this->issueCommand(UnitCommand::cancelConstruction(NULL));
  }
  //--------------------------------------------- CANCEL ADDON -----------------------------------------------
  bool Unitset::cancelAddon()
  {
    return this->issueCommand(UnitCommand::cancelAddon(NULL));
  }
  //--------------------------------------------- CANCEL TRAIN -----------------------------------------------
  bool Unitset::cancelTrain(int slot)
  {
    return this->issueCommand(UnitCommand::cancelTrain(NULL, slot));
  }
  //--------------------------------------------- CANCEL MORPH -----------------------------------------------
  bool Unitset::cancelMorph()
  {
    return this->issueCommand(UnitCommand::cancelMorph(NULL));
  }
  //--------------------------------------------- CANCEL RESEARCH --------------------------------------------
  bool Unitset::cancelResearch()
  {
    return this->issueCommand(UnitCommand::cancelResearch(NULL));
  }
  //--------------------------------------------- CANCEL UPGRADE ---------------------------------------------
  bool Unitset::cancelUpgrade()
  {
    return this->issueCommand(UnitCommand::cancelUpgrade(NULL));
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool Unitset::useTech(TechType tech, PositionOrUnit target)
  {
    if ( target.isUnit() && target.getUnit() == NULL )
      return this->issueCommand(UnitCommand::useTech(NULL,tech));
    return this->issueCommand(UnitCommand::useTech(NULL,tech,target));
  }
  //--------------------------------------------- PLACE COP --------------------------------------------------
  bool Unitset::placeCOP(TilePosition target)
  {
    return this->issueCommand(UnitCommand::placeCOP(NULL, target));
  }
}

