#include <iostream>
#include <array>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <type_traits>
#include <array>
#include <numeric>
#include <algorithm>
#include <functional>
#include <memory>

// implicit instantiation
// Implicit instantiation occurs when the compiler encounters a use of a template
// that requires the template to be instantiated, but no explicit instantiation is provided.

// explicit instantiation definition
// An explicit instantiation definition forces the compiler to instantiate the template

// The syntax for class templates is:
// template class class-name <argument-list> ;

// The syntax for function templates is:
// template return-type function-name <argument-list> (parameter-list) ;

// example

namespace n226
{
   template <typename T>
   struct wrapper
   {
      T value;
   };

   template struct wrapper<int>; // [1]
}

template struct n226::wrapper<double>; // [2]

namespace n226
{
   template <typename T>
   T add(T const a, T const b)
   {
      return a + b;
   }

   template int add(int, int); // [1]
}

// template int n226::add(int, int); // [2], a second way, but this is duplicate here

// explicit instantiation declaration
// An explicit instantiation declaration is a declaration that tells the compiler that
// the instantiation of a template is to be generated in another translation unit.
// A new definition of the template should not be generated in the translation unit.

// The key difference between explicit instantiation definition and explicit instantiation declaration is that
// the former forces the compiler to instantiate the template, while the latter does not.
// For the latter, the keyword extern is used to indicate that the instantiation is to be generated elsewhere.

// The syntax for class templates is:
// extern template class class-name <argument-list> ;

// The syntax for function templates is:
// extern template return-type function-name <argument-list> (parameter-list) ;

#include "wrapper.h"

int main()
{
   ext::wrapper<int> a{0};
}