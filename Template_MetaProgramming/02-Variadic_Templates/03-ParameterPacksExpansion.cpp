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

/// Parameter pack expansion can appear in many contexts, including:

namespace examples
{
    // 1. Template parameter list

    template <typename... T>
    struct outer
    {
        template <T... args> /// spepcify parameters for a template
        struct inner
        {
        };
    };

    // 2. Template argument list

    template <typename... T>
    struct tag
    {
    };

    /// the following examples show that you can switch the order of the types in the parameter pack

    template <typename T, typename U, typename... Args>
    void tagger()
    {
        [[maybe_unused]] tag<T, U, Args...> t1;
        [[maybe_unused]] tag<T, Args..., U> t2;
        [[maybe_unused]] tag<Args..., T, U> t3;
        [[maybe_unused]] tag<U, T, Args...> t4;
    }

    // 3. Function parameter list

    template <typename... Args>
    void make_it(Args... args)
    {
    }

    // 4. Function argument list
    // When the expansion pack appears inside a function call, the largest expression or
    // brace initialization list to the left of the ellipsis is expanded for each element in the pack.

    template <typename T>
    T step_it(T value)
    {
        return value + 1;
    }

    template <typename... T>
    int sum(T... args)
    {
        return (... + args); /// fold expression
    }

    template <typename... T>
    void do_sums(T... args)
    {
        auto s1 = sum(args...);
        auto s2 = sum(42, args...);
        auto s3 = sum(step_it(args)...);
        std::cout << s1 << " " << s2 << " " << s3 << std::endl;
    }

    // 5. Parenthesized initializer
    // When the expansion pack appears inside the parentheses of a direct initializer, function-style cast,
    // member initialize, new expression, and other similar context, the rules are the same as for function argument list.

    template <typename... T>
    struct sum_wrapper
    {
        sum_wrapper(T... args)
        {
            value = (... + args);
        }

        std::common_type_t<T...> value; /// common_type_t is a type trait that returns the common type of a set of types, which all the types can be **converted** to.

        void print()
        {
            std::cout << value << std::endl;
        }
    };

    template <typename... T>
    void parenthesized(T... args)
    {
        std::array<std::common_type_t<T...>, sizeof...(T)> arr{args...};

        sum_wrapper sw1(args...);   // value = 1 + 2 + 3 + 4
        sum_wrapper sw2(++args...); // value = 2 + 3 + 4 + 5

        sw1.print();
        sw2.print();
    }

    // 6. Braced-enclosed initializer

    template <typename... T>
    void brace_enclosed(T... args)
    {
        int arr1[sizeof...(args) + 1] = {args..., 0};   // 1,2,3,4,5
        int arr2[sizeof...(args)] = {step_it(args)...}; // 2,3,4,5
    }

    // 7. Base specifiers and member initializer lists
    // 8. Using declarations

    struct A
    {
        void execute() { std::cout << "A::execute\n"; }
    };
    struct B
    {
        void execute() { std::cout << "B::execute\n"; }
    };
    struct C
    {
        void execute() { std::cout << "C::execute\n"; }
    };

    template <typename... Bases>
    struct X : public Bases...
    {
        X(Bases const &...args) : Bases(args)...
        {
        }

        using Bases::execute...; /// using declaration
    };

    // 9. Lambda captures

    template <typename... T>
    void captures(T... args)
    {
        auto l = [args...]
        { return sum(step_it(args)...); };
        auto s = l();
    }

    // 10. Fold expressions

    // 11. sizeof... operator

    template <typename... T>
    auto make_array(T... args)
    {
        return std::array<std::common_type_t<T...>, sizeof...(T)>{args...};
    };

    // 12. Alignment specifiers

    // a pack in a alignment specifier has the same effect as having multiple alignas specifies applied to the same declaration.

    template <typename... T>
    struct alignment1
    {
        alignas(T...) char a;
    };

    template <int... args>
    struct alignment2
    {
        alignas(args...) char a;
    };

};

int main()
{
    using namespace examples;

    // [[maybe_unused]] outer<int, double, char[5]> a;
    [[maybe_unused]] outer<int, char[5]> a; /// double is not allowed until C++20

    tagger<int, double, char[5], short, float>();

    make_it(42);
    make_it(42, 'a');

    do_sums(1, 2, 3, 4);

    parenthesized(1, 2, 3, 4);

    brace_enclosed(1, 2, 3, 4);

    captures(1, 2, 3, 4);

    auto arr = make_array(1, 2, 3, 4);

    alignment1<int, double> al1;
    al1.a = 'a';

    return 0;
}