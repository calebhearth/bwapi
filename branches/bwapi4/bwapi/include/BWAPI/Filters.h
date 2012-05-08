#pragma once
#include <functional>
#include <BWAPI/Unit.h>

namespace BWAPI
{
  class Filter
  {
  private:
    std::function<bool(Unit*)> pred;
  public:
    // Constructor
    template <typename _T>
    Filter(const _T&& predicate) : pred(predicate)
    {};

    // Assignment
    template <typename _T>
    Filter &operator =(const _T&& other)
    {
      this->pred = other;
      return *this;
    };

    // Bitwise operators
    template <typename _T>
    inline Filter operator &&(const _T&& other) const
    {
      return [&](Unit *u){ return this->operator()(u) && other(u); };
    };
    template <typename _T>
    inline Filter operator &&(const _T& other) const
    {
      return [&](Unit *u){ return this->operator()(u) && other(u); };
    };
    template <typename _T>
    inline Filter operator ||(const _T&& other) const
    {
      return [&](Unit *u){ return this->operator()(u) || other(u); };
    };
    template <typename _T>
    inline Filter operator ||(const _T& other) const
    {
      return [&](Unit *u){ return this->operator()(u) || other(u); };
    };

    // operator not
    inline Filter operator !() const
    {
      return std::not1(this->pred);
    };

    // operator ()
    inline bool operator()(Unit *u) const
    {
      return pred(u);
    };
  };

  extern const Filter Workers;
  extern const Filter Larvae;
  extern const Filter Transports;
  extern const Filter ResourceDepots;
}

