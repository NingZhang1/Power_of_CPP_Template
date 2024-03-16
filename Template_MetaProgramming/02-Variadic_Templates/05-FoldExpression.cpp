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

/// fold expressions is a C++17 feature that allows to apply a *binary* operator to a pack of arguments

namespace example_01
{
    template <typename... T>
    int sum(T... args)
    {
        return (... + args);
    }

    /// is equivalent to

    template <typename T>
    T sum(T a)
    {
        return a;
    }

    template <typename T, typename... Ts>
    T sum(T a, Ts... args)
    {
        return a + sum(args...);
    }

    template <typename... T>
    int sum_from_zero(T... args)
    {
        return (0 + ... + args); /// T can be empty
    }
}

/// there are four different types of fold expressions
/// (1) unary right fold,  syntax (pack op ... ),        Expansion (arg1 op (arg2 op (arg3 op arg4))
/// (2) unary left fold,   syntax (... op pack),         Expansion (((arg1 op arg2) op arg3) op arg4)
/// (3) Binary right fold, syntax (pack op ... op init), Expansion (arg1 op (arg2 op (arg3 op (arg4 op init))))
/// (4) Binary left fold,  syntax (init op ... op pack), Expansion (((init op arg1) op arg2) op arg3) op arg4)

namespace example_02
{
    template <typename... T>
    int suml(T... args)
    {
        return (... + args);
    }

    template <typename... T>
    int sumr(T... args)
    {
        return (args + ...);
    }

    template <typename... T>
    void printl(T... args)
    {
        (..., (std::cout << args)) << '\n'; /// args can be part of an expression, as long as it is not expanded
    }

    template <typename... T>
    void printr(T... args)
    {
        ((std::cout << args), ...) << '\n';
    }

    template <typename... T>
    void print(T... args)
    {
        (std::cout << ... << args) << '\n';
    }

    template <typename T, typename... Args>
    void push_back_many(std::vector<T> &v, Args &&...args)
    {
        (v.push_back(args), ...);
    }
}

int main()
{
    {
        using namespace example_01;

        // std::cout << sum() << '\n'; // error
        std::cout << sum(1) << '\n';
        std::cout << sum(1, 2) << '\n';
        std::cout << sum(1, 2, 3, 4, 5) << '\n';

        std::cout << sum_from_zero() << '\n'; /// that is the difference between the two functions
        std::cout << sum_from_zero(1, 2, 3) << '\n';
    }

    {
        using namespace example_02;

        printl('d', 'o', 'g'); // dog
        printr('d', 'o', 'g'); // dog
        print('d', 'o', 'g');  // dog
    }

    {
        using namespace example_02;

        std::vector<int> v;
        push_back_many(v, 1, 2, 3, 4, 5);
    }
}

/// Advantages of fold expressions
/// (1) they are more readable than the equivalent recursive function
/// (2) less code
/// (3) fewer template instantiations, which can lead to faster compile times
/// (4) potential faster code by allowing the compiler to optimize the code better