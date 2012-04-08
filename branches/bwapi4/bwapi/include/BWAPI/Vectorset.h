#pragma once

#include <stdlib.h>
#include <string.h>

#include <type_traits>

namespace BWAPI
{
  template<typename _T>
  class iterator;

  template<typename _T>
  class Vectorset;

  /// @~English
  /// The iterator class template allows the iteration
  /// of elements of a Vectorset with ease while
  /// maintaining the compatibility with any familiar
  /// STL container iteration.
  ///
  /// @~
  /// @see Vectorset
  template<typename _T>
  class iterator
  {
  public:
    iterator(_T *ptr = NULL) : __val(ptr) {};
    iterator(const iterator<_T> &other) : __val(&other) {};
    bool operator ==(const iterator<_T> &other) const
    {
      return this->__val == &other;
    };
    bool operator !=(const iterator<_T> &other) const
    {
      return this->__val != &other;
    };
    bool operator ==(const _T &element) const
    {
      return *this->__val == element;
    };
    bool operator !=(const _T &element) const
    {
      return *this->__val != element;
    };
    iterator &operator ++()
    {
      ++__val;
      return *this;
    };
    iterator operator ++(int)
    {
      iterator copy = *this;
      ++__val;
      return copy;
    };
    iterator &operator --()
    {
      --__val;
      return *this;
    };
    iterator operator --(int)
    {
      iterator copy = *this;
      --__val;
      return copy;
    };
    _T operator *() const
    {
      return *__val;
    };
    _T *operator &() const
    {
      return __val;
    };
    _T operator ->() const
    {
      return *__val;
    };
  private:
    _T *__val;
  };

  /// @~English
  /// The Vectorset is a class template designed
  /// specifically for trivial classes or PODs and 
  /// performance. It mimics the usage of various
  /// stl containers (specifically the Vector and Set)
  /// in order to replace them. The Vectorset is
  /// specifically designed for BWAPI usage and is
  /// recommended, especially if stl containers are
  /// causing a bottleneck in your bot's code.
  ///
  /// @~
  template<typename _T>
  class Vectorset
  {
    static_assert(std::has_trivial_copy<_T>::value == true &&
                  std::has_trivial_copy_constructor<_T>::value == true &&
                  std::has_trivial_destructor<_T>::value == true,
                  "Vectorset can only be used with classes that have a trivial destructor and trivial copy constructor.");
  public:
    typedef iterator<_T> iterator;

    // ----------------------------------------------------------------- Constructors
    Vectorset(size_t initialSize = 16)
      : __totSize( initialSize )
      , __valArray( (_T*)malloc(initialSize*sizeof(_T)) )
      , __end(__valArray + initialSize)
      , __last(__valArray)
    { };
    Vectorset(const Vectorset<_T> &other)
      : __totSize( other.max_size() )
      , __valArray( (_T*)malloc(other.max_size()*sizeof(_T)) )
      , __end(__valArray + other.max_size())
      , __last(__valArray + other.size())
    { 
      memcpy(this->__valArray, &(other.begin()), other.size()*sizeof(_T));
    };
    Vectorset(const _T *pArray, size_t arrSize)
      : __totSize( arrSize )
      , __valArray( (_T*)malloc(arrSize*sizeof(_T)) )
      , __end( __valArray + arrSize )
      , __last( __valArray + arrSize )
    {
      memcpy(this->__valArray, pArray, arrSize*sizeof(_T) );
    };
    // ----------------------------------------------------------------- Operators
    Vectorset &operator =(const Vectorset<_T> &other)
    {
      // localize variables
      size_t nSize = other.size();

      // manage existing set
      this->clear();
      this->expand(nSize);

      // copy the data to this set
      memcpy(this->__valArray, other, nSize*sizeof(_T));

      // update variables in this set
      this->__last = this->__valArray + nSize;
      return *this;
    };
    Vectorset &operator +=(const Vectorset<_T> &other)
    {
      this->push_back(other);
      return *this;
    };
    Vectorset operator +(const Vectorset<_T> &other)
    {
      Vectorset vcopy(this->size() + other.size());
      vcopy.push_back(*this);
      vcopy.push_back(other);
      return vcopy;
    };
    Vectorset &operator |=(const Vectorset<_T> &other)
    {
      this->insert(other);
      return *this;
    };
    Vectorset operator |(const Vectorset<_T> &other)
    {
      Vectorset vcopy(this->size() + other.size());
      vcopy.insert(*this);
      vcopy.insert(other);
      return vcopy;
    };
    bool operator ==(const Vectorset<_T> &other) const
    {
      if ( this->empty() && other.empty() )
        return true;
      else if ( this->empty() || other.empty() )
        return false;

      return memcmp( this->__valArray, other, std::min(this->size(), other.size())*sizeof(_T)) == 0;
    };

    operator void*() const
    {
      return this->__valArray;
    };
    operator bool() const
    {
      return !this->empty();
    };

    _T operator [](unsigned int index) const
    {
      if ( index < this->size() )
        return this->__valArray[index];
      return __valArray[0];
    };

    // ----------------------------------------------------------------- Destructor
    ~Vectorset()
    {
      free(this->__valArray);
    };
    // ----------------------------------------------------------------- Custom functions
    void erase_once(const _T &val)
    {
      // declare iterators
      _T *i = this->__valArray, *iend = this->__last;

      // iterate everything
      while ( i != iend )
      {
        if ( val == *i )  // if values are equal
          break;  
        ++i; // iterate to next one if it is not found
      }
      
      if ( i == iend )  // ignore if not found
        return;

      // replace existing entry and assign the new last position
      --iend;
      *i = *iend;
      this->__last = iend;
    };

    // ----------------------------------------------------------------- Custom const functions
    bool exists(const _T &element) const
    {
      _T *i = this->__valArray, *iend = this->__last;
      while ( i != iend )
      {
        if ( element == *i )
          return true;
        ++i;
      }
      return false;
    };
    size_t max_size() const
    {
      return this->__totSize;
    };
    _T rand() const
    {
      size_t size = this->size();
      switch ( size )
      {
      case 0:
        return NULL;
      case 1:
        return this->__valArray[0];
      case 2:
        return this->__valArray[::rand()%2];
      case 4:
        return this->__valArray[::rand()%4];
      case 8:
        return this->__valArray[::rand()%8];
      case 16:
        return this->__valArray[::rand()%16];
      case 32:
        return this->__valArray[::rand()%32];
      case 64:
        return this->__valArray[::rand()%64];
      case 128:
        return this->__valArray[::rand()%128];
      case 256:
        return this->__valArray[::rand()%256];
      case 512:
        return this->__valArray[::rand()%512];
      case 1024:
        return this->__valArray[::rand()%1024];
      case 2048:
        return this->__valArray[::rand()%2048];
      }
      return this->__valArray[::rand()%size];
    };
    // ----------------------------------------------------------------- stl spinoff functions
    void clear()
    {
      this->__last = this->__valArray;
    };
    void erase(const _T &val)
    {
      // declare iterators
      _T *i = this->__valArray, *iend = this->__last;

      // iterate everything
      while ( i != iend )
      {
        if ( val == *i )  // if values are equal
        {
          --iend; // subtract the end iterator
          *i = *iend; // overwrite value with value that would be at the end
        }
        else
        {
          ++i;  // if values are not eq. then iterate to next one
        }
      }
      
      // assign the new last position
      this->__last = iend;
    };
    void erase(const iterator &iter)
    {
      _T *i = &iter, *iend = this->__last;
      if ( i < this->__valArray || i >= iend )  // small error checking
        return;

      while ( i+1 != iend )
      {
        *i = i[1];
        ++i;
      }
      --this->__last;
    };

    // element insertion
    void insert(const _T &val)
    {
      if ( !this->exists(val) )
        this->push_back(val);
    };
    void insert(const iterator &val)
    {
      this->insert(*val);
    };
    void insert(const Vectorset<_T> &other)
    {
      for ( iterator i = other.begin(); i != other.end(); ++i )
        this->insert(i);
    };
    void push_back(const _T val)
    {
      if ( this->__last == this->__end )
        this->expand();
      *(this->__last++) = val;
    };
    void push_back(const iterator &val)
    {
      this->push_back(*val);
    };
    void push_back(const Vectorset<_T> &other)
    {
      // localize variables
      size_t nSize = other.size();

      // manage existing set
      this->expand(nSize);
      
      // copy the data to this set
      memcpy( this->__last, other, nSize*sizeof(_T) );

      // update variables in this set
      this->__last += nSize;
    };
    void push_front(const _T val)
    {
      if ( this->__last == this->__end )
        this->expand();
      memmove(this->__valArray+1, this->__valArray, this->size()*sizeof(_T));
      this->__last++;
      *this->__valArray = val;
    };
    void push_front(const iterator &val)
    {
      this->push_front(*val);
    };
    void pop_back()
    {
      if ( !this->empty() )  // remove last element if non-empty
        --this->__last;
    };
    void pop_front()
    {
      if ( this->empty() )  // return if empty
        return;
      
      --this->__last; // subtract the last element (not removing it)
      size_t size = this->size();  // localize the new size
      switch ( size )
      {
      case 0:  // just ignore it if it was the only element
        break;
      case 1: // if only one element should remain
        *this->__valArray = *this->__last;
        break;
      default: // otherwise move all elements up
        memmove(this->__valArray, this->__valArray+1, size*sizeof(_T));
        break;
      }
    };
    // ----------------------------------------------------------------- stl spinoff const functions
    size_t size() const
    {
      return ((size_t)this->__last - (size_t)this->__valArray)/sizeof(_T);
    };
    bool empty() const
    {
      return this->__last == this->__valArray;
    };

    // iterators
    iterator begin() const
    {
      return this->__valArray;
    };
    iterator rbegin() const
    {
      return this->__last - 1;
    };
    iterator end() const
    {
      return this->__last;
    };
    iterator rend() const
    {
      return this->__valArray - 1;
    };
    iterator find(const _T &element) const
    {
      _T *i = this->__valArray, *iend = this->__last;
      while ( i != iend )
      {
        if ( element == *i )
          return i;
        ++i;
      }
      return iend;
    };
    _T front() const
    {
      return *this->__valArray;
    };
    _T back() const
    {
      return *(this->__last - 1);
    };

  // -----------------------------------------------------------------
  private:
    // expand container when full
    void expand()
    {
      // localize the variables
      size_t size   = this->__totSize;
      size_t oldsize   = this->size();

      // double the size, but don't exceed the maxSizeHint unless it is equal or already over
      size *= 2;
      
      // Reallocate and store the new values
      this->__valArray  = (_T*)realloc(__valArray, size*sizeof(_T));
      this->__end      = __valArray + size;
      this->__last    = __valArray + oldsize;
      this->__totSize   = size;
    };
    void expand(size_t expectedSize)
    {
      // localize the variables
      size_t size    = this->__totSize;
      size_t oldsize   = this->size();

      // expand to expected size
      if ( size >= oldsize + expectedSize )
        return;

      // expand to desired size
      size = oldsize + expectedSize;

      // Reallocate and store the new values
      this->__valArray  = (_T*)realloc( this->__valArray, size*sizeof(_T) );
      this->__end      = __valArray + size;
      this->__last    = __valArray + oldsize;
      this->__totSize     = size;
    };

    // Variables
    _T *__valArray;  // ptr to beginning of array
    _T *__last;    // ptr to last element + 1
    _T *__end;    // ptr to end of allocation
    size_t __totSize;
  };

}

