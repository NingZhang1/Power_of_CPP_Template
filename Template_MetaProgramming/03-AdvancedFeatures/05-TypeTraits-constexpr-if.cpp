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

//// constexpr if is a feature that allows you to conditionally compile code based on a compile-time condition.
//// and is extremely easy to use.

namespace BasicExample
{
    template <typename T>
    bool are_equal(T const &a, T const &b)
    {
        if constexpr (std::is_floating_point_v<T>)
            return std::abs(a - b) < 0.001;
        else
            return a == b;
    }

    template <unsigned int n>
    constexpr unsigned int factorial()
    {
        if constexpr (n > 1)
            return n * factorial<n - 1>();
        else
            return 1;
    }
};

int main()
{
}