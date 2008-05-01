#include "Game.h"
#include "Player.h"
#include "Offsets.h"
#include "Unit.h"
//------------------------------ CONSTRUCTOR ----------------------------------
Game::Game()
{
  for (int i = 0; i < 12; i++)
    players[i] = new Player(i);
  
  unitArrayCopy = new BW_UnitArray;
  
  for (int i = 0; i < 1700; i++)
     units[i] = new Unit(&unitArrayCopy->Unit[i]);

  this->update();
}
//------------------------------- DESTRUCTOR ----------------------------------
Game::~Game()
{
  for (int i = 0; i < 12; i++)
    delete players[i];
  delete unitArrayCopy;
}
//---------------------------------- UPDATE -----------------------------------
void Game::update()
{
  memcpy(this->players, UNIT_NODE_TABLE, sizeof(BW_UnitArray));
}
//-----------------------------------------------------------------------------