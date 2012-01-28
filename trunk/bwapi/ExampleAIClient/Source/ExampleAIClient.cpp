#include <stdio.h>

#include <BWAPI.h>
#include <BWAPI/Client.h>
//#include <BWTA.h>

#include <windows.h>

#include <string>

using namespace BWAPI;

void drawStats();
void drawBullets();
void drawVisibilityData();
//void drawTerrainData();
void showPlayers();
void showForces();
bool show_bullets;
bool show_visibility_data;
/*
bool analyzed;
bool analysis_just_finished;
BWTA::Region* home;
BWTA::Region* enemy_base;
DWORD WINAPI AnalyzeThread();
*/
void reconnect()
{
  while(!BWAPIClient.connect())
  {
    Sleep(1000);
  }
}
int main(int argc, const char* argv[])
{
  BWAPI::BWAPI_init();
  printf("Connecting...");
  reconnect();
  while(true)
  {
    printf("waiting to enter match\n");
    while (!Broodwar->isInGame())
    {
      BWAPI::BWAPIClient.update();
      if (!BWAPI::BWAPIClient.isConnected())
      {
        printf("Reconnecting...\n");
        reconnect();
      }
    }
    printf("starting match!\n");
    Broodwar->sendText("Hello world!");
    Broodwar->printf("The map is %s, a %d player map",Broodwar->mapName().c_str(),Broodwar->getStartLocations().size());
    // Enable some cheat flags
    Broodwar->enableFlag(Flag::UserInput);
    // Uncomment to enable complete map information
    //Broodwar->enableFlag(Flag::CompleteMapInformation);
    /*
    //read map information into BWTA so terrain analysis can be done in another thread
    BWTA::readMap();
    analyzed=false;
    analysis_just_finished=false;
    */
    show_bullets=false;
    show_visibility_data=false;

    if (Broodwar->isReplay())
    {
      Broodwar->printf("The following players are in this replay:");
      for(std::set<Player*>::iterator p=Broodwar->getPlayers().begin();p!=Broodwar->getPlayers().end();p++)
      {
        if (!(*p)->getUnits().empty() && !(*p)->isNeutral())
        {
          Broodwar->printf("%s, playing as a %s",(*p)->getName().c_str(),(*p)->getRace().getName().c_str());
        }
      }
    }
    else
    {
      Broodwar->printf("The match up is %s v %s",
        Broodwar->self()->getRace().getName().c_str(),
        Broodwar->enemy()->getRace().getName().c_str());

      //send each worker to the mineral field that is closest to it
      for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
      {
        if ((*i)->getType().isWorker())
        {
          Unit* closestMineral=NULL;
          for(std::set<Unit*>::iterator m=Broodwar->getMinerals().begin();m!=Broodwar->getMinerals().end();m++)
          {
            if (closestMineral==NULL || (*i)->getDistance(*m)<(*i)->getDistance(closestMineral))
              closestMineral=*m;
          }
          if (closestMineral!=NULL)
            (*i)->rightClick(closestMineral);
        }
        else if ((*i)->getType().isResourceDepot())
        {
          //if this is a center, tell it to build the appropiate type of worker
          (*i)->train(Broodwar->self()->getRace().getWorker());
        }
      }
    }
    while(Broodwar->isInGame())
    {
      for(std::list<Event>::iterator e=Broodwar->getEvents().begin();e!=Broodwar->getEvents().end();e++)
      {
        switch(e->getType())
        {
          case EventType::MatchEnd:
            if (e->isWinner())
              printf("I won the game\n");
            else
              printf("I didn't win the game\n");
            break;
          case EventType::SendText:
            if (e->getText()=="/show bullets")
            {
              show_bullets=!show_bullets;
            } else if (e->getText()=="/show players")
            {
              showPlayers();
            } else if (e->getText()=="/show forces")
            {
              showForces();
            } else if (e->getText()=="/show visibility")
            {
              show_visibility_data=!show_visibility_data;
            } /* else if (e->text=="/analyze")
            {
              if (analyzed == false)
              {
                Broodwar->printf("Analyzing map... this may take a minute");
                CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, NULL, 0, NULL);
              }
            } */ else
            {
              Broodwar->printf("You typed '%s'!",e->getText().c_str());
            }
            break;
          case EventType::ReceiveText:
            Broodwar->printf("%s said '%s'", e->getPlayer()->getName().c_str(), e->getText().c_str());
            break;
          case EventType::PlayerLeft:
            Broodwar->sendText("%s left the game.",e->getPlayer()->getName().c_str());
            break;
          case EventType::NukeDetect:
            if (e->getPosition()!=Positions::Unknown)
            {
              Broodwar->drawCircleMap(e->getPosition().x(),e->getPosition().y(),40,Colors::Red,true);
              Broodwar->printf("Nuclear Launch Detected at (%d,%d)",e->getPosition().x(),e->getPosition().y());
            }
            else
              Broodwar->printf("Nuclear Launch Detected");
            break;
          case EventType::UnitCreate:
            if (!Broodwar->isReplay())
              Broodwar->sendText("A %s [%x] has been created at (%d,%d)",e->getUnit()->getType().getName().c_str(),e->getUnit(),e->getUnit()->getPosition().x(),e->getUnit()->getPosition().y());
            else
            {
              /*if we are in a replay, then we will print out the build order
              (just of the buildings, not the units).*/
              if (e->getUnit()->getType().isBuilding() && e->getUnit()->getPlayer()->isNeutral()==false)
              {
                int seconds=Broodwar->getFrameCount()/24;
                int minutes=seconds/60;
                seconds%=60;
                Broodwar->sendText("%.2d:%.2d: %s creates a %s",minutes,seconds,e->getUnit()->getPlayer()->getName().c_str(),e->getUnit()->getType().getName().c_str());
              }
            }
            break;
          case EventType::UnitDestroy:
            if (!Broodwar->isReplay())
              Broodwar->sendText("A %s [%x] has been destroyed at (%d,%d)",e->getUnit()->getType().getName().c_str(),e->getUnit(),e->getUnit()->getPosition().x(),e->getUnit()->getPosition().y());
            break;
          case EventType::UnitMorph:
            if (!Broodwar->isReplay())
              Broodwar->sendText("A %s [%x] has been morphed at (%d,%d)",e->getUnit()->getType().getName().c_str(),e->getUnit(),e->getUnit()->getPosition().x(),e->getUnit()->getPosition().y());
            else
            {
              /*if we are in a replay, then we will print out the build order
              (just of the buildings, not the units).*/
              if (e->getUnit()->getType().isBuilding() && e->getUnit()->getPlayer()->isNeutral()==false)
              {
                int seconds=Broodwar->getFrameCount()/24;
                int minutes=seconds/60;
                seconds%=60;
                Broodwar->sendText("%.2d:%.2d: %s morphs a %s",minutes,seconds,e->getUnit()->getPlayer()->getName().c_str(),e->getUnit()->getType().getName().c_str());
              }
            }
            break;
          case EventType::UnitShow:
            if (!Broodwar->isReplay())
              Broodwar->sendText("A %s [%x] has been spotted at (%d,%d)",e->getUnit()->getType().getName().c_str(),e->getUnit(),e->getUnit()->getPosition().x(),e->getUnit()->getPosition().y());
            break;
          case EventType::UnitHide:
            if (!Broodwar->isReplay())
              Broodwar->sendText("A %s [%x] was last seen at (%d,%d)",e->getUnit()->getType().getName().c_str(),e->getUnit(),e->getUnit()->getPosition().x(),e->getUnit()->getPosition().y());
            break;
          case EventType::UnitRenegade:
            if (!Broodwar->isReplay())
              Broodwar->sendText("A %s [%x] is now owned by %s",e->getUnit()->getType().getName().c_str(),e->getUnit(),e->getUnit()->getPlayer()->getName().c_str());
            break;
          case EventType::SaveGame:
            Broodwar->sendText("The game was saved to \"%s\".",e->getText().c_str());
            break;
        }
      }

      if (show_bullets)
        drawBullets();

      if (show_visibility_data)
        drawVisibilityData();

      drawStats();
      Broodwar->drawTextScreen(300,0,"FPS: %f",Broodwar->getAverageFPS());
      /*
      if (analyzed && Broodwar->getFrameCount()%30==0)
      {
        //order one of our workers to guard our chokepoint.
        for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
        {
          if ((*i)->getType().isWorker())
          {
            //get the chokepoints linked to our home region
            std::set<BWTA::Chokepoint*> chokepoints= home->getChokepoints();
            double min_length=10000;
            BWTA::Chokepoint* choke=NULL;

            //iterate through all chokepoints and look for the one with the smallest gap (least width)
            for(std::set<BWTA::Chokepoint*>::iterator c=chokepoints.begin();c!=chokepoints.end();c++)
            {
              double length=(*c)->getWidth();
              if (length<min_length || choke==NULL)
              {
                min_length=length;
                choke=*c;
              }
            }

            //order the worker to move to the center of the gap
            (*i)->rightClick(choke->getCenter());
            break;
          }
        }
      }
      if (analysis_just_finished)
      {
        Broodwar->printf("Finished analyzing map.");
        analysis_just_finished=false;
      }
      if (analyzed)
        drawTerrainData();
      */
      BWAPI::BWAPIClient.update();
      if (!BWAPI::BWAPIClient.isConnected())
      {
        printf("Reconnecting...\n");
        reconnect();
      }
    }
    printf("Game ended\n");
  }
  system("pause");
  return 0;
}
/*
DWORD WINAPI AnalyzeThread()
{
  BWTA::analyze();

  //self start location only available if the map has base locations
  if (BWTA::getStartLocation(BWAPI::Broodwar->self())!=NULL)
  {
    home       = BWTA::getStartLocation(BWAPI::Broodwar->self())->getRegion();
  }
  //enemy start location only available if Complete Map Information is enabled.
  if (BWTA::getStartLocation(BWAPI::Broodwar->enemy())!=NULL)
  {
    enemy_base = BWTA::getStartLocation(BWAPI::Broodwar->enemy())->getRegion();
  }
  analyzed   = true;
  analysis_just_finished = true;
  return 0;
}
*/
void drawStats()
{
  std::set<Unit*> myUnits;
  if (Broodwar->isReplay())
    myUnits = Broodwar->getAllUnits();
  else
    myUnits = Broodwar->self()->getUnits();

  Broodwar->drawTextScreen(5,0,"I have %d units:",myUnits.size());
  std::map<UnitType, int> unitTypeCounts;
  for(std::set<Unit*>::iterator i=myUnits.begin();i!=myUnits.end();i++)
  {
    if (unitTypeCounts.find((*i)->getType())==unitTypeCounts.end())
    {
      unitTypeCounts.insert(std::make_pair((*i)->getType(),0));
    }
    unitTypeCounts.find((*i)->getType())->second++;
  }
  int line=1;
  for(std::map<UnitType,int>::iterator i=unitTypeCounts.begin();i!=unitTypeCounts.end();i++)
  {
    Broodwar->drawTextScreen(5,16*line,"- %d %ss",(*i).second, (*i).first.getName().c_str());
    line++;
  }
}

void drawBullets()
{
  std::set<Bullet*> bullets = Broodwar->getBullets();
  for(std::set<Bullet*>::iterator i=bullets.begin();i!=bullets.end();i++)
  {
    Position p=(*i)->getPosition();
    double velocityX = (*i)->getVelocityX();
    double velocityY = (*i)->getVelocityY();
    if ((*i)->getPlayer()==Broodwar->self())
    {
      Broodwar->drawLineMap(p.x(),p.y(),p.x()+(int)velocityX,p.y()+(int)velocityY,Colors::Green);
      Broodwar->drawTextMap(p.x(),p.y(),"\x07%s",(*i)->getType().getName().c_str());
    }
    else
    {
      Broodwar->drawLineMap(p.x(),p.y(),p.x()+(int)velocityX,p.y()+(int)velocityY,Colors::Red);
      Broodwar->drawTextMap(p.x(),p.y(),"\x06%s",(*i)->getType().getName().c_str());
    }
  }
}

void drawVisibilityData()
{
  for(int x=0;x<Broodwar->mapWidth();x++)
  {
    for(int y=0;y<Broodwar->mapHeight();y++)
    {
      if (Broodwar->isExplored(x,y))
      {
        if (Broodwar->isVisible(x,y))
          Broodwar->drawDotMap(x*32+16,y*32+16,Colors::Green);
        else
          Broodwar->drawDotMap(x*32+16,y*32+16,Colors::Blue);
      }
      else
        Broodwar->drawDotMap(x*32+16,y*32+16,Colors::Red);
    }
  }
}
/*
void drawTerrainData()
{
  //we will iterate through all the base locations, and draw their outlines.
  for(std::set<BWTA::BaseLocation*>::const_iterator i=BWTA::getBaseLocations().begin();i!=BWTA::getBaseLocations().end();i++)
  {
    TilePosition p=(*i)->getTilePosition();
    Position c=(*i)->getPosition();

    //draw outline of center location
    Broodwar->drawBox(CoordinateType::Map,p.x()*32,p.y()*32,p.x()*32+4*32,p.y()*32+3*32,Colors::Blue,false);

    //draw a circle at each mineral patch
    for(std::set<BWAPI::Unit*>::const_iterator j=(*i)->getStaticMinerals().begin();j!=(*i)->getStaticMinerals().end();j++)
    {
      Position q=(*j)->getInitialPosition();
      Broodwar->drawCircle(CoordinateType::Map,q.x(),q.y(),30,Colors::Cyan,false);
    }

    //draw the outlines of vespene geysers
    for(std::set<BWAPI::Unit*>::const_iterator j=(*i)->getGeysers().begin();j!=(*i)->getGeysers().end();j++)
    {
      TilePosition q=(*j)->getInitialTilePosition();
      Broodwar->drawBox(CoordinateType::Map,q.x()*32,q.y()*32,q.x()*32+4*32,q.y()*32+2*32,Colors::Orange,false);
    }

    //if this is an island expansion, draw a yellow circle around the base location
    if ((*i)->isIsland())
      Broodwar->drawCircle(CoordinateType::Map,c.x(),c.y(),80,Colors::Yellow,false);
  }

  //we will iterate through all the regions and draw the polygon outline of it in green.
  for(std::set<BWTA::Region*>::const_iterator r=BWTA::getRegions().begin();r!=BWTA::getRegions().end();r++)
  {
    BWTA::Polygon p=(*r)->getPolygon();
    for(int j=0;j<(int)p.size();j++)
    {
      Position point1=p[j];
      Position point2=p[(j+1) % p.size()];
      Broodwar->drawLine(CoordinateType::Map,point1.x(),point1.y(),point2.x(),point2.y(),Colors::Green);
    }
  }

  //we will visualize the chokepoints with red lines
  for(std::set<BWTA::Region*>::const_iterator r=BWTA::getRegions().begin();r!=BWTA::getRegions().end();r++)
  {
    for(std::set<BWTA::Chokepoint*>::const_iterator c=(*r)->getChokepoints().begin();c!=(*r)->getChokepoints().end();c++)
    {
      Position point1=(*c)->getSides().first;
      Position point2=(*c)->getSides().second;
      Broodwar->drawLine(CoordinateType::Map,point1.x(),point1.y(),point2.x(),point2.y(),Colors::Red);
    }
  }
}
*/
void showPlayers()
{
  std::set<Player*> players=Broodwar->getPlayers();
  for(std::set<Player*>::iterator i=players.begin();i!=players.end();i++)
  {
    Broodwar->printf("Player [%d]: %s is in force: %s",(*i)->getID(),(*i)->getName().c_str(), (*i)->getForce()->getName().c_str());
  }
}

void showForces()
{
  std::set<Force*> forces=Broodwar->getForces();
  for(std::set<Force*>::iterator i=forces.begin();i!=forces.end();i++)
  {
    std::set<Player*> players=(*i)->getPlayers();
    Broodwar->printf("Force %s has the following players:",(*i)->getName().c_str());
    for(std::set<Player*>::iterator j=players.begin();j!=players.end();j++)
    {
      Broodwar->printf("  - Player [%d]: %s",(*j)->getID(),(*j)->getName().c_str());
    }
  }
}
