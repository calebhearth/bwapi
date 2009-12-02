#pragma once

#include <Bridge\StaticGameDataStructure.h>

#include <string>

namespace BWAPI
{
  // see it as a singleton class.
  namespace BridgeServer
  {
  //public:
    extern Bridge::StaticGameDataStructure* sharedStaticData;

    extern bool initConnectionServer();
    extern bool acceptIncomingConnections();
    extern void disconnect();
    extern bool initSharedMemory();
    extern bool releaseSharedMemory();
    extern bool invokeOnFrame();
    extern bool pushSendText();
    extern bool isAgentConnected();               // true when pipe connection works
    extern bool isBridgeInitialized();            // true once initBridge called
    extern bool isSharedMemoryInitialized();      // true once memory allocated
    extern const std::string &getLastError();
  }
}
