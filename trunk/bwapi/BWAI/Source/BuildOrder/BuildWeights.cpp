#include "BuildWeights.h"

#include <tinyxml.h>
#include <Util/Logger.h>
#include <Util/Xml.h>
#include <BWAPI.h>
#include "Root.h"

namespace BuildOrder
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  BuildWeights::BuildWeights(TiXmlElement* xmlElement)
  {
    this->factory = BWAPI::UnitTypes::getUnitType(Util::Xml::getRequiredAttribute(xmlElement, "building"));
    for (TiXmlElement* buildElement = xmlElement->FirstChildElement("build"); 
         buildElement != NULL; 
         buildElement = buildElement->NextSiblingElement("build"))
      this->weights.push_back(new BuildWeight(buildElement));
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  BuildWeights::~BuildWeights()
  {
    for each (BuildWeight* i in this->weights)
      delete i;
  }
  //----------------------------------------------------------------------------------------------------------
}