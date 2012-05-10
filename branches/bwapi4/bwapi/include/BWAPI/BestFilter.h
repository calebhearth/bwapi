#pragma once
#include <functional>

namespace BWAPI
{
  /// @~English
  /// The BestFilter is used to compare two
  /// objects with each other. Each function
  /// object compares two of the same object
  /// and returns the most desirable one.
  ///
  /// @note: A function object should return
  /// one of the two given parameters, never NULL. 
  /// Both parameters being given should never be
  /// NULL.
  /// 
  /// @~
  template<typename _Param1>
  class BestFilterBase
  {
  private:
    std::function<_Param1(_Param1,_Param1)> pred;
  public:
    // Constructor
    template <typename _T>
    BestFilterBase(const _T&& predicate) : pred(predicate)
    {};

    // Assignment
    template <typename _T>
    BestFilterBase &operator =(const _T&& other)
    {
      this->pred.assign(other);
      return *this;
    };

    // Bitwise operators
    template <typename _T>
    inline BestFilterBase operator &&(const _T&& other) const
    {
      return [&](_Param1 p1, _Param1 p2){ return other( (*this)(u) ); };
    };
    template <typename _T>
    inline BestFilterBase operator &&(const _T& other) const
    {
      return [&](_Param1 p1, _Param1 p2){ return other( (*this)(u) ); };
    };

    // operator not
    inline BestFilterBase operator ~() const
    {
      return std::not1(this->pred);
    };

    // call
    inline bool operator()(_Param1 u) const
    {
      return pred(u);
    };

  };

}

