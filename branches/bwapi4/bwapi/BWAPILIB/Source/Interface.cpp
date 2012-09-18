#include <BWAPI/Interface.h>

namespace BWAPI
{
  void *Interface::getClientInfo(int index) const
  {
    // Retrieve iterator to element at index
    std::map<int,void*>::const_iterator result = this->clientInfo.find(index);

    // Return a default value if not found
    if ( result == this->clientInfo.end() )
      return nullptr;

    // return the desired value
    return result->second;
  }

}
