#pragma once

namespace BW    { class  Unit; }
namespace BW    { struct Bullet; }
namespace BWAPI { class  Player; }
namespace BWAPI { class  PlayerImpl; }
namespace BWAPI { class  Unit; }
namespace BWAPI { class  UnitImpl; }
namespace BWAPI { class  Bullet; }
namespace BWAPI { class  BulletImpl; }
namespace BWAPI { class  Command; }
namespace BWAPI { class  AIModule; }

#include <BWAPI/AIModule.h>
#include <BWAPI/Game.h>
#include <BWAPI/Server.h>
#include <BWAPI/Map.h>
#include <BWAPI/Client/Shape.h>
#include <BWAPI/Client/GameData.h>
#include <BWAPI/Unitset.h>
#include <BWAPI/Bulletset.h>
#include <BWAPI/Playerset.h>

#include <BW/Dialog.h>
#include <BW/OrderTypes.h>

/**
 * Everything in the BWAPI library that doesn't map or work directly with the bw
 * data.
 */
namespace BWAPI
{
  /** The main class wrapping the whole game data/methods. */
  class GameImpl : public Game
  {
    public :
      virtual Forceset&  getForces();
      virtual Playerset& getPlayers();
      virtual Unitset&   getAllUnits();
      virtual Unitset&   getMinerals();
      virtual Unitset&   getGeysers();
      virtual Unitset&   getNeutralUnits();
 
      virtual Unitset&   getStaticMinerals();
      virtual Unitset&   getStaticGeysers();
      virtual Unitset&   getStaticNeutralUnits();

      virtual Bulletset& getBullets();
      virtual std::set< Position >& getNukeDots();
      virtual std::list< Event >&  getEvents();

      virtual Force   *getForce(int forceID);
      virtual Player  *getPlayer(int playerID);
      virtual Unit    *getUnit(int unitID);
      virtual Unit    *indexToUnit(int unitIndex);
      virtual Region  *getRegion(int regionID);

      virtual GameType getGameType();

      virtual int      getLatency();
      virtual int      getFrameCount();
      virtual int      getReplayFrameCount();
      virtual int      getFPS();
      virtual double   getAverageFPS();

      virtual Position getMousePosition();
      virtual bool     getMouseState(MouseButton button);
      virtual bool     getMouseState(int button);
      virtual bool     getKeyState(Key key);
      bool             getKeyState(int key);

      virtual Position getScreenPosition();
      virtual void     setScreenPosition(int x, int y);
      virtual void     setScreenPosition(BWAPI::Position p);

      virtual void     pingMinimap(int x, int y);
      virtual void     pingMinimap(BWAPI::Position p);

      virtual bool     isFlagEnabled(int flag);
      virtual void     enableFlag(int flag);

      virtual Unitset& getUnitsOnTile(int x, int y);
      virtual Unitset& getUnitsInRectangle(int left, int top, int right, int bottom) const;
      virtual Unitset& getUnitsInRectangle(BWAPI::Position topLeft, BWAPI::Position bottomRight) const;
      virtual Unitset& getUnitsInRadius(BWAPI::Position center, int radius) const;

      virtual Error       getLastError() const;
      virtual bool        setLastError(BWAPI::Error e);

      virtual int         mapWidth();
      virtual int         mapHeight();
      virtual std::string mapFileName();
      virtual std::string mapPathName();
      virtual std::string mapName();
      virtual std::string mapHash();

      virtual bool isWalkable(int x, int y);
      virtual bool isWalkable(WalkPosition position);
      virtual int  getGroundHeight(int x, int y);
      virtual int  getGroundHeight(TilePosition position);
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

      virtual bool canBuildHere(const Unit* builder, TilePosition position, UnitType type, bool checkExplored = false);
      virtual bool canMake(const Unit* builder, UnitType type);
      virtual bool canResearch(const Unit* unit, TechType type);
      virtual bool canUpgrade(const Unit* unit, UpgradeType type);

      virtual std::set< TilePosition >& getStartLocations();

      /**
       * Prints text in game (only local)
       * @param format Text to be written
       */
      virtual void printf(const char *format, ...);
      virtual void sendText(const char *format, ...);
      virtual void sendTextEx(bool toAllies, const char *format, ...);

      /**
       * Changes race in the pre-game lobby.
       * @param race Desired race of the slot (Zerg/Protoss/Terran/Random)
       */
      virtual void changeRace(BWAPI::Race race);
      virtual bool isInGame();
      virtual bool isMultiplayer();
      virtual bool isBattleNet();
      virtual bool isPaused();
      virtual bool isReplay();

      /**
       * Starts the game in the pre-game lobby. Should be used only in the
       * pre-game lobby, and not during counting
       */
      virtual void startGame();
      virtual void pauseGame();
      virtual void resumeGame();
      virtual void leaveGame();
      virtual void restartGame();
      virtual void setLocalSpeed(int speed = -1);
      virtual bool issueCommand(const Unitset& units, UnitCommand command);
      virtual const Unitset& getSelectedUnits();
      virtual Player *self();
      virtual Player *enemy();
      virtual Player *neutral();
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

      virtual int getLatencyFrames();
      virtual int getLatencyTime();
      virtual int getRemainingLatencyFrames();
      virtual int getRemainingLatencyTime();

      virtual int  getRevision();
      virtual bool isDebug();
      virtual bool isLatComEnabled();
      virtual void setLatCom(bool isEnabled);
      virtual bool isGUIEnabled();
      virtual void setGUI(bool enabled = true);

      virtual int getInstanceNumber();
      virtual int getAPM(bool includeSelects = false);

      virtual bool setMap(const char *mapFileName);
      virtual void setFrameSkip(int frameSkip = 1);

      virtual bool hasPath(Position source, Position destination) const;

      virtual bool setAlliance(BWAPI::Player *player, bool allied = true, bool alliedVictory = true);
      virtual bool setVision(BWAPI::Player *player, bool enabled = true);
      virtual int  elapsedTime() const;

      virtual void setCommandOptimizationLevel(int level = 2);
      virtual int  countdownTimer() const;

      virtual const Regionset &getAllRegions() const;
      virtual BWAPI::Region *getRegionAt(int x, int y) const;
      virtual BWAPI::Region *getRegionAt(BWAPI::Position position) const;

      virtual int getLastEventTime() const;

      virtual bool setReplayVision(BWAPI::Player *player, bool enabled = true);
      virtual bool setRevealAll(bool reveal = true);

      //Internal BWAPI commands:
      GameImpl();
      ~GameImpl();

      void update(); /**< Updates unitArrayCopy according to bw memory */
      void loadAutoMenuData();
      void onMenuFrame();
      PlayerImpl *_getPlayer(int id);
      int _currentPlayerId();
      void pressKey(int key);
      void mouseDown(int x, int y);
      void mouseUp(int x, int y);

      /**
       * Changes slot state in the pre-game lobby.
       * @param slot Desired state of the slot (Open/Closed/Computer)
       * @param slotID Order of the slot (0 based)
       */
      void changeSlot(BW::Orders::ChangeSlot::Slot slot, u8 slotID);
      void addToCommandBuffer(Command* command);
      void onGameStart();
      void onGameEnd();
      int  stormIdToPlayerId(int dwStormId);
      void onSendText(const char* text);
      void onReceiveText(int playerId, std::string text);
      bool parseText(const char* text);
      bool inScreen(int ctype, int x, int y);
      bool inScreen(int ctype, int x1, int y1, int x2, int y2);
      bool inScreen(int ctype, int x1, int y1, int x2, int y2, int x3, int y3);
      void lockFlags();
      bool enabled;
      bool _isInGame() const;
      bool _isSinglePlayer() const;
      bool _isBattleNet();
      bool _isReplay() const;
      void _changeRace(int slot, BWAPI::Race race);
      int _getLobbyRace(int slot);
      Race lastKnownRaceBeforeStart[8];

      void loadSelected();
      void copyMapToSharedMemory();
      void moveToSelectedUnits();
      void executeCommand(UnitCommand command, bool addCommandToLatComBuffer = true);
      bool addToCommandOptimizer(UnitCommand command);

      void chooseNewRandomMap();
      static void SendClientEvent(BWAPI::AIModule *module, Event &e);

      UnitImpl   *getUnitFromIndex(int index);
      BulletImpl *getBulletFromIndex(int index);
      PlayerImpl *BWAPIPlayer;
      PlayerImpl *enemyPlayer;

      std::set<int> invalidIndices;
      std::list<std::string > sentMessages;
      void onSaveGame(char *name);
      std::list<Event> events;
      int bulletCount;
      BW::dialog *myDlg;
      Server server;
      Unitset lastEvadedUnits;
      bool onStartCalled;
      std::string lastMapGen;
      bool outOfGame;
      std::string autoMenuMode;
      std::string autoMenuMapPath;
      std::string autoMenuGameName;
      bool isHost;
      int  autoMapTryCount;
      std::vector<std::string> autoMapPool;
      std::string autoMapIteration;
      std::string lastAutoMapString;
      unsigned int lastAutoMapEntry;

      /** Count of game-frames passed from game start. */
      int frameCount;
      void refreshSelectionStates();
      BW::Unit *savedUnitSelection[12];
      bool wantSelectionUpdate;

      bool startedClient;

      UnitImpl *unitArray[UNIT_ARRAY_MAX_LENGTH];
      bool isTournamentCall;
      const GameData* getGameData() const;
      GameData* data;
      
      int drawShapes();
      void processEvents();
      Unit *_unitFromIndex(int index);

      int  commandOptimizerLevel;

      HMODULE hAIModule;
      AIModule *client;

      HMODULE hTournamentModule;
      AIModule *tournamentAI;

      PlayerImpl *players[PLAYER_COUNT];
    private :
      Map map;

      Unitset aliveUnits; //units alive on current frame
      Unitset dyingUnits; //units leaving aliveUnits set on current frame

      Unitset discoverUnits; //units entering accessibleUnits set on current frame
      Unitset accessibleUnits; //units that are accessible to the client on current frame
      Unitset evadeUnits; //units leaving accessibleUnits set on current frame

      Unitset  selectedUnitSet;
      Unitset emptyUnitset;

      std::set<TilePosition>  startLocations;

      Forceset  forces;
      Playerset playerSet;

      Unitset   minerals;
      Unitset   geysers;
      Unitset   neutralUnits;
      Bulletset bullets;
      std::set<Position>       nukeDots;
      Unitset pylons;
      Util::RectangleArray< Unitset > unitsOnTileData;

      Unitset staticMinerals;
      Unitset staticGeysers;
      Unitset staticNeutralUnits;

      Regionset regionsList;

      BulletImpl* bulletArray[BULLET_ARRAY_MAX_LENGTH];
      std::vector<std::vector<Command *> > commandBuffer;
      /** Will update the unitsOnTile content, should be called every frame. */
      void updateUnits();
      void updateBullets();
      void computeUnitExistence();
      void computePrimaryUnitSets();
      void extractUnitData();
      void augmentUnitData();
      void applyLatencyCompensation();
      void computeSecondaryUnitSets();
      /**
       * Specifies if some order was given, so the loadSelect function will have
       * to be called.
       */
      std::string savedMapHash;
      bool flags[BWAPI::Flag::Max];
      TournamentModule  *tournamentController;
      bool              bTournamentMessageAppeared;
      BWAPI::Error lastError;
      Unitset deadUnits;
      u32 cheatFlags;
      std::string autoMenuLanMode;
      std::string autoMenuRace;
      std::string autoMenuEnemyRace[8];
      unsigned int autoMenuEnemyCount;
      unsigned int autoMenuMinPlayerCount;
      unsigned int autoMenuMaxPlayerCount;
      unsigned int autoMenuWaitPlayerTime;
      std::string autoMenuGameType;
      std::string autoMenuRestartGame;
      std::string autoMenuSaveReplay;
#ifdef _DEBUG
      std::string autoMenuPause;
#endif
      std::string rn_BWAPIName;
      std::string rn_BWAPIRace;
      std::string rn_MapName;
      std::string rn_AlliesNames;
      std::string rn_AlliesRaces;
      std::string rn_EnemiesNames;
      std::string rn_EnemiesRaces;
      DWORD endTick;
      Playerset _allies;
      Playerset _enemies;
      Playerset _observers;

      bool inGame;
      bool actStartedGame;
      bool actRaceSel;

      DWORD   startTickCount;
      DWORD   lastTickCount;
      int     accumulatedFrames;
      int     fps;
      double  averageFPS;
      int     botAPM_noselects;
      int     botAPM_selects;
      double  botAPMCounter_noselects;
      double  botAPMCounter_selects;

      int  textSize;

      bool pathDebug;
      bool unitDebug;
      bool grid;

      bool externalModuleConnected;
      bool calledMatchEnd;
      bool tournamentCheck(int type, void *parameter = NULL);
      std::vector<UnitCommand> commandOptimizer[BWAPI_UNIT_COMMAND_TYPE_COUNT];

      int lastEventTime;
      int addShape(const BWAPIC::Shape &s);
      int addString(const char* text);
      int addText(BWAPIC::Shape &s, const char* text);

  };
  /**
   * Broodwar is, and always should be the ONLY instance of the Game class, it is singleton.
   * As there is only one instance, the class is defined globaly and as value (not pointer), so the instance
   * is constructed automatically and there is also no need to care about the destructor.
   * We wanted to save passing the Game parameter everywhere, so we expect everywhere in the code that this
   * variable is instantialised.
   */
  extern GameImpl BroodwarImpl;
};
