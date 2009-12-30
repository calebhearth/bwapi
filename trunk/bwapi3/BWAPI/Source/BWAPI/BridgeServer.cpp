#include "BridgeServer.h"

#include <Bridge\Constants.h>
#include <Bridge\PipeMessage.h>
#include <Bridge\EventEntry.h>

#include <Util\Version.h>
#include <Util\Strings.h>
#include <Util\TypedPacketSwitch.h>

namespace BWAPI
{
  // see it as a singleton class.
  namespace BridgeServer
  {
  //private:
    //-------------------------- PRIVATE VARIABLES ----------------------------------------------
    bool stateSharedMemoryInitialized = false;
    bool stateConnectionEstablished = false;
    bool stateIsInitialized = false;
    Bridge::SharedStuff sharedStuff;

    //error handling
    std::string lastError;
    void resetError()
    {
      lastError = "no bridge error";
    }
    const std::string &getLastError()
    {
      return lastError;
    }
    //-------------------------- ----------------------------------------------------------------
  //public:
    //-------------------------- PUBLIC DATA ----------------------------------------------------
    BWAPI::StaticGameData* sharedStaticData = NULL;
    //-------------------------- INIT -----------------------------------------------------------
    bool initConnectionServer()
    {
      resetError();
      sharedStuff.pipe.create(Bridge::globalPipeName);
      stateIsInitialized = true;
      return true;
    }
    //-------------------------- CHECK INCOMING CONNECTIONS -------------------------------------
    void acceptIncomingConnections()
    {
      resetError();
      if(stateConnectionEstablished)
        return;

      // check for incoming pipe connections
      if(!sharedStuff.pipe.pollIncomingConnection())
        return;

      // receive handshake
      Bridge::PipeMessage::AgentHandshake handshake;
      {
        Util::Buffer data;
        sharedStuff.pipe.receive(data);
        data.getMemory().readTo(handshake);
      }

      // audit agent
      bool accept = true;
      if(handshake.agentVersion != SVN_REV)
      {
        accept = false;
        lastError = "Wrong agent version, rejected";
      }
      if(!sharedStuff.remoteProcess.acquire(handshake.agentProcessId, true))
      {
        accept = false;
        lastError = "Could not open agent's process, rejected";
      }

      // send back response
      Bridge::PipeMessage::ServerHandshake handshake2;
      handshake2.accepted = accept;
      handshake2.serverProcessHandle = sharedStuff.remoteProcess.exportOwnHandle();
      handshake2.serverVersion = SVN_REV;
      sharedStuff.pipe.sendRawStructure(handshake2);

      // agent screw'd it, beat it
      if(!accept)
      {
        throw GeneralException(lastError);
      }

      // wait for response acknoledgement
      Bridge::PipeMessage::AgentHandshakeAcknoledge ack;
      {
        Util::Buffer data;
        sharedStuff.pipe.receive(data);
        data.getMemory().readTo(ack);
      }
      if(!ack.accepted)
      {
        throw GeneralException("Agent has rejected response (could not open process handle?)");
      }

      stateConnectionEstablished = true;
    }
    //-------------------------- DISCONNECT -----------------------------------------------------
    void disconnect()
    {
      if(stateSharedMemoryInitialized)
      {
        releaseSharedMemory();
      }
      sharedStuff.pipe.disconnect();
      stateConnectionEstablished = false;
    }
    //-------------------------- CREATE SHARED MEMORY -------------------------------------------
    bool createSharedMemory()
    {
      // check prerequisites
      if(!stateConnectionEstablished)
      {
        lastError = std::string(__FUNCTION__)+": connection not established";
        return false;
      }
      resetError();
      stateSharedMemoryInitialized = false;

      // create and publish static data
      sharedStuff.staticData.create();
      sharedStaticData = &sharedStuff.staticData.get();

      // init dynamic objects
      sharedStuff.userInput.init(1000, true);
      sharedStuff.knownUnits.init(100, true);
      sharedStuff.events.init(2000, true);

      stateSharedMemoryInitialized = true;
      return true;
    }

    //-------------------------- STOP MATCH -----------------------------------------------------
    bool releaseSharedMemory()
    {
      stateSharedMemoryInitialized = false;
      return true;
    }
    //-------------------------- INVOKE ON START MATCH ------------------------------------------
    void invokeOnStartMatch(bool fromBeginning)
    {
      // check prerequisites
      if(!stateConnectionEstablished)
      {
        throw GeneralException(__FUNCTION__ ": connection not established");
      }
      resetError();

      // send onStartMatch event
      {
        // create packet
        Bridge::PipeMessage::ServerMatchInit startMatchEvent;

        startMatchEvent.fromBeginning = fromBeginning;

        startMatchEvent.staticGameDataExport = sharedStuff.staticData.exportToProcess(sharedStuff.remoteProcess, true);

        // pushlish the shared memory location
        sharedStuff.pipe.sendRawStructure(startMatchEvent);
      }

      // wait untill event is done
      Util::Buffer buffer;
      sharedStuff.pipe.receive(buffer);

      // audit completion
      Util::MemoryFrame packet = buffer.getMemory();
      int packetType = packet.getAs<int>();
      if(packetType != Bridge::PipeMessage::AgentMatchInitDone::_typeId)
      {
        throw GeneralException(Util::Strings::ssprintf(__FUNCTION__ ": received unexpected packet type '%d'", packetType));
      }

      Bridge::PipeMessage::AgentMatchInitDone initMatchDone;
      packet.readTo(initMatchDone);

      initMatchDone;  // yet no data to read

      stateSharedMemoryInitialized = true;
    }
    //-------------------------- UPDATE PACKET HANDLERS -----------------------------------------
    bool handleUpdateCommands(Bridge::PipeMessage::AgentUpdateCommands& packet)
    {
      sharedStuff.commands.importNextUpdate(packet.exp);
      return true;
    }
    bool handleUpdateSendText(Bridge::PipeMessage::AgentUpdateSendText& packet)
    {
      sharedStuff.sendText.importNextUpdate(packet.exp);
      return true;
    }
    bool handleUpdateDrawShapes(Bridge::PipeMessage::AgentUpdateDrawShapes& packet)
    {
      sharedStuff.drawShapes.importNextUpdate(packet.exp);
      return true;
    }
    //-------------------------- NEXT FRAME COMPLETION PACKET HANDLER ---------------------------
    bool handleFrameNextDone(Bridge::PipeMessage::AgentFrameNextDone& packet)
    {
      return false;
    }
    //-------------------------- INVOKE ON FRAME ------------------------------------------------
    void invokeOnFrame()
    {
      // packet handlers have bool (wait for next packet) as return type
      static Util::TypedPacketSwitch<bool> packetSwitch;
      if(!packetSwitch.getHandlerCount())
      {
        // init packet switch
        packetSwitch.addHandler(handleUpdateCommands);
        packetSwitch.addHandler(handleUpdateSendText);
        packetSwitch.addHandler(handleUpdateDrawShapes);
        packetSwitch.addHandler(handleFrameNextDone);
      }

      // check prerequisites
      if(!stateConnectionEstablished)
        throw GeneralException(__FUNCTION__ ": no connection");

      // send next frame invocation packet
      Bridge::PipeMessage::ServerFrameNext nextFrame;
      nextFrame; // no data yet
      sharedStuff.pipe.sendRawStructure(nextFrame);

      // wait untill completion packet received
      bool waitForNextPacket = true;
      while(waitForNextPacket)
      {
        // receive completion notification
        Util::Buffer buffer;
        sharedStuff.pipe.receive(buffer);

        // get packet type
        waitForNextPacket = packetSwitch.handlePacket(buffer.getMemory());
      }

      // just returned from frame invokation
      // clear frame-to-frame buffers
      sharedStuff.userInput.clear();
      sharedStuff.events.clear();
    }
    //------------------------------ PUSH SEND TEXT ----------------------------------------------
    bool pushSendText(const char *text)
    {
      // check prerequisites
      if(!stateSharedMemoryInitialized)
      {
        lastError = std::string(__FUNCTION__)+ ": shared memory not initialized";
        return false;
      }

      Util::Buffer userInputEntry;
      int stringLength = ::strlen(text) + 1; // including terminal NULL
      userInputEntry.append(Util::MemoryFrame((char*)text, stringLength));
      sharedStuff.userInput.insert(userInputEntry.getMemory());

      return true;
    }
    //------------------------------ ADD KNOWN UNIT ----------------------------------------------
    void addKnownUnit(Bridge::KnownUnitEntry **out_pKnownUnit, Bridge::SharedStuff::KnownUnitSet::Index *out_index, BWAPI::UnitAddEventTypeId reason)
    {
      // check prerequisites
      if(!stateSharedMemoryInitialized)
        throw GeneralException(__FUNCTION__ ": shared memory not initialized");

      // insert new known unit to set
      Bridge::SharedStuff::KnownUnitSet::Index index;
      *out_pKnownUnit = &sharedStuff.knownUnits.insertEmpty(&index);
      if(out_index)
        *out_index = index;

      // push known unit event
      Bridge::EventEntry::KnownUnitAdd entry;
      entry.data.unitId = sharedStuff.knownUnits.getLinearByIndex(index);
      entry.data.type = reason;
      sharedStuff.events.insert(Util::MemoryFrame::from(entry));
    }
    //------------------------------ REMOVE KNOWN UNIT -------------------------------------------
    void removeKnownUnit(Bridge::SharedStuff::KnownUnitSet::Index index, BWAPI::UnitRemoveEventTypeId reason)
    {
      // check prerequisites
      if(!stateSharedMemoryInitialized)
        throw GeneralException(__FUNCTION__ ": shared memory not initialized");

      // remove known unit from set
      sharedStuff.knownUnits.remove(index);

      // push known unit event
      Bridge::EventEntry::KnownUnitRemove entry;
      entry.data.unitId = sharedStuff.knownUnits.getLinearByIndex(index);
      entry.data.type = reason;
      sharedStuff.events.insert(Util::MemoryFrame::from(entry));
    }
    //-------------------------- GET SEND TEXT ENTRIES ------------------------------------------
    std::deque<Bridge::SendTextEntry*> getSendTextEntries()
    {
      std::deque<Bridge::SendTextEntry*> retval;
      Bridge::SharedStuff::SendTextStack::Index i = sharedStuff.sendText.begin();
      while(i.isValid())
      {
        retval.push_back(sharedStuff.sendText.get(i).beginAs<Bridge::SendTextEntry>());
        i = sharedStuff.sendText.getNext(i);
      }
      return retval;
    }
    //-------------------------- UPDATE REMOTE SHARED MEMORY ------------------------------------
    bool updateRemoteSharedMemory()
    {
      // export userInput updates
      while(sharedStuff.userInput.isUpdateExportNeeded())
      {
        // create export package
        Bridge::PipeMessage::ServerUpdateUserInput packet;
        if(!sharedStuff.userInput.exportNextUpdate(packet.exp, sharedStuff.remoteProcess))
        {
          lastError = std::string(__FUNCTION__)+ ": exporting userInput update failed";
          return false;
        }

        // send update export
        sharedStuff.pipe.sendRawStructure(packet);
      }

      // export knownUnits updates
      while(sharedStuff.knownUnits.isUpdateExportNeeded())
      {
        // create export package
        Bridge::PipeMessage::ServerUpdateKnownUnits packet;
        if(!sharedStuff.knownUnits.exportNextUpdate(packet.exp, sharedStuff.remoteProcess))
        {
          lastError = std::string(__FUNCTION__)+ ": exporting knownUnits update failed";
          return false;
        }

        // send update export
        sharedStuff.pipe.sendRawStructure(packet);
      }

      // export events updates
      while(sharedStuff.events.isUpdateExportNeeded())
      {
        // create export package
        Bridge::PipeMessage::ServerUpdateEvents packet;
        if(!sharedStuff.events.exportNextUpdate(packet.exp, sharedStuff.remoteProcess))
        {
          lastError = std::string(__FUNCTION__)+ ": exporting events update failed";
          return false;
        }

        // send update export
        sharedStuff.pipe.sendRawStructure(packet);
      }

      return true;
    }
    //-------------------------- GET ALL DRAW SHAPES --------------------------------------------
    void enumAllDrawShapes(DrawShapeCallback enumCallback)
    {
      if(!enumCallback)
        return;

      // iterate over all shapes
      Bridge::SharedStuff::DrawShapeStack::Index index = sharedStuff.drawShapes.begin();
      while(index.isValid())
      {
        // the sharedmemory is not visible outside of the BridgeServer
        enumCallback(sharedStuff.drawShapes.get(index));
        index = sharedStuff.drawShapes.getNext(index);
      }
    }
    //-------------------------- IS AGENT CONNECTED ---------------------------------------------
    bool isAgentConnected()
    {
      return stateConnectionEstablished;
    }
    //-------------------------- IS BRIDGE INITIALIZED ------------------------------------------
    bool isBridgeInitialized()
    {
      return stateIsInitialized;
    }
    //-------------------------- IS SHARED MEMORY INITIALIZED -----------------------------------
    bool isSharedMemoryInitialized()
    {
      return stateSharedMemoryInitialized;
    }
    //-------------------------- ----------------------------------------------------------------
  }
}
