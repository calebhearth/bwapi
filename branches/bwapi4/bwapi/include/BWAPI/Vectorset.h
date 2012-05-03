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
  /// designed for BWAPI usage and is recommended, 
  /// especially if stl containers are causing a 
  /// bottleneck in your bot's code.
  ///
  /// @warning The Vectorset should only be used with 
  /// objects that have a trivial destructor and 
  /// trivial copy constructor. Objects in the
  /// Vectorset should not do any memory management 
  /// or the behaviour will be undefined. Use with
  /// caution.
  ///
  /// @note The Vectorset will only free memory
  /// when the object is destroyed.
  /// @~
  /// @see std::vector, std::set
  template<typename _T>
  class Vectorset
  {
    static_assert(std::has_trivial_copy<_T>::value == true &&
                  std::has_trivial_copy_constructor<_T>::value == true &&
                  std::has_trivial_destructor<_T>::value == true,
                  "Vectorset can only be used with classes that have a trivial destructor and trivial copy constructor.");
  public:
    /// @~English
    /// The iterator is used to traverse the elements 
    /// in the set. It is used the same way as in the
    /// stl containers.
    /// @~
    typedef iterator<_T> iterator;

  // ----------------------------------------------------------------- Constructors
    /// @~English
    /// This is the default constructor. The Vectorset
    /// will allocate memory for the given number of
    /// elements (or 16 by default).
    ///
    /// @param initialSize The number of elements
    /// of type _T to allocate memory for.
    /// @~
    Vectorset(size_t initialSize = 16)
      : __totSize( initialSize )
      , __valArray( (_T*)malloc(initialSize*sizeof(_T)) )
      , __end(__valArray + initialSize)
      , __last(__valArray)
    { };
    /// @~English
    /// This is the copy constructor. The Vectorset
    /// will allocate only the necessary space to
    /// copy the other Vectorset's contents.
    ///
    /// @param other Reference to the Vectorset of the 
    /// same type whose contents will be copied.
    ///
    /// @note Duplicate entries are not removed.
    /// @~
    Vectorset(const Vectorset<_T> &other)
      : __totSize( other.max_size() )
      , __valArray( (_T*)malloc(other.max_size()*sizeof(_T)) )
      , __end(__valArray + other.max_size())
      , __last(__valArray + other.size())
    { 
      memcpy(this->__valArray, &(other.begin()), other.size()*sizeof(_T));
    };
    /// @~English
    /// This constructor uses an existing array
    /// of objects and copies them into the vector.
    /// The Vectorset will allocate only enough
    /// memory to copy the array's contents.
    ///
    /// @param pArray Pointer to an array of objects of
    /// type _T.
    /// @param arrSize The number of elements contained
    /// in the given array.
    ///
    /// @note Duplicate entries are not removed.
    /// @~
    Vectorset(const _T *pArray, size_t arrSize)
      : __totSize( arrSize )
      , __valArray( (_T*)malloc(arrSize*sizeof(_T)) )
      , __end( __valArray + arrSize )
      , __last( __valArray + arrSize )
    {
      memcpy(this->__valArray, pArray, arrSize*sizeof(_T) );
    };
  // ----------------------------------------------------------------- Operators
    /// @copydoc push_back(const Vectorset<_T> &)
    /// @~English
    /// @note The Vectorset is emptied before the contents
    /// of other are copied.
    ///
    /// @returns A reference to the current object.
    /// @~
    Vectorset &operator =(const Vectorset<_T> &other)
    {
      this->clear();
      this->push_back(other);
      return *this;
    };
    /// @copydoc push_back(const Vectorset<_T> &)
    /// @~English
    /// @returns A reference to the current object.
    /// @~
    /// @see operator|=
    Vectorset &operator +=(const Vectorset<_T> &other)
    {
      this->push_back(other);
      return *this;
    };
    /// @~English
    /// Creates a new Vectorset containing all elements
    /// of the current Vectorset with all elements of
    /// the other Vectorset appended on to it.
    ///
    /// @param other The other Vectorset of the same 
    /// type to use in combination.
    ///
    /// @returns A new Vectorset containing the contents
    /// of both this and other.
    ///
    /// @note Duplicate entries are not removed.
    /// @~
    /// @see operator|
    Vectorset operator +(const Vectorset<_T> &other)
    {
      Vectorset vcopy(this->size() + other.size());
      vcopy.push_back(*this);
      vcopy.push_back(other);
      return vcopy;
    };
    /// @copydoc insert(const Vectorset<_T> &)
    /// @~English
    /// @returns A reference to the current object.
    /// @~
    /// @see operator+=
    Vectorset &operator |=(const Vectorset<_T> &other)
    {
      this->insert(other);
      return *this;
    };
    /// @~English
    /// Creates a new Vectorset containing all elements
    /// of the current Vectorset and all elements of
    /// the other Vectorset with duplicates removed.
    ///
    /// @param other The other Vectorset of the same 
    /// type to use in combination.
    ///
    /// @returns A new Vectorset containing the contents
    /// of both this and other.
    /// @~
    /// @see operator+
    Vectorset operator |(const Vectorset<_T> &other)
    {
      Vectorset vcopy(this->size() + other.size());
      vcopy.insert(*this);
      vcopy.insert(other);
      return vcopy;
    };
    /// @~English
    /// Compares this Vectorset with another Vectorset
    /// of the same type.
    /// 
    /// @param other A Vectorset of the same type to
    /// use for comparison.
    ///
    /// @retval true if the other Vectorset is exactly
    /// the same as this one.
    /// @retval false if at least one element in this
    /// Vectorset is not found in the other, or if
    /// they are not in the exact same positions.
    ///
    /// @note This simply calls memcmp.
    /// @~
    bool operator ==(const Vectorset<_T> &other) const
    {
      if ( this->empty() && other.empty() )
        return true;
      else if ( this->empty() || other.empty() )
        return false;

      return memcmp( this->__valArray, other, std::min(this->size(), other.size())*sizeof(_T)) == 0;
    };
    
    /// @~English
    /// Retrieves a pointer to the raw data in the Vectorset.
    ///
    /// @note Vectorset data is simply an array.
    ///
    /// @returns A pointer to the Vectorset's array data.
    /// @~
    operator void*() const
    {
      return this->__valArray;
    };
    /// @~English
    /// Used as a macro to check if the Vectorset is empty.
    ///
    /// @retval true if the Vectorset is not empty.
    /// @retval false if the Vectorset is empty.
    /// @~
    operator bool() const
    {
      return !this->empty();
    };
    /// @~English
    /// Array indexing operator.
    /// 
    /// @TODO Needs to be modified
    ///
    /// @param index The array index in the Vectorset to 
    /// retrieve the value from.
    /// @~
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
    /// @~English
    /// This function erases an element from a Vectorset.
    /// Unlike erase, it assumes there exists only one element.
    /// If the element is found, it is removed and the function
    /// immediately returns.
    ///
    /// @param val The value to erase from the Vectorset.
    /// @~
    /// @see erase
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
    /// @~English
    /// This function checks if an element exists in the
    /// Vectorset.
    ///
    /// @param element The value to search for in the Vectorset.
    ///
    /// @retval true if the element is in the Vectorset.
    /// @retval false if the element was not found.
    /// @~
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
    /// @~English
    /// Retrieves the current capacity of the Vectorset.
    ///
    /// @returns The number of elements this Vectorset
    /// can contain before needing to allocate more memory.
    /// @~
    size_t max_size() const
    {
      return this->__totSize;
    };
    /// @~English
    /// This macro is used to choose a random value from
    /// a Vectorset.
    ///
    /// @TODO needs to be modified
    ///
    /// @retval NULL if the Vectorset is empty.
    ///
    /// @note This function calls the rand() function.
    /// A call to srand() should be used for initialization.
    /// @~
    /// @see rand()
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
    /// @~English
    /// Clears the Vectorset, removing all elements.
    /// Used the same way as stl containers.
    ///
    /// @note Because of the restrictions placed on
    /// the Vectorset, this function executes a
    /// single instruction regardless of the
    /// number of entries.
    /// @~
    void clear()
    {
      this->__last = this->__valArray;
    };
    /// @~English
    /// Erases all values found in the set. When
    /// a value is found, it is erased and the
    /// function continues searching for the same
    /// value until it reaches the end of the
    /// Vectorset.
    ///
    /// @param val The value to search for and remove.
    /// 
    /// @~
    /// @see erase_once
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
    /// @~English
    /// Erases the value at an iterator for this
    /// Vectorset. The advantage of this function
    /// is that searching for the value is not
    /// necessary.
    ///
    /// @param iter The iterator for the position
    /// to erase.
    /// 
    /// @~
    /// @see erase_once
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
    /// @~English
    /// Inserts a new value into the Vectorset
    /// only if it does not already exist.
    ///
    /// @param val The value to insert.
    /// 
    /// @~
    /// @see std::set
    void insert(const _T &val)
    {
      if ( !this->exists(val) )
        this->push_back(val);
    };
    /// @copydoc insert(const _T &val)
    void insert(const iterator &val)
    {
      this->insert(*val);
    };
    /// @~English
    /// Inserts all elements of another vector
    /// only if each element does not already
    /// exsist.
    ///
    /// @param other Another Vectorset of the
    /// same type whose elements will be inserted
    /// into this one.
    ///
    /// @~
    void insert(const Vectorset<_T> &other)
    {
      for ( iterator i = other.begin(); i != other.end(); ++i )
        this->insert(i);
    };
    /// @~English
    /// Pushes a value to the back of the Vectorset,
    /// expanding it if necessary.
    /// 
    /// @param val The value to add to the back of
    /// this Vectorset.
    ///
    /// @note Duplicate entries are not removed.
    /// @~
    /// @see push_front
    void push_back(const _T val)
    {
      if ( this->__last == this->__end )
        this->expand();
      *(this->__last++) = val;
    };
    /// @copydoc push_back(const _T val)
    void push_back(const iterator &val)
    {
      this->push_back(*val);
    };
    /// @~English
    /// Pushes all values of another Vectorset
    /// to the back of this one, expanding it
    /// to contain exactly the number of elements
    /// in both Vectorsets if necessary.
    /// 
    /// @param other The other Vectorset of the
    /// same type whose values will be appended
    /// to this one.
    ///
    /// @note Duplicate entries are not removed.
    /// @~
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
    /// @~English
    /// Pushes a value to the front of the Vectorset,
    /// expanding it if necessary.
    /// 
    /// @param val The value to add to the back of
    /// this Vectorset.
    ///
    /// @note Duplicate entries are not removed.
    /// @note For efficiency, it is recommended to use push_back.
    /// @~
    /// @see push_back
    void push_front(const _T val)
    {
      if ( this->__last == this->__end )
        this->expand();
      memmove(this->__valArray+1, this->__valArray, this->size()*sizeof(_T));
      this->__last++;
      *this->__valArray = val;
    };
    /// @copydoc push_front(const _T val)
    void push_front(const iterator &val)
    {
      this->push_front(*val);
    };
    /// @~English
    /// @TODO change return value to copy of value that was popped
    /// @~
    /// @see pop_front
    void pop_back()
    {
      if ( !this->empty() )  // remove last element if non-empty
        --this->__last;
    };
    /// @~English
    /// @TODO change return value to copy of value that was popped
    /// @note For efficiency, it is recommended to use pop_back.
    /// @~
    /// @see pop_back
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
    /// @~English
    /// Retrieves the number of elements currently
    /// in the Vectorset.
    ///
    /// @returns The number of elements contained
    /// in this Vectorset.
    ///
    /// @note For efficiency it is recommended to use
    /// empty() to check if the Vectorset has 0 elements.
    /// @~
    /// @see empty
    size_t size() const
    {
      return ((size_t)this->__last - (size_t)this->__valArray)/sizeof(_T);
    };
    /// @~English
    /// Checks if the Vectorset is empty.
    ///
    /// @retval true if the Vectorset is empty.
    /// @retval false if the Vectorset contains elements.
    /// @~
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
  // ----------------------------------------------------------------- stl algorithms
    /// @~English
    /// Works similar to the STL algorithm remove_if.
    /// Iterates and calls a function predicate for
    /// each element in the Vectorset. If the predicate
    /// call returns true, then the value is removed.
    ///
    /// @param pred Function predicate used to determine
    /// if a value is removed.
    /// @~
    /// @see std::remove_if
    template <typename Func>
    void remove_if( const Func &pred )
    {
      auto i = this->begin();
      while ( i != this->end() )
      {
        if ( pred(*i) )
          this->erase(i);
        else
          ++i;
      }
    };
    /// @~English
    /// Works similar to the STL algorithm count_if.
    /// Iterates and calls a function predicate for
    /// each element in the Vectorset. If the predicate
    /// call returns true, then a counter is incremented.
    ///
    /// @param pred Function predicate used to determine
    /// if a value is counted.
    ///
    /// @returns An integer containing the number of
    /// elements that were counted.
    /// @~
    /// @see std::count_if, count
    template <typename Func>
    int count_if( const Func &pred ) const
    {
      size_t rval = 0;
      for ( auto i = this->begin(); i != this->end(); ++i )
      {
        if ( pred(*i) )
          ++rval;
      }
      return rval;
    };
    /// @~English
    /// Works similar to the STL algorithm count.
    /// Iterates and compares each element of the
    /// Vectorset to a value. If the value matches,
    /// then a counter is incremented.
    ///
    /// @param val The value to compare each element
    /// with.
    ///
    /// @returns An integer containing the number of
    /// elements that were counted.
    /// @~
    /// @see std::count, count_if, size
    int count(const _T &val) const
    {
      size_t rval = 0;
      for ( auto i = this->begin(); i != this->end(); ++i )
      {
        if ( *i == val )
          ++rval;
      }
      return rval;
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

