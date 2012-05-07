#pragma once
#include <functional>

namespace BWAPI
{
  class Unit;

  typedef std::function<bool(Unit*)> Filter;
  /*
  class Filter
  {
  private:
    std::function<bool(Unit*)> pred;
  public:
    Filter(std::function<bool(Unit*)> inFunction) : pred(inFunction) {};
    Filter(const Filter &other) : pred(other) {};

    inline Filter &operator=(const Filter &other)
    {
      this->pred = other.pred;
      return *this;
    };

    // OPERATORS
    inline Filter operator &&(const Filter &other) const
    {
      return [&](Unit *u){ return this->operator()(u) && other(u); };
    };
    inline Filter operator ||(Filter other) const
    {
      return [&](Unit *u){ return this->operator()(u) || other(u); };
    };
    inline Filter operator !() const
    {
      return [&](Unit *u){ return !this->operator()(u); };
    };
    bool operator()(Unit *u) const
    {
      return pred.operator()(u);
    };
  };*/

  /*
  class Filter : public std::function<bool(Unit*)>
  {
  public:
    // FORWARD CONSTRUCTORS (pass directly to all function ctors)
    Filter() : function() {};
    Filter(const _Myt& _Right) : function(_Right) {};
	  
    template<class _Fx> 
    Filter(_Fx _Func _NOT_INTEGRAL(_Fx)) : function(_Func) {};
	  
    template<class _Fx, class _Alloc>
    Filter(_Fx _Func _NOT_INTEGRAL(_Fx), const _Alloc& _Ax) : function(_Func,_Ax) {};

    template<class _Fx>
    Filter(std::tr1::reference_wrapper<_Fx> _Func) : function(_Func) {};

	  template<class _Fx, class _Alloc>
    Filter(std::tr1::reference_wrapper<_Fx> _Func, const _Alloc& _Ax) : function(_Func, _Ax) {};

    Filter(std::tr1::_Unutterable u) : function(u) {};
 #if defined(_NATIVE_NULLPTR_SUPPORTED) && !defined(_DO_NOT_USE_NULLPTR_IN_STL)
    Filter(int i) : function(i) {};
 #endif

    // OPERATORS
    inline Filter &operator &&(const Filter &other)
    {
      *this = [&](Unit *u){ return this->operator()(u) && other(u); };
      return *this;
    };
    inline Filter &operator ||(Filter other)
    {
      *this = [&](Unit *u){ return this->operator()(u) || other(u); };
      return *this;
    };
    inline Filter &operator !()
    {
      *this = [&](Unit *u){ return !this->operator()(u); };
      return *this;
    };

  };
*/
  extern Filter Workers;
  extern Filter Transports;
  extern Filter ResourceDepots;

  /*
  template <typename _pred>
  class Filter
  {
  public:
    Filter(){};

    std::function<bool(Unit*)> operator &&( const Filter &other)
    {
      return [](Unit *u){ return _pred(u) && other(u); };
    };
    std::function<bool(Unit*)> operator ||( const Filter &other)
    {
      return [](Unit *u){ return _pred(u) || other(u); };
    };

    bool operator ()(Unit *u)
    {
      return _pred(u);
    };

  };*/


}
/*
BWAPI::Filter operator&&(const BWAPI::Filter &p1, const BWAPI::Filter &p2);
BWAPI::Filter operator||(const BWAPI::Filter &p1, const BWAPI::Filter &p2);
BWAPI::Filter operator~(const BWAPI::Filter &p1);
*/

