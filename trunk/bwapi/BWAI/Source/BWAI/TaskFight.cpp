#include "TaskFight.h"

namespace BWAI
{
  //------------------------------ CONSTRUCTOR --------------------------------
  TaskFight::TaskFight()
  :Task()
  {
  }
  //------------------------------- DESTRUCTOR --------------------------------
  TaskFight::~TaskFight()
  {
  }
  //-------------------------------- EXECUTE ----------------------------------
  bool TaskFight::execute()
  {
    return false;
  }
  //-------------------------------- GET TYPE ---------------------------------
  TaskType::Enum TaskFight::getType()
  {
    return TaskType::Fight;
  }
  //---------------------------------------------------------------------------
}