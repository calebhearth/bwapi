#include "BridgeClient.h"
#include "Mappings.h"
#include "Unit.h"

#include <Bridge\SharedStuff.h>
#include <Bridge\PipeMessage.h>
#include <Bridge\Constants.h>
#include <Bridge\Doodle.h>
#include <Util\Version.h>
#include <Util\RemoteProcess.h>
#include <Util\Pipe.h>
#include <Util\SharedMemory.h>
#include <Util\SharedSet.h>

namespace BWAgent
{
  // singleton class
  namespace BridgeClient
  {
  //private:
    Bridge::SharedStuff sharedStuff;

    // Bridge Client state
    bool connectionEstablished = false;
    BridgeState bridgeState = Intermediate;

    // error handling
    std::string lastError;
    void resetError()
    {
      lastError = "no error";
    }

  //public:
    //----------------------------------------- PUBLIC DATA -----------------------------------------------------
    // public access to shared memory
    Bridge::StaticGameDataStructure* sharedStaticData;
    //----------------------------------------- CONNECT ---------------------------------------------------------
    int connect()
    {
      resetError();
      if(connectionEstablished)
      {
        lastError = __FUNCTION__ ": Already connected";
        return true;
      }

      // try to connect
      if(!sharedStuff.pipe.connect(Bridge::globalPipeName))
      {
        lastError = __FUNCTION__ ": Could not establish pipe connection. Check whether:\n - Broodwar is lauched with BWAPI\n - Another bot is already connected";
        return false;
      }

      // send handshake packet
      {
        Bridge::PipeMessage::AgentHandshake handshake;
        handshake.agentVersion = SVN_REV;
        handshake.agentProcessId = ::GetCurrentProcessId();
        if(!sharedStuff.pipe.sendStructure(handshake))
        {
          lastError = __FUNCTION__ ": Could not send per pipe";
          return false;
        }
      }

      // receive handshake
      {
        Util::Buffer data;
        if(!sharedStuff.pipe.receive(data))
        {
          lastError = __FUNCTION__ ": Could not read pipe";
          return false;
        }
        Bridge::PipeMessage::ServerHandshake handshake;
        if(!data.getMemory().readTo(handshake))
        {
          lastError = __FUNCTION__ ": Handshake failure";
          return false;
        }
        if(!handshake.accepted)
        {
          if(handshake.serverVersion != SVN_REV)
          {
            lastError = __FUNCTION__ ": BWAPI.dll(";
            lastError += handshake.serverVersion;
            lastError += ") and BWAgent.dll(" SVN_REV_STR ") version mismatch";
            return false;
          }
          lastError = __FUNCTION__ ": BWAPI rejected the connection";
          return false;
        }
        // try access the process for handle duplication
        if(!sharedStuff.remoteProcess.importHandle(handshake.serverProcessHandle))
        {
          lastError = __FUNCTION__ ": imported faulty process handle";
          return false;
        }
      }

      // acknoledge handshake
      {
        Bridge::PipeMessage::AgentHandshakeAcknoledge ack;
        ack.accepted = true;
        if(!sharedStuff.pipe.sendStructure(ack))
        {
          lastError = __FUNCTION__ ": Sending AgentHandshakeAcknoledge failed";
          return false;
        }
      }

      // connected
      connectionEstablished = true;

      return true;
    }
    //----------------------------------------- GET CURRENT STATE -----------------------------------------------
    BridgeState getCurrentState()
    {
      return bridgeState;
    }
    void updateMappings()
    {
      allUnits.clear();
      for(int i=0;i<sharedStaticData->unitCount;i++)
      {
        int id=sharedStaticData->unitData[i].getID;
        if (unitIdToObject.find(id)==unitIdToObject.end())
          unitIdToObject[id]=new BWAgent::Unit();
        unitIdToObject[id]->_update(BWAPI::ClearanceLevels::FullyObservable,(BWAPI::State*)&sharedStaticData->unitData[i]);
        allUnits.insert(unitIdToObject[id]);
      }
    }
    //----------------------------------------- WAIT FOR EVENT --------------------------------------------------
    bool waitForEvent()
    {
      resetError();

      // send readyness packet
      bool success = false;
      switch(bridgeState)
      {
      case Intermediate:
        {
          success = true;
        }break;
      case OnFrame:
        {
          Bridge::PipeMessage::Packet<Bridge::PipeMessage::AgentNextFrameDone> done;
          success = sharedStuff.pipe.sendStructure(done);
        }break;
      case OnInitMatch:
        {
          Bridge::PipeMessage::Packet<Bridge::PipeMessage::AgentInitMatchDone> done;
          success = sharedStuff.pipe.sendStructure(done);
        }break;
      }
      if(!success)
      {
        lastError = __FUNCTION__ ": error pipe send.";
        return false;
      }

      // wait and receive something
      Util::Buffer buffer;
      if(!sharedStuff.pipe.receive(buffer))
      {
        lastError = __FUNCTION__ ": error pipe receive.";
        return false;
      }

      // examine received packet
      Util::MemoryFrame packet = buffer.getMemory();
      int packetType;
      if(!packet.readTo(packetType))
      {
        lastError = __FUNCTION__ ": too small packet.";
        return false;
      }

      if(packetType == Bridge::PipeMessage::ServerInitMatch::Id)
      {
        // onInitMatch state
        Bridge::SharedStuff::SharedGameDataStructure::Export staticGameData;
        packet.readTo(staticGameData);
        if (!sharedStuff.staticData.import(staticGameData))
        {
          lastError = __FUNCTION__ ": staticGameData failed importing.";
          return false;
        }
        sharedStaticData = &sharedStuff.staticData.get();
        bridgeState = OnInitMatch;
      }
      else if(packetType == Bridge::PipeMessage::ServerNextFrame::Id)
      {
        // onFrame state
        bridgeState = OnFrame;
        updateMappings();
      }
      else
      {
        lastError = __FUNCTION__ ": unknown packet type " + packetType;
        return false;
      }

      return true;
    }
    //----------------------------------------- GET LAST ERROR --------------------------------------------------
    std::string getLastError()
    {
      return lastError;
    }
    //----------------------------------------- -----------------------------------------------------------------
  }
}
