#pragma once
#include "Vectorset.h"

#define BWAPI_TYPESET(setname,name,...) static const name setname ## struc[] = { __VA_ARGS__ };                    \
                            static const name::set setname(setname ## struc, sizeof(setname ## struc)/sizeof(setname ## struc[0]));

namespace BWAPI
{
  template<class _T>
  class Typeset : public Vectorset<_T>
  {
  public:
    // Constructors
    Typeset() : Vectorset() {};
    // copy ctor
    Typeset(const Typeset<_T> &other) : Vectorset(other) {};
    // type ctor
    Typeset(const _T &val) : Vectorset() { this->push_back(val); };
    // array ctor
    Typeset(const _T *pArray, size_t size) : Vectorset(pArray, size) {};
    Typeset(const int *pArray, size_t size) : Vectorset((const _T*)pArray, size) {};

    // Operators (adding elements)
    Typeset operator |(const _T &val) const
    {
      Typeset newset(*this);
      newset.insert(val);
      return newset;
    };
    Typeset &operator |=(const _T &val)
    {
      this->insert(val);
      return *this;
    };
    Typeset &operator |=(const Typeset<_T> &val)
    {
      this->insert(val);
      return *this;
    };
  };


  template<class _T, int __unk>
  class Type
  {
  private:
    int id;
  public:
    // Constructor
    Type(int _id)
    {
      if ( _id < 0 || _id > __unk )
        this->id = __unk;
      else
        this->id = _id;
    };
    
    // Types
    typedef Typeset<_T> set;

    // Operators
    operator int() const { return this->id; };
    set operator |(const _T &other) const
    {
      set rset(this->id);
      rset.insert(other);
      return rset;
    };

    // Members
    int getID() const { return this->id; };
    bool isValid() const { return this->id >= 0 && this->id <= __unk; };
  };
}
