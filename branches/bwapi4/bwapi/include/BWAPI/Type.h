#pragma once
#include <string>
#include <cctype>

#include <BWAPI/Vectorset.h>

namespace BWAPI
{
  template<class _T>
  class Typeset : public Vectorset<_T>
  {
  public:
    // Constructors
    Typeset() : Vectorset() {};
    // copy ctor
    Typeset(const Typeset<_T> &other) : Vectorset( other ) {};
    Typeset(const ConstVectorset<_T> &other) : Vectorset( other ) {};

    // move ctor
    Typeset(Typeset<_T> &&other) : Vectorset( std::forward< Typeset<_T> >(other) ) {};
    // type ctor
    Typeset(const _T &val) : Vectorset() { this->push_back(val); };
    // array ctor
    Typeset(const _T *pArray, size_t size) : Vectorset(pArray, size) {};
    Typeset(const int *pArray, size_t size) : Vectorset((const _T*)pArray, size) {};

    ~Typeset() {};

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

  /// Base class for all BWAPI Types.
  template<class _T, int __unk>
  class Type
  {
  protected:
    /// Primitive storage unit for the type identifier
    int tid;

    /// Array of strings containing the type names.
    static const std::string typeNames[__unk+1];
    
  public:
    /// Expected type constructor. If the type is an invalid type, then it becomes Types::Unknown.
    /// A type is invalid if its value is less than 0 or greater than Types::Unknown.
    ///
    /// @param id (optional)
    ///   The id that corresponds to this type. It is typically an integer value that corresponds
    ///   to an internal Broodwar type. If the given id is invalid, then it becomes Types::Unknown.
    ///   If it is omitted, then it becomes Types::None.
    explicit Type(int id) : tid( id < 0 || id > __unk ? __unk : id ) {};
    
    /// The set that contains the current type.
    typedef Typeset<_T> set;

    /// The constant set that contains the current type.
    typedef ConstVectorset<_T> const_set;

    /// Conversion/convenience operator to convert this type to its primitive type.
    inline operator int() const { return this->tid; };

    /// Retrieves this type's identifier.
    ///
    /// @returns An integer representation of this type.
    inline int getID() const { return this->tid; };

    /// Checks if the current type has a valid identifier. The purpose of this function is to
    /// prevent buffer overflows if a type has been handled improperly.
    ///
    /// A type is valid if it is between 0 and Unknown (inclusive).
    ///
    /// @retval true If this type is valid.
    /// @retval false if this type is invalid.
    inline bool isValid() const { return this->tid >= 0 && this->tid <= __unk; };

    /// Retrieves the variable name of the type.
    ///
    /// @returns Reference to std::string object containing the name.
    inline const std::string &getName() const
    {
      return typeNames[this->isValid() ? this->tid : __unk];
    };

    /// @copydoc Type::getName
    inline const std::string &toString() const
    {
      return this->getName();
    };

    /// Retrieves the variable name of the type as a c-style string. Meant to be a convenience
    /// member.
    ///
    /// @returns Pointer to constant c-style string containing the name.
    inline const char *c_str() const
    {
      return this->getName().c_str();
    };

    /// Output stream operator overload. Allows printing of the type without calling
    /// Type::getName.
    friend inline std::ostream &operator << (std::ostream &out, const Type<_T,__unk> &t)
    {
      return out << t.getName();
    };

    /// Searches for the type associated with the given string and returns it.
    ///
    /// @param name
    ///     A string containing the name of the type.
    ///
    /// @returns The type that resolves to the given name.
    /// @TODO: Test. This may be incorrect.
    static _T getType(const std::string &name)
    {
      for ( int i = 0; i < __unk; ++i )
      {
        const std::string &str = typeNames[i];
        std::string::const_iterator ita, itb;
        
        for ( ita = name.begin(), itb = str.begin(); 
              ita != name.end() && itb != str.end();
              ++ita, ++itb )
        {
          if ( isspace(*ita) || *ita == '_' ) // Ignore spaces for iterator a
          {
            ++ita;
            continue;
          }
          if ( isspace(*itb) || *itb == '_' ) // Ignore spaces for iterator b
          {
            ++itb;
            continue;
          }

          if ( tolower(*ita) != tolower(*itb) ) // If matching differs at some point
            break;
        }
        if ( ita == name.end() && itb == str.end() ) // found perfect match
          return _T(i);
      }
      return _T(__unk);
    };
  };

}
