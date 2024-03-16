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

/// *typetraits* are a set of classes and functions that allow to inspect and transform types

namespace example
{
    template <typename T>
    struct is_floating_point
    {
        static const bool value = false;
    };

    template <>
    struct is_floating_point<float>
    {
        static const bool value = true;
    };

    template <>
    struct is_floating_point<double>
    {
        static const bool value = true;
    };

    template <>
    struct is_floating_point<long double>
    {
        static const bool value = true;
    };

    template <typename T>
    void process_real_number(T const value)
    {
        static_assert(is_floating_point<T>::value);

        std::cout << "processing a real number: " << value << '\n';
    }
}

int main()
{
    {
        using namespace example;

        static_assert(is_floating_point<float>::value);
        static_assert(is_floating_point<double>::value);
        static_assert(is_floating_point<long double>::value);
        static_assert(!is_floating_point<int>::value);
        static_assert(!is_floating_point<bool>::value);

        process_real_number(42.0);
        // process_real_number(42); // error: static assertion failed
    }
}