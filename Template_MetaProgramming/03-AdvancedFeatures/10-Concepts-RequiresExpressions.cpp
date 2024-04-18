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

/// the syntax of requires expressions is similar to function syntax

/// requires (parameter-list) { expression; }

/// parameter-list is a comma-separated list of parameters, default values are not allowed.
/// The parameters in this list do not have storage, linkage or lifetime, the compiler does not allocate memory for them.

/// In C++ there are four types of requires expressions:

/// 1. Simple requirements

/// Simple requirements are used to check the validity of an expression.

namespace ExampleSimpleRequirements
{
    template <typename T>
    concept arithmetic = requires {
        std::is_arithmetic_v<T>;
    };

    template <typename T>
    concept addable = requires(T a, T b) {
        a + b;
    };

    template <typename T>
    concept logger = requires(T t) {
        t.error("just");
        t.warning("a");
        t.info("demo"); /// the str "just", "a", "demo" are not used in the function body, they does not matter.
    };

    template <logger T>
    void log_error(T &logger)
    {
    }

    struct console_logger
    {
        void error(std::string_view text) {}
        void warning(std::string_view text) {}
        void info(std::string_view text) {}
    };

    struct stream_logger
    {
        void error(std::string_view text, bool = false) {}
        void warning(std::string_view text, bool = false) {}
        void info(std::string_view text, bool) {}
    };
}

/// 2. Type requirements

/// used to

/// (a) verify that a nested type exists
/// (b) verify that a class template specialization names a type
/// (c) verify that an alias template specialization names a type

namespace ExampleTypeRequirements
{
    template <typename T>
    concept KVP = requires {
        typename T::key_type;
        typename T::value_type;
    };

    template <typename T, typename V>
    struct key_value_pair
    {
        using key_type = T;
        using value_type = V;

        key_type key;
        value_type value;
    };

    static_assert(KVP<key_value_pair<int, std::string>>);
    static_assert(!KVP<std::pair<int, std::string>>);

    /// the following examples show how to check a template specialization names a type

    template <typename T>
        requires std::is_arithmetic_v<T>
    struct container
    { /* ... */
    };

    template <typename T>
    concept containerizeable = requires {
        typename container<T>;
    };

    static_assert(containerizeable<int>);
    static_assert(!containerizeable<std::string>);
}

/// 3. Compound requirements

/// we can further check whether an expression does not throw exceptions or requirements on the result type.

/// {expression} noexcept -> type_constraint;

/// both noexcept and type_constraint are optional.

namespace ExampleCompoundRequirements
{
    template <typename T>
    void f(T) noexcept {}

    template <typename T>
    void g(T) {}

    template <typename F, typename... T>
    concept NonThrowing = requires(F &&func, T... t) {
        {
            func(t...)
        } noexcept; /// requires that this expression func(t...) does not throw error !
    };

    template <typename F, typename... T>
        requires NonThrowing<F, T...> // not this !
    void invoke(F &&func, T... t)     /// functor !
    {
        func(t...);
    }

    ///// the following examples is more complex

    template <typename T>
    concept timer = requires(T t) {
        {
            t.start()
        } -> std::same_as<void>;
        {
            t.stop()
        } -> std::convertible_to<long long>;
    };

    struct timerA
    {
        void start() {}
        long long stop() { return 0; }
    };

    struct timerB
    {
        void start() {}
        int stop() { return 0; }
    };

    struct timerC
    {
        void start() {}
        void stop() {}
        long long getTicks() { return 0; }
    };

    static_assert(timer<timerA>);
    static_assert(timer<timerB>);
    static_assert(!timer<timerC>);

};

/// 4. Nested requirements, reqrirements in requires expressions can be nested.

namespace ExampleNestedRequirements
{
    template <typename T, typename... Ts>
    inline constexpr bool are_same_v = std::conjunction_v<std::is_same<T, Ts>...>;

    template <typename... T>
    concept HomogenousRange = requires(T... t) {
        (... + t);
        requires are_same_v<T...>; /// nested requirements
        requires sizeof...(T) > 1; /// nested requirements
    };

    template <typename... T>
        requires HomogenousRange<T...>
    auto add(T &&...t)
    {
        return (... + t);
    }

    static_assert(HomogenousRange<int, int>);
    static_assert(!HomogenousRange<int>);
    static_assert(!HomogenousRange<int, double>);
};

int main()
{
    {
        using namespace ExampleSimpleRequirements;

        console_logger cl;
        log_error(cl);

        [[maybe_unused]] stream_logger sl;
        // log_error(sl); // error: the associated constraints are not satisfied
    }

    {
        {
            using namespace ExampleCompoundRequirements;

            invoke(f<int>, 42);
            // invoke(g<int>, 42); // error
        }
    }
}