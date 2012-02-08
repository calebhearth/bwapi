#include <BWAPI/Vectorset.h>
#include <BWAPI/Unitset.h>
#include <BWAPI/Position.h>
#include <BWAPI/Unit.h>

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

		// Add up the positions for all units in the set
		for ( Unitset::iterator i = this->begin(); i != this->end(); ++i )
			retPosition += i->getPosition();

		// Divides the position by the size of the set and returns it
		retPosition /= this->size();
		return retPosition;
	}
	////////////////////////////////////////////////////////// sets
	Unitset Unitset::getLoadedUnits() const
	{
		Unitset retSet;	// the return set
		// Iterate all units in the set
		for ( Unitset::iterator i = this->begin(); i != this->end(); ++i )
			retSet += i->getLoadedUnits();
		return retSet;
	}
	Unitset Unitset::getInterceptors() const
	{
		Unitset retSet;	// the return set
		// Iterate all units in the set
		for ( Unitset::iterator i = this->begin(); i != this->end(); ++i )
			retSet += i->getInterceptors();
		return retSet;
	}
	Unitset Unitset::getLarva() const
	{
		Unitset retSet;	// the return set
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

}

