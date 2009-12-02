#include "BridgeAgent.h"

#include <Bridge\SharedStuff.h>
#include <Bridge\PipeMessage.h>
#include <Bridge\Constants.h>
#include "Bridge\Doodle.h"

#include "Util\Version.h"
#include "Util\RemoteProcess.h"
#include "Util\Pipe.h"
#include "Util\SharedMemory.h"
#include "Util\SharedSet.h"

namespace BWAgent
{
  // singleton class
  namespace BridgeAgent
  {
  //private:
    Bridge::SharedStuff sharedStuff;
    bool connectionEstablished;

    // access to shared memory
    Bridge::StaticGameDataStructure* sharedStaticData;

    // error handling
    std::string lastError;
    void resetError()
    {
      lastError = "no error";
    }

  //public:
    int connect()
    {
      resetError();
      if(connectionEstablished)
      {
        lastError = "Already connected";
        return 0;
      }

      // try to connect
      if(!sharedStuff.pipe.connect(Bridge::globalPipeName))
      {
        lastError = "Could not establish pipe connection. Check whether:\n - Broodwar is lauched with BWAPI\n - Another bot is already connected";
        return 0;
      }

      // send handshake packet
      {
        Bridge::PipeMessage::AgentHandshake handshake;
        handshake.agentVersion = SVN_REV;
        handshake.agentProcessId = ::GetCurrentProcessId();
        if(!sharedStuff.pipe.sendStructure(handshake))
        {
          lastError = "Could not send per pipe";
          return 0;
        }
      }

      // receive handshake
      {
        Util::Buffer data;
        if(!sharedStuff.pipe.receive(data))
        {
          lastError = "Could not read pipe";
          return 0;
        }
        Bridge::PipeMessage::HubHandshake handshake;
        if(!data.getMemory().readTo(handshake))
        {
          lastError = "Handshake failure";
          return 0;
        }
        if(!handshake.accepted)
        {
          if(handshake.hubVersion != SVN_REV)
          {
            lastError = "BWAPI.dll(";
            lastError += handshake.hubVersion;
            lastError += ") and BWAgent.dll(" SVN_REV_STR ") version mismatch";
            return 0;
          }
          lastError = "BWAPI rejected the connection";
          return 0;
        }
        // try access the process for handle duplication
        if(!sharedStuff.remoteProcess.importHandle(handshake.hubProcessHandle))
        {
          lastError = "imported faulty process handle";
          return 0;
        }
      }

      // acknoledge handshake
      {
        Bridge::PipeMessage::AgentHandshakeAcknoledge ack;
        ack.accepted = true;
        if(!sharedStuff.pipe.sendStructure(ack))
        {
          lastError = "Sending AgentHandshakeAcknoledge failed";
          return 0;
        }
      }

      // connected
      connectionEstablished = true;

      return 1;
    }

    std::string getLastError()
    {
      return lastError;
    }
  }
}
