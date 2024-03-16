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

/// lambda expressions : are a concise way to define a function object
/// they are syntactic sugar for a function object.

int main()
{
    {
        int arr[] = {1, 6, 3, 8, 4, 2, 9};
        std::sort(
            std::begin(arr), std::end(arr),
            [](int const a, int const b)
            { return a > b; });

        int pivot = 5;
        auto count = std::count_if(
            std::begin(arr), std::end(arr),
            [pivot](int const a)
            { return a > pivot; });

        std::cout << count << '\n';
    }

    /// classification of lambda expressions

    {
        auto l1 = [](int a)
        { return a + a; }; // C++11, regular lambda
        auto l2 = [](auto a)
        { return a + a; }; // C++14, generic lambda, at least one of the argument is defined by the auto specifier

        auto l3 = []<typename T>(T a)
        { return a + a; }; // C++20, template lambda

        auto v1 = l1(42);   // OK
        auto v2 = l1(42.0); // warning
        // auto v3 = l1(std::string{ "42" });   // error

        auto v5 = l2(42);                // OK
        auto v6 = l2(42.0);              // OK
        auto v7 = l2(std::string{"42"}); // OK

        auto v8 = l3(42);                 // OK
        auto v9 = l3(42.0);               // OK
        auto v10 = l3(std::string{"42"}); // OK
    }

    /// what is the difference between a generic lambdas and lambda tempaltes ?

    {
        auto l1 = [](int a, int b)
        { return a + b; };
        auto l2 = [](auto a, auto b)
        { return a + b; }; // the auto specifier is used to define the type of the arguments, however it puts a restriction that the types of the arguments should be the same
        auto l3 = []<typename T, typename U>(T a, U b)
        { return a + b; };
        auto l4 = [](auto a, decltype(a) b)
        { return a + b; };

        auto v1 = l1(42, 1);     // OK
        auto v2 = l1(42.0, 1.0); // warning
        // auto v3 = l1(std::string{ "42" }, '1'); // error

        auto v4 = l2(42, 1);                               // OK
        auto v5 = l2(42.0, 1);                             // OK
        auto v6 = l2(std::string{"42"}, '1');              // OK
        auto v7 = l2(std::string{"42"}, std::string{"1"}); // OK

        auto v8 = l3(42, 1);                                 // OK
        auto v9 = l3(42.0, 1);                               // OK
        auto v10 = l3(std::string{"42"}, '1');               // OK
        auto v11 = l3(std::string{"42"}, std::string{"42"}); // OK

        auto v12 = l4(42.0, 1); // OK
        auto v13 = l4(42, 1.0); // warning
                                // auto v14 = l4(std::string{ "42" }, '1'); // error
    }

    /// one benefit of generic lambdas over regular lambdas concerns recursive lambdas

    /// with regular lambdas you have to first define a function objcet and then capture it !
    {
        std::function<int(int)> factorial;
        factorial = [&factorial](int const n)
        {
            if (n < 2)
                return 1;
            else
                return n * factorial(n - 1);
        };

        std::cout << factorial(5) << '\n';
    }

    {
        auto factorial = [](auto f, int const n)
        {
            if (n < 2)
                return 1;
            else
                return n * f(f, n - 1);
        };

        std::cout << factorial(factorial, 5) << '\n';

        /// hence generic lambdas are more powerful than regular lambdas, and can be treated as a function object with the auto specifier as a template parameter ! 
    }
}