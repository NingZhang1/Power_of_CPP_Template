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

/// decltype is a C++11 feature that allows to deduce the type of an expression at compile time
// it can be used in different situation and yields different results

// Rules:
// (1) If the expressions is an identifier or a class member access, then the result
// is the type of the entity that is named by the expression.
// It the entity does not exist or the function is overloaded, then the result is an error.
// (2) If the expression is a function call, then the result is the type of the function
// return value. But it is a function, then the result is T where T is the type of the function.
// (3) If the expression is a lvalue, then the result is T& where T is the type of the lvalue.
// (4) If the expression is sth else, then the result is T where T is the type of the expression.

namespace example
{
    int f() { return 42; }

    int g() { return 0; }
    int g(int a) { return a; }
    int g(int a, int b) { return a + b; }

    struct wrapper
    {
        int val;

        int get() const { return val; }
    };
}

/// further rules on data member access.

/// (1) the const or volatile qualification of the object is ignored, i.e.
/// it does not contribute to the deduced type
/// (2) whether or not the object or pointer expression is an lvalue or an rvalue,
/// does not affect the deduced type
/// (3) It the data access expression is parenthesized, such as
/// decltype((expression)), then the previous two rules do not apply.
/// The deduced type is affected by const or volatile qualification and lvalue or rvalue category.

namespace example2
{
    struct foo
    {
        int a = 0;
        volatile int b = 0;
        const int c = 42;
    };
}

/// decltype in templates, it is its main design purpose used to deduce the return type of a template function

namespace example_returntype_1
{
    template <typename T>
    T minimum(T &&a, T &&b)
    {
        return a < b ? a : b;
    }
    // what if minimum (1, 0.99) ? it will not compile
}

namespace example_returntype_2
{
    template <typename T, typename U>
    auto minimum(T &&a, U &&b) -> decltype(a < b ? a : b)
    {
        return a < b ? a : b;
    }
}

namespace example_returntype_3
{
    template <typename T, typename U>
    decltype(auto) minimum(T &&a, U &&b)
    {
        return a < b ? a : b;
    }
}

namespace example_returntype_4
{
    template <typename T, typename U>
    auto minimum(T &&a, U &&b)
    {
        return a < b ? a : b;
    }
}

/// decltype(auto) is not the same as auto

namespace example_returntype_5
{
    template <typename T>
    T const &func(T const &ref)
    {
        return ref;
    }

    template <typename T>
    auto func_caller(T &&ref)
    {
        return func(std::forward<T>(ref)); 
    }
}

namespace example_returntype_6
{
    template <typename T>
    T const &func(T const &ref)
    {
        return ref;
    }

    template <typename T>
    decltype(auto) func_caller(T &&ref)
    {
        return func(std::forward<T>(ref));
    } /// decltype(auto) is achieve a perfect forwarding of the return type
}

int main()
{
    {
        using namespace example;

        int a = 42;
        int &ra = a;
        const double d = 42.99;
        long arr[10];
        long l = 0;
        char *p = nullptr;
        char c = 'x';
        wrapper w1{1};
        wrapper *w2 = new wrapper{2};

        [[maybe_unused]] decltype(a) e1;      // R1, int
        [[maybe_unused]] decltype(ra) e2 = a; // R1, int&
        [[maybe_unused]] decltype(f) e3;      // R1, int(), f is a function not a functional call
        [[maybe_unused]] decltype(f()) e4;    // R2, int, it is a function call
        //[[maybe_unused]] decltype(g) e5;           // R1, error
        [[maybe_unused]] decltype(g(1)) e6;         // R2, int, a function call
        [[maybe_unused]] decltype(&f) e7 = nullptr; // R4, int(*)()
        [[maybe_unused]] decltype(d) e8 = 1;        // R1, const double
        [[maybe_unused]] decltype(arr) e9;          // R1, long[10]
        [[maybe_unused]] decltype(arr[1]) e10 = l;  // R3, long&, arr[1] is a lvalue
        [[maybe_unused]] decltype(w1.val) e11;      // R1, int
        [[maybe_unused]] decltype(w1.get()) e12;    // R1, int
        [[maybe_unused]] decltype(w2->val) e13;     // R1, int
        [[maybe_unused]] decltype(w2->get()) e14;   // R1, int
        [[maybe_unused]] decltype(42) e15 = 1;      // R4, int
        [[maybe_unused]] decltype(1 + 2) e16;       // R4, int
        [[maybe_unused]] decltype(a + 1) e17;       // R4, int
        [[maybe_unused]] decltype(a = 0) e18 = a;   // R3, int&
        [[maybe_unused]] decltype(p) e19 = nullptr; // R1, char*
        [[maybe_unused]] decltype(*p) e20 = c;      // R3, char&, *p is a lvalue
        [[maybe_unused]] decltype(p[0]) e21 = c;    // R3, char&, p[0] is a lvalue

        delete w2;
    }

    {
        using namespace example2;

        foo f;
        foo const cf;
        volatile foo *pf = &f;

        [[maybe_unused]] decltype(f.a) e1 = 0; // int
        [[maybe_unused]] decltype(f.b) e2 = 0; // int volatile
        [[maybe_unused]] decltype(f.c) e3 = 0; // int const

        [[maybe_unused]] decltype(cf.a) e4 = 0; // int
        [[maybe_unused]] decltype(cf.b) e5 = 0; // int volatile
        [[maybe_unused]] decltype(cf.c) e6 = 0; // int const

        [[maybe_unused]] decltype(pf->a) e7 = 0; // int
        [[maybe_unused]] decltype(pf->b) e8 = 0; // int volatile
        [[maybe_unused]] decltype(pf->c) e9 = 0; // int const

        [[maybe_unused]] decltype(foo{}.a) e10 = 0; // int
        [[maybe_unused]] decltype(foo{}.b) e11 = 0; // int volatile
        [[maybe_unused]] decltype(foo{}.c) e12 = 0; // int const
    }

    {
        using namespace example2;

        foo f;
        foo const cf;
        volatile foo *pf = &f;

        int x = 1;
        int volatile y = 2;
        int const z = 3;

        [[maybe_unused]] decltype((f.a)) e1 = x; // int&
        [[maybe_unused]] decltype((f.b)) e2 = y; // int volatile&
        [[maybe_unused]] decltype((f.c)) e3 = z; // int const&

        [[maybe_unused]] decltype((cf.a)) e4 = x; // int const&
        [[maybe_unused]] decltype((cf.b)) e5 = y; // int const volatile&
        [[maybe_unused]] decltype((cf.c)) e6 = z; // int const&

        [[maybe_unused]] decltype((pf->a)) e7 = x; // int volatile&
        [[maybe_unused]] decltype((pf->b)) e8 = y; // int volatile&
        [[maybe_unused]] decltype((pf->c)) e9 = z; // int const volatile&

        [[maybe_unused]] decltype((foo{}.a)) e10 = 0; // int&&
        [[maybe_unused]] decltype((foo{}.b)) e11 = 0; // int volatile&&
        [[maybe_unused]] decltype((foo{}.c)) e12 = 0; // int const&&
    }

    {
        int a = 0;
        int &ra = a;
        int const c = 42;
        int volatile d = 99;

        [[maybe_unused]] decltype(ra) &e1 = a;        // int&
        [[maybe_unused]] decltype(c) const e2 = 1;    // int const
        [[maybe_unused]] decltype(d) volatile e3 = 1; // int volatile
    }

    {
        using namespace example_returntype_1;
        auto m1 = minimum(1, 5);        // OK
        auto m2 = minimum(18.49, 9.99); // OK

        // auto m3 = minimum(1, 9.99);      // error, arguments of different type
    }

    {
        using namespace example_returntype_2;
        auto m1 = minimum(1, 5);        // OK
        auto m2 = minimum(18.49, 9.99); // OK
        auto m3 = minimum(1, 9.99);     // OK
    }

    {
        using namespace example_returntype_3;
        auto m1 = minimum(1, 5);        // OK
        auto m2 = minimum(18.49, 9.99); // OK
        auto m3 = minimum(1, 9.99);     // OK
    }

    {
        using namespace example_returntype_4;
        auto m1 = minimum(1, 5);        // OK
        auto m2 = minimum(18.49, 9.99); // OK
        auto m3 = minimum(1, 9.99);     // OK
    }

    {
        using namespace example_returntype_5;

        int a = 42;

        decltype(func(a)) r1 = func(a);               // int const&
        decltype(func_caller(a)) r2 = func_caller(a); // int, not int const& !!! 
    }

    {
        using namespace example_returntype_6;

        int a = 42;

        decltype(func(a)) r1 = func(a);               // int const&
        decltype(func_caller(a)) r2 = func_caller(a); // int const&
    }
}