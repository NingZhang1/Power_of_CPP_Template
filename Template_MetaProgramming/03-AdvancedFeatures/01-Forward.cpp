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

/// what is lvalues and rvalues?
/// lvalues are objects that occupy some identifiable location in memory (e.g. variables)
/// rvalues are objects that do not occupy some identifiable location in memory (e.g. temporaries), not lvalues
/// hence rvalues cannot take their address by & operator

/// what is lvalue reference and rvalue reference?
/// lvalue reference is a reference that binds to an lvalue &
/// rvalue reference is a reference that binds to an rvalue &&

namespace reference_example
{
    struct foo
    {
        int data;
    };

    void f(foo &v)
    {
        std::cout << "f(foo&)\n";
    }

    void g(foo &v)
    {
        std::cout << "g(foo&)\n";
    }

    void g(foo &&v)
    {
        std::cout << "g(foo&&)\n";
    }

    void h(foo &&v)
    {
        std::cout << "h(foo&&)\n";
    }
}

/// passing rvalues to a function has two purposes:
/// (1) indicating that the object is temporary and the function can do anything with it
/// (2) indicating that the object is no longer needed and the function can steal its resources
/// that is the design purpose for move constructor and move assignment operator

/// issues of perfect  forwarding

namespace forwarding_problem_example
{
    struct foo
    {
        int data;
    };

    void g(foo &v)
    {
        std::cout << "g(foo&)\n";
    }

    void g(foo &&v)
    {
        std::cout << "g(foo&&)\n";
    }

    void h(foo &v)
    {
        g(v);
    }

    void h(foo &&v) // in the case of h(foo&& v), v is actually an lvalue
    {
        g(v); // call g(foo&), not g(foo&&)
    }
}

namespace perfect_forwarding_example
{
    struct foo
    {
        int data;
    };

    void g(foo &v)
    {
        std::cout << "g(foo&)\n";
    }

    void g(foo &&v)
    {
        std::cout << "g(foo&&)\n";
    }

    void h(foo &v)
    {
        g(std::forward<foo &>(v));
    }

    void h(foo &&v)
    {
        g(std::forward<foo &&>(v));
    }
}

namespace forward_rvalue_reference_example
{
    template <typename T>
    void f(T &&arg) // forwarding reference, T&& is an rvalue reference if arg is an rvalue, otherwise it is an lvalue reference
    // it only works from template functions , T&& , without any other thing.
    {
        std::cout << "f(T&&)\n";
    }

    template <typename T>
    void f(T const &&arg) // rvalue reference, not a forwarding reference
    {
        std::cout << "f(T const&&)\n";
    }

    template <typename T>
    void f(std::vector<T> &&arg) // rvalue reference, not a forwarding reference
    {
        std::cout << "f(vector<T>&&)\n";
    }

    template <typename T>
    struct S
    {
        void f(T &&arg) // rvalue reference. it is not a template function but a member function of a template class
        {
            std::cout << "S.f(T&&)\n";
        }
    };
}

int main()
{
    {
        using namespace reference_example;

        foo x = {42}; // x is l-value
        foo &rx = x;  // rx is l-value

        f(x);  // f(foo&)
        f(rx); // f(foo&)
               // f(foo{ 42 });     // error, a non-const reference may only be bound to an lvalue, foo{42} is an rvalue
    }

    {
        using namespace reference_example;

        foo x = {42}; // x is l-value
        foo &rx = x;  // rx is l-value

        g(x);       // g(foo&)
        g(rx);      // g(foo&)
        g(foo{42}); // g(foo&&)
    }

    {
        using namespace reference_example;

        foo x = {42}; // x is l-value
        foo &rx = x;  // rx is l-value

        // h(x);             // error, cannot bind an lvalue to an rvalue reference
        // h(rx);         // error
        h(foo{42});      // h(foo&&)
        h(std::move(x)); // h(foo&&)
    }

    {
        using namespace forwarding_problem_example;

        foo x{42};

        h(x);       // g(foo&)
        h(foo{42}); // g(foo&)
    }

    {
        using namespace perfect_forwarding_example;

        foo x{42};

        h(x);       // g(foo&)
        h(foo{42}); // g(foo&&)
    }

    {
        using namespace forward_rvalue_reference_example;

        int x = 42;
        f(x);  // f(T&&)
        f(42); // f(T&&)

        int const cx = 100;
        f(cx);            // f(T&&)
        f(std::move(cx)); // f(T const&&)

        std::vector<int> v{42};
        f(v);                    // f(T&&)
        f(std::vector<int>{42}); // f(vector<T>&&)

        S<int> s;
        // s.f(x);                 // error
        s.f(42); // S.f(T&&)
    }

    {
        // the mechanism of std::forward
        // reference collapsing rules
        // an rvalue reference to an rvalue reference collapses to an rvalue reference, otherwise it collapses to an lvalue reference
        using lrefint = int &;
        using rrefint = int &&;
        int x = 42;
        lrefint &r1 = x;  // type of r1 is int&
        lrefint &&r2 = x; // type of r2 is int&
        rrefint &r3 = x;  // type of r3 is int&
        rrefint &&r4 = 1; // type of r4 is int&&
    }
}