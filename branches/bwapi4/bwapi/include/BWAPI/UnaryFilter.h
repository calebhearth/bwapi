#pragma once
#include <functional>

namespace BWAPI
{
  template<typename _Param1>
  class UnaryFilterBase
  {
  private:
    std::function<bool(_Param1)> pred;
  public:
    // Constructor
    template <typename _T>
    UnaryFilterBase(const _T& predicate) : pred(predicate)
    {};

    // Assignment
    template <typename _T>
    UnaryFilterBase &operator =(const _T& other)
    {
      this->pred.assign(other);
      return *this;
    };

    // Bitwise operators    
    template <typename _T>
    inline UnaryFilterBase operator &&(const _T& other) const
    {
      return [&](_Param1 u){ return (*this)(u) && other(u); };
    };
    template <typename _T>
    inline UnaryFilterBase operator ||(const _T& other) const
    {
      return [&](_Param1 u){ return (*this)(u) || other(u); };
    };

    // operator not
    inline UnaryFilterBase operator ~() const
    {
      return std::not1(this->pred);
    };

    // call
    inline bool operator()(_Param1 u) const
    {
      return pred(u);
    };

    // operator bool
    inline operator bool() const
    {
      return (bool)pred;
    };
  };
}

