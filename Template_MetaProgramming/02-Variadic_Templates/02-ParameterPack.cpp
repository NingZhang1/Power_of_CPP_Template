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

// a template or function parameter pack is a template parameter that accepts zero or more template arguments (non-types, types, or templates)
// The standard does not specify the maximum number of arguments a parameter pack can accept, but in practice it is limited by the compiler's implementation.

// How to retrive the number of arguments in a parameter pack?

// 1. sizeof... operator
// The sizeof... operator returns the number of arguments in a parameter pack.
// The sizeof... operator can only be used in a context that depends on a template parameter pack.

namespace example_01
{
    template <typename T, typename... Args>
    T sum(T a, Args... args)
    {
        if constexpr (sizeof...(args) == 0)
            return a;
        else
            return a + sum(args...);
    }

    void test()
    {
        std::cout << sum(1, 2, 3, 4, 5) << std::endl;
        std::cout << sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) << std::endl;
        std::cout << sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15) << std::endl;
    }
};

/// the above example is equivalent to the following recursive function
namespace example_02
{
    template <typename T>
    T sum(T a)
    {
        return a;
    }

    template <typename T, typename... Args>
    T sum(T a, Args... args)
    {
        return a + sum(args...);
    }
};

/// the difference between sizeof...(Args), sizeof...(args), and sizeof(args)...
/// sizeof...(Args) returns the number of template arguments in the parameter pack Args
/// sizeof...(args) returns the number of function arguments in the parameter pack args
/// in most cases, sizeof...(args) is the same as sizeof...(Args), but there are cases where they are different

/// sizeof(args)... is a fold expression that expands to a comma-separated list of the sizes of the arguments in the parameter pack args

//// it is illurstrated in the following example

namespace example_03
{
    template <typename... Args>
    void print(Args... args)
    {
        std::cout << "sizeof...(Args): " << sizeof...(Args) << std::endl;
        std::cout << "sizeof...(args): " << sizeof...(args) << std::endl;
        std::cout << "sizeof(args)...: " << (sizeof(args), ...) << std::endl;  /// not as expected
    }

    template <typename... Ts>
    constexpr auto get_type_sizes()
    {
        return std::array<std::size_t, sizeof...(Ts)>{sizeof(Ts)...};
    }

    /// for a special case with 4 types, the function will be expanded to:

    template <typename T1, typename T2, typename T3, typename T4>
    constexpr auto get_type_sizes()
    {
        return std::array<std::size_t, 4>{
            sizeof(T1), sizeof(T2), sizeof(T3), sizeof(T4)};
    }

    void test()
    {
        print(1, 2, 3, 4, 5);
        print(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        print(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);

        auto sizes = get_type_sizes<int, double, char, std::string, std::vector<int>, std::map<int, std::string>>();
        for (auto size : sizes)
            std::cout << size << " ";
        std::cout << std::endl;
    }

};

int main()
{
    {
        using namespace example_01;
        test();
    }

    {
        using namespace example_03;
        test();
    }
}