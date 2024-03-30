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

/// Concepts in C++20 are used to constrain templates in an elegant way.

/// One way to constrain templates is to use SFINAE (Substitution Failure Is Not An Error) technique.
/// but it is not easy to read and write.
/// another way is to use requires clause, but it is not flexible.

namespace ExamplesRequires01
{
    template <typename T>
        requires std::is_arithmetic_v<T>
    T add(T const a, T const b)
    {
        return a + b;
    }
}

namespace ExamplesRequires01
{
    template <typename T>
    T add(T const a, T const b)
        requires std::is_arithmetic_v<T>
    {
        return a + b;
    }
}

/// or we can use concepts

namespace ExamplesConcepts01
{
    template <typename T>
    concept arithmetic = requires { std::is_arithmetic_v<T>; };

    template <arithmetic T>
    T add(T const a, T const b)
    {
        return a + b;
    }
}

int main()
{
}