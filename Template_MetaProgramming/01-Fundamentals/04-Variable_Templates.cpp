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

// Variable templates are a feature introduced in C++14 that allow the definition of variables
// that are templated either at namespace scope, in which case they represent a family of global variables,
// or at class scope, in which case they represent a family of static data members.

// an example to show its usage

namespace example_01
{

    constexpr long double PI = 3.1415926535897932385L;

    template <typename T>
    T sphere_volume(T const r)
    {
        return static_cast<T>(4 * PI * r * r * r / 3);
    }

}

namespace example_02
{

    struct math_constants
    {
        template <class T>
        static const T PI; // variable template as a static data member
    };

    template <class T>
    const T math_constants::PI = T(3.1415926535897932385L);

    template <typename T>
    T sphere_volume(T const r)
    {
        return 4 * math_constants::PI<T> * r * r * r / 3;
    }

}

namespace example_03
{
    template <class T>
    constexpr T PI = T(3.1415926535897932385L); // variable template as a global variable

    template <typename T>
    T sphere_volume(T const r)
    {
        return 4 * PI<T> * r * r * r / 3;
    }

}

namespace example_04
{
    /// combined with explicit template instantiation

    template <typename T>
    struct is_floating_point
    {
        constexpr static bool value = false;
    };

    template <>
    struct is_floating_point<float>
    {
        constexpr static bool value = true;
    };

    template <>
    struct is_floating_point<double>
    {
        constexpr static bool value = true;
    };

    template <>
    struct is_floating_point<long double>
    {
        constexpr static bool value = true;
    };

    template <typename T>
    inline constexpr bool is_floating_point_v = is_floating_point<T>::value; // variable template, type trait, C++14 easier to use

}

int main()
{
    {
        using namespace example_01;

        float v1 = sphere_volume(2.0f); // warning
        double v2 = sphere_volume(2.0); // OK
    }

    {
        using namespace example_02;

        float v1 = sphere_volume(2.0f); // OK
        double v2 = sphere_volume(2.0); // OK
    }

    {
        using namespace example_03;

        float v1 = sphere_volume(2.0f); // OK
        double v2 = sphere_volume(2.0); // OK
    }
}