#include "Logger.h"
#include <stdlib.h> // needed to use system() function
namespace Util
{
  //---------------------------- CONSTRUCTOR ------------------------------------
  Logger::Logger(const std::string& name, LogLevel::Enum levelToLog)
  :name(name + ".log")
  ,levelToLog(levelToLog)
  {
  }
  //------------------------------- LOG -----------------------------------------
  bool Logger::log(const    std::string&   message, 
                            LogLevel::Enum logLevel)
  {
    if (levelToLog > this->levelToLog)
      return true;
    char time[9];
    _strtime(time);
    FILE *f = fopen(name.c_str(),"at");
    if (!f)
      return false;
    fprintf(f, "%s %s\n", time, message.c_str());
    fclose(f);
    if (globalLog != NULL &&
        this != globalLog)
      globalLog->log(message, levelToLog);
    return true;
  }
  //------------------------------- SET FILE NAME -------------------------------
  void Logger::setFileName(const std::string& name)
  {
    this->name = name;
  }
  //-----------------------------------------------------------------------------
  Logger* Logger::globalLog = new Logger("global", LogLevel::MicroDetailed);
}
