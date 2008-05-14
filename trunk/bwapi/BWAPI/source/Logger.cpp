#include "Logger.h"
//---------------------------- CONSTRUCTOR ------------------------------------
Logger::Logger(const std::string& name, LogLevel::Enum levelToLog)
:name(name + ".log")
,levelToLog(levelToLog)
{
}
//------------------------------- LOG -----------------------------------------
bool Logger::log(const    std::string&   message, 
                          LogLevel::Enum level)
{
  if (level <= this->levelToLog)
    return true;
  char time[9];
  _strtime(time);
  FILE *f = fopen(name.c_str(),"at");
  if (!f)
    return false;
  fprintf(f, "%s %s\n", time, message.c_str());
  fclose(f);
  if (this != &globalLog)
    globalLog.log(message, level);
  return true;
}
//-----------------------------------------------------------------------------
Logger Logger::globalLog = Logger("global.log", LogLevel::MicroDetailed);


