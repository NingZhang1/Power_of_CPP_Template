#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <string>
#include <string_view>
#include <memory>
#include <functional>
#include <future>
#include <thread>
#include <chrono>
#include <concepts>
#include <any>
#include <ranges>

// typelist
// a typelist is a compile-time construct that enables us to manage a list of types
// in some sense, it is similar to a std::tuple, but it is a compile-time construct, does not hold any data

// most of the problem solved by typelists can be solved by using variadic templates !

// but there still are some problems that can be solved easier by using typelists
// see the following meta functions

namespace examples_simple
{
    /// if you want to transform a list of type and write like this
    /// template <typename... Ts>
    /// struct transformer
    /// {
    ///     using input_types = Ts...;
    ///     using output_types = std::add_const_t<Ts>...;
    /// };
    /// this won't work because of the pack expansion does not work here!

    template <typename... Ts>
    struct typelist
    {
    };

    template <typename... Ts>
    struct transformer
    {
        using input_types = typelist<Ts...>;
        using output_types = typelist<std::add_const_t<Ts>...>;
    };

    static_assert(
        std::is_same_v<
            transformer<int, double>::output_types,
            typelist<int const, double const>>); // true!
};

namespace TypeLists
{
    template <typename... Ts>
    struct typelist
    {
    };

    struct empty_type
    {
    };

    //// operations on typelists, it is a kind of template meta programming, define meta functions
    //// to operate on typelists

    namespace detail
    {
        template <typename TL>
        struct length;

        template <template <typename...> typename TL, typename... Ts>
        struct length<TL<Ts...>>
        {
            using type = std::integral_constant<std::size_t, sizeof...(Ts)>;
        };

        template <typename TL>
        struct front_type;

        template <template <typename...> typename TL, typename T, typename... Ts>
        struct front_type<TL<T, Ts...>>
        {
            using type = T;
        };

        template <template <typename...> typename TL>
        struct front_type<TL<>>
        {
            using type = empty_type;
        };

        template <typename TL>
        struct back_type;

        template <template <typename...> typename TL, typename T, typename... Ts>
        struct back_type<TL<T, Ts...>>
        {
            using type = back_type<TL<Ts...>>::type;
        };

        template <template <typename...> typename TL, typename T>
        struct back_type<TL<T>>
        {
            using type = T;
        };

        template <template <typename...> typename TL>
        struct back_type<TL<>>
        {
            using type = empty_type;
        };

        template <typename TL, typename T>
        struct push_back_type;

        template <template <typename...> typename TL, typename T, typename... Ts>
        struct push_back_type<TL<Ts...>, T>
        {
            using type = TL<Ts..., T>;
        };

        template <typename TL, typename T>
        struct push_front_type;

        template <template <typename...> typename TL, typename T, typename... Ts>
        struct push_front_type<TL<Ts...>, T>
        {
            using type = TL<T, Ts...>;
        };

        template <typename TL>
        struct pop_front_type;

        template <template <typename...> typename TL, typename T, typename... Ts>
        struct pop_front_type<TL<T, Ts...>>
        {
            using type = TL<Ts...>;
        };

        template <template <typename...> typename TL>
        struct pop_front_type<TL<>>
        {
            using type = TL<>;
        };

        template <std::ptrdiff_t N, typename R, typename TL>
        struct pop_back_type;

        template <std::ptrdiff_t N, typename... Ts, typename U, typename... Us>
        struct pop_back_type<N, typelist<Ts...>, typelist<U, Us...>>
        {
            using type = typename pop_back_type<N - 1, typelist<Ts..., U>, typelist<Us...>>::type;
        };

        template <typename... Ts, typename... Us>
        struct pop_back_type<0, typelist<Ts...>, typelist<Us...>>
        {
            using type = typelist<Ts...>;
        };

        template <typename... Ts, typename U, typename... Us>
        struct pop_back_type<0, typelist<Ts...>, typelist<U, Us...>>
        {
            using type = typelist<Ts...>;
        };

        template <>
        struct pop_back_type<-1, typelist<>, typelist<>>
        {
            using type = typelist<>;
        };

        template <std::size_t I, std::size_t N, typename TL>
        struct at_type;

        template <std::size_t I, std::size_t N, template <typename...> typename TL, typename T, typename... Ts>
        struct at_type<I, N, TL<T, Ts...>>
        {
            using type = std::conditional_t<I == N, T, typename at_type<I, N + 1, TL<Ts...>>::type>;
        };

        template <std::size_t I, std::size_t N>
        struct at_type<I, N, typelist<>>
        {
            using type = empty_type;
        };
    }

    template <typename TL>
    using length_t = typename detail::length<TL>::type;

    template <typename TL>
    constexpr std::size_t length_v = length_t<TL>::value; /// alias template

    static_assert(length_t<typelist<int, double, char>>::value == 3);
    static_assert(length_v<typelist<int, double, char>> == 3);
    static_assert(length_v<typelist<int, double>> == 2);
    static_assert(length_v<typelist<int>> == 1);

    template <typename TL>
    using front_t = typename detail::front_type<TL>::type;

    static_assert(std::is_same_v<front_t<typelist<>>, empty_type>);
    static_assert(std::is_same_v<front_t<typelist<int>>, int>);
    static_assert(std::is_same_v<front_t<typelist<int, double, char>>, int>);

    template <typename TL>
    using back_t = typename detail::back_type<TL>::type;

    static_assert(std::is_same_v<back_t<typelist<>>, empty_type>);
    static_assert(std::is_same_v<back_t<typelist<int>>, int>);
    static_assert(std::is_same_v<back_t<typelist<int, double, char>>, char>);

    template <typename TL, typename T>
    using push_back_t = typename detail::push_back_type<TL, T>::type;

    static_assert(std::is_same_v<push_back_t<typelist<>, int>, typelist<int>>);
    static_assert(std::is_same_v<push_back_t<typelist<char>, int>, typelist<char, int>>);
    static_assert(std::is_same_v<push_back_t<typelist<double, char>, int>, typelist<double, char, int>>);

    template <typename TL, typename T>
    using push_front_t = typename detail::push_front_type<TL, T>::type;

    static_assert(std::is_same_v<push_front_t<typelist<>, int>, typelist<int>>);
    static_assert(std::is_same_v<push_front_t<typelist<char>, int>, typelist<int, char>>);
    static_assert(std::is_same_v<push_front_t<typelist<double, char>, int>, typelist<int, double, char>>);

    template <typename TL>
    using pop_front_t = typename detail::pop_front_type<TL>::type;

    static_assert(std::is_same_v<pop_front_t<typelist<>>, typelist<>>);
    static_assert(std::is_same_v<pop_front_t<typelist<char>>, typelist<>>);
    static_assert(std::is_same_v<pop_front_t<typelist<double, char>>, typelist<char>>);

    template <typename TL>
    using pop_back_t = typename detail::pop_back_type<static_cast<std::ptrdiff_t>(length_v<TL>) - 1, typelist<>, TL>::type;

    static_assert(std::is_same_v<pop_back_t<typelist<>>, typelist<>>);
    static_assert(std::is_same_v<pop_back_t<typelist<double>>, typelist<>>);
    static_assert(std::is_same_v<pop_back_t<typelist<double, char>>, typelist<double>>);
    static_assert(std::is_same_v<pop_back_t<typelist<double, char, int>>, typelist<double, char>>);

    template <std::size_t I, typename TL>
    using at_t = typename detail::at_type<I, 0, TL>::type;

    static_assert(std::is_same_v<at_t<0, typelist<>>, empty_type>);
    static_assert(std::is_same_v<at_t<0, typelist<int>>, int>);
    static_assert(std::is_same_v<at_t<0, typelist<int, char>>, int>);

    static_assert(std::is_same_v<at_t<1, typelist<>>, empty_type>);
    static_assert(std::is_same_v<at_t<1, typelist<int>>, empty_type>);
    static_assert(std::is_same_v<at_t<1, typelist<int, char>>, char>);

    static_assert(std::is_same_v<at_t<2, typelist<>>, empty_type>);
    static_assert(std::is_same_v<at_t<2, typelist<int>>, empty_type>);
    static_assert(std::is_same_v<at_t<2, typelist<int, char>>, empty_type>);

};

namespace examples_typical
{
    /// suppose that we want to apply operations on a given struct in many ways
    /// how to express the combination of operations in a type-safe way ?

    struct game_unit
    {
        int attack;
        int defense;
    };

    struct upgrade_defense
    {
        void operator()(game_unit &u)
        {
            u.defense = static_cast<int>(u.defense * 1.2);
        }
    };

    struct upgrade_attack
    {
        void operator()(game_unit &u)
        {
            u.attack += 2;
        }
    };

    using namespace TypeLists;

    template <typename TL>
    struct apply_functors /// a functor can be used !
    {
    private:
        template <size_t I>
        static void apply(game_unit &unit)
        {
            using F = at_t<I, TL>;
            std::invoke(F{}, unit);
        }

        template <size_t... I>
        static void apply_all(game_unit &unit, std::index_sequence<I...>)
        {
            (apply<I>(unit), ...);
        }

    public:
        void operator()(game_unit &unit) const
        {
            apply_all(unit, std::make_index_sequence<length_v<TL>>{});
        }
    };

    void upgrade_unit(game_unit &unit)
    {
        using upgrade_types = typelist<upgrade_defense, upgrade_attack>;
        apply_functors<upgrade_types>{}(unit); /// excellent!, there indeed create a object, but reload its operator() to do the work
    }

}; // namespace examples_typical

int main()
{
    {
        using namespace examples_typical;

        game_unit u{100, 50};
        // std::cout << std::format("{},{}\n", u.attack, u.defense);
        printf("%d,%d\n", u.attack, u.defense);

        upgrade_unit(u);
        // std::cout << std::format("{},{}\n", u.attack, u.defense);
        printf("%d,%d\n", u.attack, u.defense);
    }
}