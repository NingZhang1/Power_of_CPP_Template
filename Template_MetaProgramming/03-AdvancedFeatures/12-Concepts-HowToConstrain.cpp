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

//// 1. the ordering of templates with constraints

//// more specific constraints is selected as the best match

namespace ExampleOrdering01
{
    int add(int a, int b)
    {
        return a + b;
    }

    template <typename T>
    T add(T a, T b)
    {
        return a + b;
    }
}

namespace ExampleOrdering02
{
    template <typename T>
    T add(T a, T b)
    {
        return a + b;
    }

    template <typename T>
        requires std::is_integral_v<T>
    T add(T a, T b)
    {
        return a + b;
    }
}

namespace ExampleOrdering03
{
    template <typename T>
        requires(sizeof(T) == 4)
    T add(T a, T b)
    {
        return a + b;
    }

    template <typename T>
        requires std::is_integral_v<T>
    T add(T a, T b)
    {
        return a + b;
    }
}

namespace ExampleOrdering04
{
    template <typename T>
        requires std::is_integral_v<T>
    T add(T a, T b)
    {
        return a + b;
    }

    template <typename T>
        requires std::is_integral_v<T> && (sizeof(T) == 4)
    T add(T a, T b)
    {
        return a + b;
    }
}

namespace ExampleOrdering05
{
    template <typename T>
    concept Integral = std::is_integral_v<T>;

    template <typename T>
        requires Integral<T>
    T add(T a, T b)
    {
        return a + b;
    }

    template <typename T>
        requires Integral<T> && (sizeof(T) == 4)
    T add(T a, T b)
    {
        return a + b;
    }
}

namespace ExampleOrdering06
{
    template <typename T>
    concept Integral = std::is_integral_v<T>;

    template <Integral T>
    T add(T a, T b)
    {
        return a + b;
    }

    template <Integral T>
        requires(sizeof(T) == 4)
    T add(T a, T b)
    {
        return a + b;
    }
}

/// 2. constrain non-template member functions

namespace ExampleNonTemplateMemberFunc
{
    template <typename T>
    struct wrapper
    {
        T value;

        bool operator==(std::string_view str)
            requires std::is_convertible_v<T, std::string_view>
        {
            return value == str;
        }
    };
};

namespace ExampleSFINAE
{
    template <typename T>
    struct wrapper
    {
        T value;

        template <typename U,
                  typename = std::enable_if_t<std::is_copy_constructible_v<U> &&
                                              std::is_convertible_v<U, T>>>
        wrapper(U const &v) : value(v) {}
    };
}

namespace ExampleConcepts
{
    template <typename T>
    struct wrapper
    {
        T value;

        wrapper(T const &v)
            requires std::is_copy_constructible_v<T>
            : value(v)
        {
        }
    };
}

/// 3. constrain class template

namespace ExampleClassTemplate
{
    template <typename T>
    concept Integral = std::is_integral_v<T>;

    template <Integral T>
    struct wrapper
    {
        T value;

        T get() const
        {
            return value;
        }
    };
}

/// 4. constrain variable template and template alias

namespace Example01
{
    template <std::floating_point T>
    constexpr T PI = T(3.1415926535897932385L);
}

namespace Example02
{
    template <std::integral T>
    using integral_vector = std::vector<T>;
}

namespace Example03
{
    template <typename T>
        requires std::integral<T>
    using integral_vector = std::vector<T>;
}

/// 5. anything that can be placed on the right-hand side of the = token in a concept definition can be used in a requires clause

/// but requires requires is not recommended

namespace Example04
{
    template <typename T>
        requires requires(T a, T b) { a + b; } // requires clause with requires expression
    auto add(T a, T b)
    {
        return a + b;
    }
}

/// 6. constrain auto and lambda

namespace Example05
{
    /// generic lambda

    auto lsum = [](auto a, auto b)
    {
        return a + b;
    };

    ///   abbreviated function template
    auto add(auto a, auto b)
    {
        return a + b;
    };

    /// equivalent to

    template <typename T, typename U>
    auto add(T a, U b)
    {
        return a + b;
    }
};

namespace Example06
{
    auto add2 = [](auto a, auto b)
        requires std::is_integral_v<decltype(a)> && std::is_integral_v<decltype(b)>
    {
        return a + b;
    };

    //// constrained version

    auto add(std::integral auto a, std::integral auto b)
    {
        return a + b;
    };

    auto add(std::integral auto... args)
    {
        return (args + ...);
    }

    auto lsum = [](std::integral auto a, std::integral auto b)
    {
        return a + b;
    };
}

int main()
{
    {
        using namespace ExampleOrdering01;

        add(1.0, 2.0); // match the template
        add(1, 2);     // match the non-template function
    }

    {
        using namespace ExampleOrdering02;

        add(1.0, 2.0);
        add(1, 2); // match the template with more specific constraints
    }

    {
        using namespace ExampleOrdering03;

        add((short)1, (short)2);
        // add(1, 2);  // error: ambiguous call
    }

    {
        using namespace ExampleOrdering04;

        add((short)1, (short)2);
        // add(1, 2);
        // it is still an error even if the second template is more specific,
        // because we use type traits rather than concepts
    }

    {
        using namespace ExampleOrdering05;

        add((short)1, (short)2);
        add(1, 2); /// use the concept to resolve the ambiguity
    }
}