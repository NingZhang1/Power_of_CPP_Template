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

namespace BasicExamples01
{
    template <typename T>
        requires std::is_integral_v<T> && std::is_signed_v<T>
    T decrement(T value)
    {
        return value--;
    }
};

namespace BasicExamples02
{
    template <typename T>
    concept Integral = std::is_integral_v<T>;

    template <typename T>
    concept Signed = std::is_signed_v<T>;

    template <typename T>
    concept SignedIntegral = Integral<T> && Signed<T>; /// we can perform && or || operations on concepts !

    template <SignedIntegral T>
    T decrement(T value)
    {
        return value--;
    }
}

namespace Examples03
{   
   template <typename T>
   concept A = std::is_integral_v<T>;

   template <typename T>
   concept B = std::is_floating_point_v<T>;

   // disjunctions
   template <typename T>
   requires A<T> || B<T>  // disjunction of two atomic constraints
   void f() {}

   // template <typename T>
   // requires (A<T> || B<T>)
   // void f() {}

   template <typename T>
   requires A<T> && (!A<T> || B<T>)
   void f() {}

   // logical operators
   template <typename T>
   requires (!(A<T> || B<T>))  /// the whole expression is checked 
   void f() {}

   template <typename T>
   requires (static_cast<bool>(A<T> || B<T>))
   void f() {}
}

namespace ExampleParameterPack
{
   template <typename T>
   concept Integral = std::is_integral_v<T>;

   template <typename ... T>
   requires (Integral<T> && ...)   /// here it is not a disjunction, it is a fold expression
   // requires Integral<T> && ...   /// here it is a disjunction, it is not correct
   auto add(T ... args)
   {
      return (args + ...);
   }
}

int main()
{
}