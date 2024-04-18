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

/// Concepts in C++20 are used to constrain templates in an elegant way.

/// One way to constrain templates is to use SFINAE (Substitution Failure Is Not An Error) technique.
/// but it is not easy to read and write.
/// another way is to use requires clause, but it is not flexible.

namespace ExamplesRequires01
{
    template <typename T>
        requires std::is_arithmetic_v<T>
    T add(T const a, T const b)
    {
        return a + b;
    }
}

namespace ExamplesRequires01
{
    template <typename T>
    T add(T const a, T const b)
        requires std::is_arithmetic_v<T>
    {
        return a + b;
    }
}

/// or we can use concepts

namespace ExamplesConcepts01
{
    template <typename T>
    concept arithmetic = requires { std::is_arithmetic_v<T>; };

    template <arithmetic T>
    T add(T const a, T const b)
    {
        return a + b;
    }
}

/// Hence concepts are defined as **requires expressions** that can be used to constrain templates.

/// a more complex example

/// defining a concept that checks whether a type is a container

namespace ExamplesConcepts02
{
    template <typename T, typename U = void>
    struct is_container : std::false_type
    {
    };

    template <typename T>
    struct is_container<T,
                        std::void_t<typename T::value_type, /// check whether T has a member type value_type
                                    typename T::size_type,
                                    typename T::allocator_type,
                                    typename T::iterator,
                                    typename T::const_iterator,
                                    decltype(std::declval<T>().size()), /// check whether T has a member function size()
                                    decltype(std::declval<T>().begin()),
                                    decltype(std::declval<T>().end()),
                                    decltype(std::declval<T>().cbegin()),
                                    decltype(std::declval<T>().cend())>> /// SFINAE !
        : std::true_type
    {
    };

    template <typename T, typename U = void>
    constexpr bool is_container_v = is_container<T, U>::value;

    struct foo
    {
    };

    static_assert(!is_container_v<foo>);
    static_assert(is_container_v<std::vector<foo>>);
}

int main()
{
}