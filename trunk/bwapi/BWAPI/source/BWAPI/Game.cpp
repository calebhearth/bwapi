#include "Game.h"

#include "../BWAPI/Player.h"
#include "../BWAPI/Unit.h"
#include "../BWAPI/UnitPrototype.h"
#include "../BWAPI/UnitPrototypeDefinitions.h"

#include "../BW/Offsets.h"
#include "../BW/UnitData.h"
#include "../BW/OrderTypes.h"

#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>


namespace BWAPI 
{
  //------------------------------ CONSTRUCTOR ----------------------------------
  Game::Game()
  {
    unitArrayCopy = new BW::UnitArray;

    for (int i = 0; i < 12; i++)
      players[i] = new Player((u8)i);    
    
    players[11]->setName("Player 12 (Neutral)");
    
    for (int i = 0; i < 1700; i++)
      units[i] = new Unit(&unitArrayCopy->unit[i], &BW::UnitNodeTable->unit[i]);

    this->update();
  }
  //------------------------------- DESTRUCTOR ----------------------------------
  Game::~Game()
  {
    delete unitArrayCopy;

    for (int i = 0; i < 12; i++)
      delete players[i];

    for (int i = 0; i < 1700; i++)
      delete units[i];
  }
  int count = 0;
     void __fastcall IssueCommand(PBYTE pbBuffer, int iSize) {
     static const int BWFXNIssuseCommand = 0x4858F0;
      __asm 
        {
        MOV ECX, pbBuffer
        MOV EDX, iSize
        CALL [BWFXNIssuseCommand]
        }
     }
  //---------------------------------- UPDATE -----------------------------------
  void Game::update()
  {
    memcpy(this->unitArrayCopy, BW::UnitNodeTable, sizeof(BW::UnitArray));
    players[11]->setName("Player 12 (Neutral)");
  }
  Unit *cc;
  int closerToCC(Unit*& unit1, Unit*& unit2)
  {
   return cc->getDistance(unit1) < cc->getDistance(unit2);
  }
  //----------------------------- JMP PATCH -----------------------------------
  void Game::test(void)
  {

    /*
    I will implement this later on using some correct pointers method on unit
    _w64 int memoryPositionDifference = this->unitArrayCopy - UNIT_NODE_TABLE; 
    for (int i = 0; i < 1700; i++)
    {
    units[i]->rawData->previousUnit += memoryPositionDifference;
    units[i]->rawData->nextUnit += memoryPositionDifference;
    }
    */

    FILE *f;
    f = fopen("bwapi.log","at"); 
    /*if (this->inGame())
      fprintf(f, "Game running.\n");
    else
      fprintf(f, "Game not running.\n");*/
    fprintf(f,"\nUnit List:\n");
    if (this->inGame())
      fprintf(f,"In game:\n");
    else
      fprintf(f,"Not in game:\n");
    bool found = false;
    std::vector<Unit*> unitList;
    cc = NULL;
    Player *marwin = NULL;
    for (int i = 0; i < 1700; i++)
    {
      if (units[i]->isValid() &&
          strcmp(units[i]->getOwner()->getName(),"NEM)Marwin") == 0)
      {
        marwin = units[i]->getOwner();
        found = true;
        if (units[i]->getPrototype() == Prototypes::SCV &&
            units[i]->getOrderID() == BW::OrderID::Idle)
          unitList.push_back(this->units[i]);
        else
         if (units[i]->getPrototype() == Prototypes::CommandCenter)
         {
           cc = this->units[i];
           fprintf(f, "%s (%d,%d) \n", 
                    units[i]->getPrototype()->getName().c_str(),
                    units[i]->getPosition().x,
                    units[i]->getPosition().y);
           for (int j = 0; j < 5; j++)
             fprintf(f,"Queue %d = (%d)\n", j, units[i]->getRawData()->queue[j]);
           fprintf(f,"Current queue slot = %d\n", units[i]->getRawData()->queueSlot);
         }
      }
    }
    bool reselected = false;
    BW::UnitData** selected = NULL;
    if (marwin != NULL)
    {
      fprintf(f, "Before commands:\n");
      for (int i = 0; i < 12; i++)
      {
        int unitID = 0;
        if (marwin->selectedUnit()[i] != NULL) 
           unitID = marwin->selectedUnit()[i]->unitID;
        fprintf(f, "Selected unit %d (%d) \n", i, unitID);
      }
      /*selected = new BW::UnitData * [13];
      memcpy(selected, marwin->selectedUnit(), 4*12);
      selected[12] = NULL;*/
    }
    if (cc != NULL)
    {
      BW::UnitData * * list = new BW::UnitData * [2];
      list[0] = cc->getOriginalRawData();
      list[1] = NULL;
      int one = 1;
      void (_stdcall* selectUnitsHelperSTD)(int, BW::UnitData * *, bool, bool) = (void (_stdcall*) (int, BW::UnitData * *, bool, bool))0x0049AB90;
      selectUnitsHelperSTD(one, list, true, true);
      reselected = true;
      delete[] list;
      if (cc->hasEmptyQueue())
        IssueCommand((PBYTE)&BW::Orders::TrainUnit(BW::UnitType::Terran_SCV), 0x3);  
    }
    if (unitList.size() != 0)
    {
      std::vector<Unit *> mineralList;
      for (int i = 0; i < 1700; i++)
      {
        if (units[i]->isValid() && 
            units[i]->getPrototype() == Prototypes::Minerals)
         mineralList.push_back(units[i]);
      }
      std::sort(mineralList.begin(),mineralList.end(), closerToCC);
      for (unsigned int i = 0; i < unitList.size(); i++)
      {
        BW::UnitData * * list = new BW::UnitData * [2];
        list[0] = unitList[i]->getOriginalRawData();
	       list[1] = NULL;
        int one = 1;
        void (_stdcall* selectUnitsHelperSTD)(int, BW::UnitData * *, bool, bool) = (void (_stdcall*) (int, BW::UnitData * *, bool, bool))0x0049AB90;
	 	     selectUnitsHelperSTD(one, list, true, true);
        reselected = true;
        delete[] list;
        BW::Orders::MoveTarget order(mineralList[i]);
        fprintf(f, "Target unit id = %d .\n", order.unitOrder);
        IssueCommand((PBYTE)&order, 0xA);  
      }
    }
   
    if (found)
       count++;
    else
    {
      this->changeSlot(BW::Orders::ChangeSlot::Computer, 1);
      this->changeRace(BW::Orders::ChangeRace::Zerg, 1);
      this->changeRace(BW::Orders::ChangeRace::Terran, 0);
    }
  /* if (reselected)
    {
     fprintf(f, "After commands:\n");
     for (int i = 0; i < 12; i++)
      {
        int unitID = 0;
        if (marwin->selectedUnit()[i] != NULL) 
           unitID = marwin->selectedUnit()[i]->unitID;
        fprintf(f, "Selected unit %d (%d) \n", i, unitID);
      }
      BW::UnitData * * list = selected;
      int unitCount = 0;
      while (selected[count] != NULL)
        unitCount ++;
      void (_stdcall* selectUnitsHelperSTD)(int, BW::UnitData * *, bool, bool) = (void (_stdcall*) (int, BW::UnitData * *, bool, bool))0x0049AB90;
	     selectUnitsHelperSTD(unitCount, list, true, true);
    }
    if (selected)
      delete [] selected;*/
    if (count > 0)
    {
      char message[30];
      sprintf(message, "Update %d", count);
      this->print(message);
    }
    fclose(f);
  }
  //----------------------------- JMP PATCH -----------------------------------
  
  #pragma warning(push)
  #pragma warning(disable:4311)
  #pragma warning(disable:4312)

  VOID JmpPatch(void *pDest, void *pSrc, int nNops = 0)
  {
      DWORD OldProt;
      
      VirtualProtect(pSrc, 5 + nNops, PAGE_EXECUTE_READWRITE, &OldProt);
      
      *(u8*)pSrc = (u8)0xE9;
      *(DWORD*)((DWORD)pSrc + 1) = (DWORD)pDest - (DWORD)pSrc - 5;
      
      for (int i = 0; i < nNops; ++i)
      {
          *(BYTE*)((DWORD)pSrc + 5 + i) = 0x90;
      }
      
      VirtualProtect(pSrc, 5 + nNops, OldProt, &OldProt);
  }
  VOID JmpPatch(void *pDest, void *pSrc, unsigned char jmp = 0xE9, int nNops = 0)
  {
	  DWORD OldProt;
      
      VirtualProtect(pSrc, 5 + nNops, PAGE_EXECUTE_READWRITE, &OldProt);
      
      *(char*)pSrc = (char)jmp;
      *(DWORD*)((DWORD)pSrc + 1) = (DWORD)pDest - (DWORD)pSrc - 5;
      
	  for (int i = 0; i < nNops; ++i)
	  {
	      *(BYTE*)((DWORD)pSrc + 5 + i) = 0x90;
	  }
  	    
	  VirtualProtect(pSrc, 5 + nNops, OldProt, &OldProt);
  }

  #pragma warning(pop)

  void DrawStaticText(char* Text, int X, int Y)
  {
      __asm
      {
          mov eax, Text
          mov edx, Y
          push edx
          mov esi, X
          call [BW::BWFXN_HUD]
      }
  }
  char buffer[200];
  int xToPrint, yToPrint;
  void ProcessTest()
  {
    DrawStaticText(buffer, xToPrint , yToPrint);
  }

  void __declspec(naked) Test()
  {
    static int ReturnAddr = (0x48CC25+6);
    __asm 
	   {
      pushad
      call ProcessTest
      popad
      mov esi, 0x0A
      jmp [ReturnAddr]
    }
  }
//------------------------------ PRINT X Y ------------------------------------
  void Game::printXY(int x, int y, char* text) const
  {
   strcpy(buffer, text);
   xToPrint = x;
   yToPrint = y;
   JmpPatch(&Test,(PBYTE)0x48CC25,6);
  }
  //-----------------------------------------------------------------------------
  #pragma warning(push)
  #pragma warning(disable:4312)
  bool Game::inGame()
  {
    return (*((unsigned char *)BW::BWFXN_InGame) != 0);
  }
  //----------------------------------- PRINT -----------------------------------
  void Game::print(char *text) const
  {
   void (_stdcall* sendText)(char *) = (void (_stdcall*) (char *))BW::BWXFN_PrintText;
	 	sendText(text);
  }
  #pragma warning(pop)
  //-----------------------------------------------------------------------------
  void Game::printPublic(char *text) const
  {
   void (_stdcall* sendText)(char *) = (void (_stdcall*) (char *))BW::BWXFN_PrintPublicText;
	 	sendText(text);
  }
  //--------------------------------- CHANGE SLOT -----------------------------
  void Game::changeSlot(BW::Orders::ChangeSlot::Slot slot, u8 slotID)
  {
    IssueCommand((PBYTE)&BW::Orders::ChangeSlot(slot, slotID),3); 
  }
  //--------------------------------- CHANGE RACE -------------------------------
  void Game::changeRace(BW::Orders::ChangeRace::Race race, u8 slotID)
  {
    IssueCommand((PBYTE)&BW::Orders::ChangeRace(race, slotID),3); 
  }
  //-----------------------------------------------------------------------------
};
