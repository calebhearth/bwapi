#include "Branch.h"

#include <tinyxml.h>
#include <Util/FileLogger.h>
#include <Util/Exceptions.h>
#include <Util/Xml.h>
#include "CommandBuild.h"
#include "CommandFight.h"
#include "CommandChangeWeights.h"
#include "CommandInvent.h"
#include "CommandUpgrade.h"
#include "CommandCall.h"
#include "CommandRegisterException.h"
#include "Condition.h"

namespace BuildOrder
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Branch::Branch(TiXmlElement* xmlElement)
      : condition(NULL)
  {
    this->name = Util::Xml::getRequiredAttribute(xmlElement, "name");

    Util::Logger* log = new Util::FileLogger("elements", Util::LogLevel::MicroDetailed);
    for (TiXmlElement* i = xmlElement->FirstChildElement(); i != NULL; i = i->NextSiblingElement())
      if (i->ValueTStr() == "condition" && i == xmlElement->FirstChildElement())
        this->condition = Condition::load(i);
      else if (i->ValueTStr() == "build")
        this->commands.push_back(new CommandBuild(i));
      else if (i->ValueTStr() == "train")
        this->commands.push_back(new CommandChangeWeights(i));
      else if (i->ValueTStr() == "invent")
        this->commands.push_back(new CommandInvent(i));
      else if (i->ValueTStr() == "upgrade")
        this->commands.push_back(new CommandUpgrade(i));
      else if (i->ValueTStr() == "fight")
        this->commands.push_back(new CommandFight(i));
      else if (i->ValueTStr() == "call")
        this->commands.push_back(new CommandCall(i));
      else if (i->ValueTStr() == "register-exception")
        this->commands.push_back(new CommandRegisterException(i));
      else
        throw ParseException((std::string)"Unknown element '" + i->ValueTStr().c_str() + "' found in <bulid-order>");

    delete log;
  }
  //------------------------------------------------ GET NAME ------------------------------------------------
  std::string Branch::getName()
  {
    return this->name;
  }
  //----------------------------------------------------------------------------------------------------------
}