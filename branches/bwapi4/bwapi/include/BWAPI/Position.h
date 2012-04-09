#pragma once
#include <math.h>
#include <algorithm>

#include <BWAPI/Vectorset.h>

#ifndef _MAKE_POSITION_TEMPLATE
#define _MAKE_POSITION_TEMPLATE(_n,_t,_s) typedef BWAPI::Point<_t,_s> _n;        \
                      namespace _n ## s              \
                      {  const _n Invalid(32000/_s,32000/_s);  \
                        const _n None(32000/_s,32032/_s);    \
                        const _n Unknown(32000/_s,32064/_s);  \
                      }

#define _OPERATOR_OP_PT(op) Point operator op (const Point &pos) const            \
              { return Point(this->x op pos.x, this->y op pos.y); };  \
              Point &operator op ## = (const Point &pos)              \
              { this->x op ## = pos.x; this->y op ## = pos.y;          \
                return *this; }; 

#define _OPERATOR_OP_VAL(op) Point operator op (const _T &val) const        \
              { return Point(this->x op val, this->y op val); };  \
              Point &operator op ## = (const _T &val)            \
              { this->x op ## = val; this->y op ## = val;          \
                return *this; }; 

#define _OPERATOR_OP_VAL_CHK(op) Point operator op (const _T &val) const                  \
                { if ( val == 0 ) return Point(32000/__Scale,32000/__Scale);        \
                  return Point(this->x op val, this->y op val); };          \
                Point &operator op ## = (const _T &val)                      \
                { if ( val == 0 ) { this->x = 32000/__Scale; this->y = 32000/__Scale; }  \
                  else { this->x op ## = val; this->y op ## = val; }              \
                  return *this; }; 

#endif

namespace BWAPI
{
  // Declaration
  template<typename _T, int __Scale = 1>
  class Point;
  template<typename _T, int __Scale = 1>
  class PointIterator;

  // Restrictions
  template<typename _T> class Point<_T, 0> {};
  template<int __Scale> class Point<char, __Scale> {};
  template<int __Scale> class Point<unsigned char, __Scale> {};
  template<int __Scale> class Point<bool, __Scale> {};
  template<typename _T> class PointIterator<_T, 0> {};
  template<int __Scale> class PointIterator<char, __Scale> {};
  template<int __Scale> class PointIterator<unsigned char, __Scale> {};
  template<int __Scale> class PointIterator<bool, __Scale> {};

  // ------------------------------------------------------ Point Iterator template ----------------
  template<typename _T, int __Scale>
  class PointIterator
  {
  public:
    // ----------------------------- CONSTRUCTORS --------------------------------
    PointIterator(const Point<_T,__Scale> &bottomRight) 
      : begin( Point<_T,__Scale>(0,0) )
      , end(bottomRight)
      , x(0)
      , y(0)
    {};
    PointIterator(_T xMax, _T yMax)
      : begin( Point<_T,__Scale>(0,0) )
      , end( Point<_T,__Scale>(xMax,yMax) )
      , x(0)
      , y(0)
    {};
    PointIterator(const Point<_T,__Scale> &topLeft, const Point<_T,__Scale> &bottomRight) 
      : begin(topLeft)
      , end(bottomRight)
      , x(topLeft.x)
      , y(topLeft.y)
    {};
    // ----------------------------- OPERATORS --------------------------------
    operator bool()
    {
      return this->x >= begin.x && this->x < end.x &&
             this->y >= begin.y && this->y < end.y;
    };
    PointIterator &operator =(const PointIterator &other)
    {
      this->x = other.x;
      this->y = other.y;
      this->begin = other.begin;
      this->end = other.end;
    };
    PointIterator &operator ++()
    {
      ++this->y;
      if ( this->y >= this->end.y )
      {
        this->y = this->begin.y;
        ++this->x;
      }
      return *this;
    };
    PointIterator operator ++(int)
    {
      PointIterator copy = *this;
      ++this->y;
      if ( this->y >= this->end.y )
      {
        this->y = this->begin.y;
        ++this->x;
      }
      return copy;
    };

    _T x;
    _T y;

    const Point<_T,__Scale> begin;
    const Point<_T,__Scale> end;
  };

  // ------------------------------------------------------ Point template ----------------
  template<typename _T, int __Scale>
  class Point
  {
  public:
    typedef PointIterator<_T,__Scale> iterator;
    typedef Vectorset< Point<_T,__Scale> > set;

    // Constructors
    Point(_T _x = 0, _T _y = 0) : x(_x), y(_y) {};
    Point(const iterator &pt) : x(pt.x), y(pt.y) {};

#pragma warning( disable: 4723 )
    // Conversion constructor
    template<typename _NT, int __NScale> explicit Point(const Point<_NT, __NScale> &pt)
      : x( (_T)(__NScale > __Scale ? pt.x*(__NScale/__Scale) : pt.x/(__Scale/__NScale)) )
      , y( (_T)(__NScale > __Scale ? pt.y*(__NScale/__Scale) : pt.y/(__Scale/__NScale)) ) { };
    template<typename _NT, int __NScale> explicit Point(const PointIterator<_NT, __NScale> &pt)
      : x( (_T)(__NScale > __Scale ? pt.x*(__NScale/__Scale) : pt.x/(__Scale/__NScale)) )
      , y( (_T)(__NScale > __Scale ? pt.y*(__NScale/__Scale) : pt.y/(__Scale/__NScale)) ) { };

#pragma warning( default: 4723 )
    // Conversion restriction constructor
    template<typename _NT> explicit Point(const Point<_NT, 0> &pt) : x(0), y(0) {};
    template<typename _NT> explicit Point(const PointIterator<_NT, 0> &pt) : x(0), y(0) {};

    // Operators
    operator bool() const { return this->isValid(); };
    
    bool operator == (const Point &pos) const
    { 
      return  this->x == pos.x &&
          this->y == pos.y;
    }; 
    bool operator != (const Point &pos) const
    { 
      return  this->x != pos.x ||
          this->y != pos.y;
    }; 

    bool operator  < (const Point &position) const
    {
      return  this->x < position.x ||
          (this->x == position.x && this->y < position.y);
    };

    _OPERATOR_OP_PT(+)
    _OPERATOR_OP_PT(-)

    _OPERATOR_OP_VAL(*)
    _OPERATOR_OP_VAL(&)
    _OPERATOR_OP_VAL(|)
    _OPERATOR_OP_VAL(^)
    _OPERATOR_OP_VAL(>>)
    _OPERATOR_OP_VAL(<<)

    _OPERATOR_OP_VAL_CHK(/)
    _OPERATOR_OP_VAL_CHK(%)

    // Functions
    bool isValid() const;
    Point &makeValid();

    double getDistance(const Point &position) const
    {
      return ((*this) - position).getLength();
    };
    double getLength() const
    {
      double x = (double)this->x;
      double y = (double)this->y;
      return sqrt(x * x + y * y);
    };

    int getApproxDistance(const Point &position) const
    {
      unsigned int min = abs((int)(this->x - position.x));
      unsigned int max = abs((int)(this->y - position.y));
      if ( max < min )
        std::swap(min, max);

      if ( min < (max >> 2) )
        return max;

      unsigned int minCalc = (3*min) >> 3;
      return (minCalc >> 5) + minCalc + max - (max >> 4) - (max >> 6);
    };

    Point &setMax(_T _x, _T _y)
    {
      if ( x > _x )
        x = _x;
      if ( y > _y )
        y = _y;
      return *this;
    };
    Point &setMax(const Point &max)
    {
      this->setMax(max.x, max.y);
      return *this;
    };
    Point &setMin(_T _x, _T _y)
    {
      if ( x < _x )
        x = _x;
      if ( y < _y )
        y = _y;
      return *this;
    };
    Point &setMin(const Point &max)
    {
      this->setMin(max.x, max.y);
      return *this;
    };

    // members
    _T x, y;
  };

  _MAKE_POSITION_TEMPLATE(WalkPosition,int,8)
  _MAKE_POSITION_TEMPLATE(Position,int,1)
  _MAKE_POSITION_TEMPLATE(TilePosition,int,32)
}

