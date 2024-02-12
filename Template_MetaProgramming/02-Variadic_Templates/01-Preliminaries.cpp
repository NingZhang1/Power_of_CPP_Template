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

/// functions with variable number of arguments,
/// for example, printf, scanf, etc.
/// In C, these functions are implemented using va_list, va_start, va_arg, va_end

namespace CStyle_VariadicFunc
{
#include <stdarg.h>

    int min(int count, ...)
    {
        va_list args;
        va_start(args, count);

        int val = va_arg(args, int);
        for (int i = 1; i < count; i++)
        {
            int n = va_arg(args, int);
            if (n < val)
                val = n;
        }

        va_end(args);

        return val;
    }

    /// we can make it a template function

    template <typename T>
    T min(int count, ...)
    {
        va_list args;
        va_start(args, count);   /// WARNING: we have to know the number of arguments in some way, not generic

        T val = va_arg(args, T); /// WARNING: not type safe, not generic
        for (int i = 1; i < count; i++)
        {
            T n = va_arg(args, T);
            if (n < val)
                val = n;
        }

        va_end(args);

        return val;
    }

}; // namespace CStyle_VariadicFunc

namespace VariadicTemplates
{
    template <typename T>
    T min(T a)
    {
        return a;
    }

    /// or we can end the recursion with a template function with two arguments

    template <typename T>
    T min(T a, T b)
    {
        return std::min(a, b);
    }

    /// note, in the following function, ... appears three times

    template <typename T, typename... Args> /// template parameter pack, can be defined for type, non-type and template template parameters, **template parameter pack**
    T min(T a, Args... args)                /// specify a pack of arguments in the function parameter list, **function parameter pack**
    {
        return std::min(a, min(args...)); /// expand a pack of arguments in the function body, **parameter pack expansion**
    }

    /// for a pack of arguments with five integers, the function will be expanded to:

    int min(int a, int b, int c, int d, int e)
    {
        return std::min(a, std::min(b, std::min(c, std::min(d, e))));
    }

}; // namespace VariadicTemplates

int main()
{

    {
        using namespace CStyle_VariadicFunc;

        std::cout << "CStyle_VariadicFunc::min(3, 42, 7, 0)=" << min(3, 42, 7, 0) << '\n';
        std::cout << "min(42, 7)=" << min(2, 42, 7) << '\n';
        std::cout << "min(1,5,3,-4,9)=" << min(5, 1, 5, 3, -4, 9) << '\n';

        std::cout << "min<double>(2.0, 42.0, 7.5)=" << min<double>(2, 42.0, 7.5) << '\n';
        std::cout << "min<int>(1,5,3,-4,9)=" << min<int>(5, 1, 5, 3, -4, 9) << '\n';
    }

    {
        using namespace VariadicTemplates;

        std::cout << "VariadicTemplates::min(3, 42, 7, 0)=" << min(3, 42, 7, 0) << '\n';
        std::cout << "min(42, 7)=" << min(42, 7) << '\n';
        std::cout << "min(1,5,3,-4,9)=" << min(1, 5, 3, -4, 9) << '\n';
    }
}