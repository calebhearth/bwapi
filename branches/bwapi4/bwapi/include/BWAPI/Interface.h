#pragma once
#include <map>

namespace BWAPI
{
  /// This generalized class allows the application of features that are common to all interface
  /// classes.
  class Interface
  {
  protected:
    Interface() {};
    virtual ~Interface() {};

    std::map<int,void*> clientInfo;
  public:
    /// Retrieves a pointer or value at an index that was stored for this interface using
    /// setClientInfo.
    ///
    /// @param key
    ///   The key containing the value to retrieve. Default is 0.
    /// 
    /// @retval nullptr if index is out of bounds.
    ///
    /// @returns A pointer to the client info at that index.
    ///
    /// @see setClientInfo
    void *getClientInfo(int key = 0) const;

    template <typename T>
    T getClientInfo(int key = 0) const
    {
      return (T)(int)this->getClientInfo(key);
    };

    /// Associates one or more pointers or values with any BWAPI interface.
    ///
    /// This client information is managed entirely by the AI module. It is not modified by BWAPI.
    /// If a pointer to allocated memory is used, then the AI module is responsible for
    /// deallocating the memory when the game ends.
    ///
    /// If client info at the given index has already been set, then it will be overwritten.
    ///
    /// @param clientinfo
    ///   The data to associate with this interface.
    /// @param key
    ///   The key to use for this data. Default is 0.
    ///
    /// @see getClientInfo
    template < typename V >
    void setClientInfo(const V &clientInfo, int key = 0)
    {
      this->clientInfo[key] = (void*)clientInfo;
    };

  };


}
