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

// SFINAE stands for Substitution Failure Is Not An Error
// It is a feature of C++ template system that allows the compiler to discard certain template specializations
// that would otherwise result in a compilation error.
// Which means that when the compiler tries to instantiate a template with a certain type, if the substitution
// of the type in the template results in an error, the compiler will discard this specialization rather than raise an error.

// SFINAE only applies in the so-called immediate context of a function, which is basically the template declaration,
// rather than the funciton body.

namespace BasicExample
{
    template <typename T>
    auto begin(T &c) { return c.begin(); }

    template <typename T, size_t N>
    T *begin(T (&arr)[N]) { return arr; }

    template <typename T>
    void increment(T &val) { val++; }
};

// errors considered as SFINAE error
// 1. creating an array of void, an array of refercence, or an array of functions, and array of negative size,
// an array of size 0 and an array of non-integral size.
// 2. Using a type that is not a class or enum on the left side of the scope resolution operator::
// 3. Creating a pointer to reference
// 4. Creating a reference of void
// 5. Creating a pointer to member of T, where T is not a class type
// 6. using a member of a type when the type does not contain that member ### NOTE: can be used to check if a type has a member or not
// 7. Using a member of a type where a type is required but the member is not a type
// 8. using a member of a type where a template is required but the member is not a template
// 9. Using a member of a type where a non-type is required but the member is not a non-type
// 10. creating a function type with a parameter of type void
// 11. creating a function type that returns an array type or another function type
// 12. performing an invalid conversion in a template argument expression or an expression used in a function declaration
// 13. Supplying an invalid type to a non-type template parameter
// 14. Instantiating a pack expansion containing mulitple packs of different lengths

namespace AdvancedExample
{
    template <typename T>
    struct foo
    {
        using foo_type = T;
    };

    template <typename T>
    struct bar
    {
        using bar_type = T;
    };

    struct int_foo : foo<int>
    {
    };
    struct int_bar : bar<int>
    {
    };

    /// comma operator means the first argument is evaluated but then discarded
    /// hence the deduce type is always the second argument, which is void in this case
    template <typename T>
    decltype(typename T::foo_type(), void()) handle(T const &v)
    {
        std::cout << "handle a foo\n";
    }

    template <typename T>
    decltype(typename T::bar_type(), void()) handle(T const &v)
    {
        std::cout << "handle a bar\n";
    }
};

///// A better way to enable SFINAE is to use the std::enable_if_t type trait

///// type support library in C++, provides a set of type traits that can be used to query the properties of types
///// or perform type transformations (metafunctions). These type traits are defined in the <type_traits> header file.

namespace EnableIfExamples1
{
    struct widget
    {
        int id;
        std::string name;

        std::ostream &write(std::ostream &os) const
        {
            os << id << ',' << name << '\n';
            return os;
        }
    };
    template <typename T>
    struct uses_write
    {
        static constexpr bool value = false;
    };

    template <>
    struct uses_write<widget>
    {
        static constexpr bool value = true;
    };
    template <typename T>
    inline constexpr bool uses_write_v = uses_write<T>::value;

    // template <bool>
    // struct serializer
    // {
    //     template <typename T>
    //     static void serialize(std::ostream &os, T const &value)
    //     {
    //         os << value;
    //     }
    // };
    // template <>
    // struct serializer<true>
    // {
    //     template <typename T>
    //     static void serialize(std::ostream &os, T const &value)
    //     {
    //         value.write(os);
    //     }
    // };
    // template <typename T>
    // void serialize(std::ostream &os, T const &value)
    // {
    //     serializer<uses_write_v<T>>::serialize(os, value);
    // }

    /// the above code is equivalent ot

    /// if enable_if<true>, then type is void by default

    template <typename T>
    typename std::enable_if<uses_write_v<T>>::type serialize(std::ostream &os, T const &value)
    {
        value.write(os);
    }

    template <typename T>
    typename std::enable_if<!uses_write_v<T>>::type serialize(std::ostream &os, T const &value)
    {
        os << value;
    }

    //// or equivalent to

    // template <typename T>
    // void serialize(std::ostream &os, T const &value, typename std::enable_if<uses_write_v<T>>::type * = nullptr)
    // {
    //     value.write(os);
    // }
    // template <typename T>
    // void serialize(std::ostream &os, T const &value, typename std::enable_if<!uses_write_v<T>>::type * = nullptr)
    // {
    //     os << value;
    // }

};

/// enable_if can also be used to restrict instantiation of a template based on the value of a boolean expression.

namespace EnableIFExamples2
{
    template <typename T, typename = typename std::enable_if_t<std::is_integral_v<T>>>
    struct integral_wrapper
    {
        T value;

        integral_wrapper(T v) : value(v) {}
    };

    template <typename T, typename = typename std::enable_if_t<std::is_floating_point_v<T>>>
    struct floating_wrapper
    {
        T value;

        floating_wrapper(T v) : value(v) {}
    };
}

int main()
{
    {
        std::array<int, 5> arr1{1, 2, 3, 4, 5};
        std::cout << *BasicExample::begin(arr1) << '\n';

        int arr2[]{5, 4, 3, 2, 1};
        std::cout << *BasicExample::begin(arr2) << '\n';
    }

    {
        int a = 42;
        BasicExample::increment(a); // OK

        std::string s{"42"};
        // n503::increment(s);  // error, because the compiler tries to increment a string, operator++ is not defined for strings !
    }

    {
        using namespace AdvancedExample;

        int_foo fi;
        int_bar bi;
        int x = 0;
        handle(fi);
        handle(bi);
        // handle(x); // error !
    }

    {
        using namespace EnableIFExamples2;

        integral_wrapper w1{42}; // OK
        // integral_wrapper w2{ 42.0 }; // error
        // integral_wrapper w3{ "42" }; // error

        // floating_wrapper w4{ 42 };   // error
        floating_wrapper w5{42.0}; // OK
                                   // floating_wrapper w6{ "42" }; // error
    }
}