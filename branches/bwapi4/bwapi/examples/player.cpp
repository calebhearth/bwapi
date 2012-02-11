#include <BWAPI.h>

using namespace BWAPI;

void ExampleAIModule::onFrame()
{
	/// [getName]
	std::string name = Broodwar->self()->getName();
	Broodwar->sendText("Hello, my name is %s.", name.c_str());
	/// [getName]
	
	/// [getUnits]
	Unitset myUnits = Broodwar->self()->getUnits();
	myUnits->burrow();	// order all units to burrow
	/// [getUnits]
	
	/// [getRace]
	if ( Broodwar->enemy()->getRace() == Races::Zerg )
		Broodwar->sendText("I'll send you back to Char, Zerg scum!");
	/// [getRace]
	
	/// [getType]
	if ( Broodwar->enemy()->getType() == PlayerTypes::Player )
		Broodwar->sendText("%s, can you please ally?", Broodwar->enemy()->getName().c_str() );
	/// [getType]
	
	
}

