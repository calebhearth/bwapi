#pragma once
#include <functional>
#include "UnaryFilter.h"

#define BWAPI_COMPARE_FILTER_OP(op) UnaryFilterBase<_Param> operator op(const int &cmp)    \
                                    {   return [&](_Param u)->bool{ return (*this)(u) op cmp; };   }

namespace BWAPI
{
  /// @~English
  /// The CompareFilter is a temporary container
  /// in which the stored function predicate
  /// returns an integer. If any relational
  /// operators are used, then it becomes
  /// a UnaryFunction.
  ///
  /// @~
  template<typename _Param>
  class CompareFilterBase
  {
  private:
    std::function<int(_Param)> pred;
  public:
    // Constructor
    template <typename _T>
    CompareFilterBase(const _T&& predicate) : pred(predicate)
    {};

    // Assignment
    template <typename _T>
    CompareFilterBase &operator =(const _T&& other)
    {
      this->pred.assign(other);
      return *this;
    };

    // Comparisons
    BWAPI_COMPARE_FILTER_OP(==);
    BWAPI_COMPARE_FILTER_OP(!=);
    BWAPI_COMPARE_FILTER_OP(<=);
    BWAPI_COMPARE_FILTER_OP(>=);
    BWAPI_COMPARE_FILTER_OP(<);
    BWAPI_COMPARE_FILTER_OP(>);

    // call
    inline bool operator()(_Param u) const
    {
      return pred(u);
    };
    inline operator bool() const
    {
      return (bool)pred;
    };
  };
}

