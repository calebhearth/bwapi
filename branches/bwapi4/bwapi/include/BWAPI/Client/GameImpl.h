#pragma once
#include <BWAPI.h>
#include "GameData.h"
#include "Client.h"
#include "Shape.h"
#include "Command.h"
#include "UnitCommand.h"
#include "ForceImpl.h"
#include "PlayerImpl.h"
#include "RegionImpl.h"
#include "UnitImpl.h"
#include "BulletImpl.h"
#include <list>
#include <map>
#include <set>
#include <vector>

#include <BWAPI/Unitset.h>
#include <BWAPI/Bulletset.h>
#include <BWAPI/Playerset.h>
#include <BWAPI/Forceset.h>
#include <BWAPI/Regionset.h>

namespace BWAPI
{
	class Force;
	class Player;
	class Unit;
	class GameImpl : public Game
	{
		private :
			int addShape(const BWAPIC::Shape &s);
			int addString(const char* text);
			int addText(BWAPIC::Shape &s, const char* text);
			int addCommand(const BWAPIC::Command &c);
			void clearAll();

			GameData* data;
			std::vector<ForceImpl>	forceVector;
			std::vector<PlayerImpl> playerVector;
			std::vector<UnitImpl>	 unitVector;
			std::vector<BulletImpl> bulletVector;
			RegionImpl *regionArray[5000];

			Forceset forces;
			Playerset playerSet;
			Unitset accessibleUnits;//all units that are accessible (and definitely alive)
			//notDestroyedUnits - accessibleUnits = all units that may or may not be alive (status unknown)
			Unitset minerals;
			Unitset geysers;
			Unitset neutralUnits;
			Unitset staticMinerals;
			Unitset staticGeysers;
			Unitset staticNeutralUnits;
			Bulletset bullets;
			std::set<Position> nukeDots;
			Unitset selectedUnits;
			Unitset pylons;
			Unitset unitsOnTileData[256][256];
			Regionset regionsList;

			std::set< TilePosition > startLocations;
			std::list< Event > events;
			bool flagEnabled[2];
			Player* thePlayer;
			Player* theEnemy;
			Player* theNeutral;
			Playerset _allies;
			Playerset _enemies;
			Playerset _observers;
			Error lastError;
			int textSize;

		public :
			Event makeEvent(BWAPIC::Event e);
			int addUnitCommand(BWAPIC::UnitCommand& c);
			bool inGame;
			GameImpl(GameData* data);
			void onMatchStart();
			void onMatchEnd();
			void onMatchFrame();
			const GameData* getGameData() const;
			Unit *_unitFromIndex(int index);

			virtual Forceset& getForces();
			virtual Playerset& getPlayers();
			virtual Unitset& getAllUnits();
			virtual Unitset& getMinerals();
			virtual Unitset& getGeysers();
			virtual Unitset& getNeutralUnits();

			virtual Unitset& getStaticMinerals();
			virtual Unitset& getStaticGeysers();
			virtual Unitset& getStaticNeutralUnits();

			virtual Bulletset& getBullets();
			virtual std::set< Position >& getNukeDots();
			virtual std::list< Event>& getEvents();

			virtual Force*	getForce(int forceID);
			virtual Player* getPlayer(int playerID);
			virtual Unit*	 getUnit(int unitID);
			virtual Unit*	 indexToUnit(int unitIndex);
			virtual Region* getRegion(int regionID);

			virtual GameType getGameType();
			virtual int getLatency();
			virtual int getFrameCount();
			virtual int getReplayFrameCount();
			virtual int getFPS();
			virtual double getAverageFPS();
			virtual BWAPI::Position getMousePosition();
			virtual bool getMouseState(MouseButton button);
			virtual bool getMouseState(int button);
			virtual bool getKeyState(Key key);
			virtual bool getKeyState(int key);
			virtual BWAPI::Position getScreenPosition();
			virtual void setScreenPosition(int x, int y);
			virtual void setScreenPosition(BWAPI::Position p);
			virtual void pingMinimap(int x, int y);
			virtual void pingMinimap(BWAPI::Position p);

			virtual bool		isFlagEnabled(int flag);
			virtual void		enableFlag(int flag);
			virtual Unitset&	getUnitsOnTile(int x, int y);
			virtual Unitset&	getUnitsInRectangle(int left, int top, int right, int bottom) const;
			virtual Unitset&	getUnitsInRectangle(BWAPI::Position topLeft, BWAPI::Position bottomRight) const;
			virtual Unitset&	getUnitsInRadius(BWAPI::Position center, int radius) const;
			virtual Error		getLastError() const;
			virtual bool		setLastError(BWAPI::Error e = Errors::None);

			virtual int			mapWidth();
			virtual int			mapHeight();
			virtual std::string mapFileName();
			virtual std::string mapPathName();
			virtual std::string mapName();
			virtual std::string mapHash();

			virtual bool isWalkable(int x, int y);
			virtual bool isWalkable(WalkPosition position);
			virtual int	 getGroundHeight(int x, int y);
			virtual int	 getGroundHeight(TilePosition position);
			virtual bool isBuildable(int x, int y, bool includeBuildings = false);
			virtual bool isBuildable(TilePosition position, bool includeBuildings = false);
			virtual bool isVisible(int x, int y);
			virtual bool isVisible(TilePosition position);
			virtual bool isExplored(int x, int y);
			virtual bool isExplored(TilePosition position);
			virtual bool hasCreep(int x, int y);
			virtual bool hasCreep(TilePosition position);
			virtual bool hasPower(int tileX, int tileY, UnitType unitType = UnitTypes::None) const;
			virtual bool hasPower(TilePosition position, UnitType unitType = UnitTypes::None) const;
			virtual bool hasPower(int tileX, int tileY, int tileWidth, int tileHeight, UnitType unitType = UnitTypes::None) const;
			virtual bool hasPower(TilePosition position, int tileWidth, int tileHeight, UnitType unitType = UnitTypes::None) const;
			virtual bool hasPowerPrecise(int x, int y, UnitType unitType = UnitTypes::None ) const;
			virtual bool hasPowerPrecise(Position position, UnitType unitType = UnitTypes::None) const;

			virtual bool canBuildHere(TilePosition position, UnitType type, const Unit* builder = NULL, bool checkExplored = false);
			virtual bool canMake(UnitType type, const Unit* builder = NULL);
			virtual bool canResearch(TechType type, const Unit* unit = NULL);
			virtual bool canUpgrade(UpgradeType type, const Unit* unit = NULL);
			virtual std::set< TilePosition >& getStartLocations();

			virtual void printf(const char* format, ...);
			virtual void sendText(const char* format, ...);
			virtual void sendTextEx(bool toAllies, const char *format, ...);

			virtual void changeRace(BWAPI::Race race);
			virtual bool isInGame();
			virtual bool isMultiplayer();
			virtual bool isBattleNet();
			virtual bool isPaused();
			virtual bool isReplay();

			virtual void startGame();
			virtual void pauseGame();
			virtual void resumeGame();
			virtual void leaveGame();
			virtual void restartGame();
			virtual void setLocalSpeed(int speed = -1);
			virtual bool issueCommand(const Unitset& units, UnitCommand command);
			virtual const Unitset& getSelectedUnits();
			virtual Player* self();
			virtual Player* enemy();
			virtual Player* neutral();
			virtual Playerset& allies();
			virtual Playerset& enemies();
			virtual Playerset& observers();

			virtual void setTextSize(int size = 1);
			virtual void drawText(int ctype, int x, int y, const char *format, ...);
			virtual void drawTextMap(int x, int y, const char *format, ...);
			virtual void drawTextMouse(int x, int y, const char *format, ...);
			virtual void drawTextScreen(int x, int y, const char *format, ...);

			virtual void drawBox(int ctype, int left, int top, int right, int bottom, Color color, bool isSolid = false);
			virtual void drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid = false);
			virtual void drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid = false);
			virtual void drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid = false);

			virtual void drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
			virtual void drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
			virtual void drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
			virtual void drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);

			virtual void drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid = false);
			virtual void drawCircleMap(int x, int y, int radius, Color color, bool isSolid = false);
			virtual void drawCircleMouse(int x, int y, int radius, Color color, bool isSolid = false);
			virtual void drawCircleScreen(int x, int y, int radius, Color color, bool isSolid = false);

			virtual void drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
			virtual void drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
			virtual void drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
			virtual void drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);

			virtual void drawDot(int ctype, int x, int y, Color color);
			virtual void drawDotMap(int x, int y, Color color);
			virtual void drawDotMouse(int x, int y, Color color);
			virtual void drawDotScreen(int x, int y, Color color);

			virtual void drawLine(int ctype, int x1, int y1, int x2, int y2, Color color);
			virtual void drawLineMap(int x1, int y1, int x2, int y2, Color color);
			virtual void drawLineMouse(int x1, int y1, int x2, int y2, Color color);
			virtual void drawLineScreen(int x1, int y1, int x2, int y2, Color color);

			virtual void *getScreenBuffer();
			virtual int	getLatencyFrames();
			virtual int	getLatencyTime();
			virtual int	getRemainingLatencyFrames();
			virtual int	getRemainingLatencyTime();
			virtual int	getRevision();
			virtual bool isDebug();
			virtual bool isLatComEnabled();
			virtual void setLatCom(bool isEnabled);
			virtual bool isGUIEnabled();
			virtual void setGUI(bool enabled = true);
			virtual int	getInstanceNumber();
			virtual int	getAPM(bool includeSelects = false);
			virtual bool setMap(const char *mapFileName);
			virtual void setFrameSkip(int frameSkip = 1);
			virtual bool hasPath(Position source, Position destination) const;
			virtual bool setAlliance(BWAPI::Player *player, bool allied = true, bool alliedVictory = true);
			virtual bool setVision(BWAPI::Player *player, bool enabled = true);
			virtual int	elapsedTime() const;
			virtual void setCommandOptimizationLevel(int level = 2);
			virtual int	countdownTimer() const;
			virtual const Regionset &getAllRegions() const;
			virtual BWAPI::Region *getRegionAt(int x, int y) const;
			virtual BWAPI::Region *getRegionAt(BWAPI::Position position) const;
			virtual int getLastEventTime() const;
			virtual bool setReplayVision(BWAPI::Player *player, bool enabled = true);
			virtual bool setRevealAll(bool reveal = true);
	};
}
