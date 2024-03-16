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

namespace example_tuple
{
    template <typename T, typename... Ts>
    struct tuple
    {
        tuple(T const &t, Ts const &...ts)
            : value(t), rest(ts...)
        {
        }

        constexpr int size() const { return 1 + rest.size(); }

        T value;
        tuple<Ts...> rest;
    };

    template <typename T>
    struct tuple<T>
    {
        tuple(const T &t)
            : value(t)
        {
        }

        constexpr int size() const { return 1; }

        T value;
    };

    template <size_t N, typename T, typename... Ts>
    struct nth_type : nth_type<N - 1, Ts...>
    {
        static_assert(N < sizeof...(Ts) + 1, "index out of bounds");
    };

    template <typename T, typename... Ts>
    struct nth_type<0, T, Ts...>
    {
        using value_type = T;
    };

    template <size_t N>
    struct getter
    {
        template <typename... Ts>
        static typename nth_type<N, Ts...>::value_type &get(tuple<Ts...> &t)
        {
            return getter<N - 1>::get(t.rest);
        }
    };

    template <>
    struct getter<0>
    {
        template <typename T, typename... Ts>
        static T &get(tuple<T, Ts...> &t)
        {
            return t.value;
        }
    };

    template <size_t N, typename... Ts>
    typename nth_type<N, Ts...>::value_type &get(tuple<Ts...> &t)
    {
        return getter<N>::get(t);
    }
};

int main()
{
    {
        using namespace example_tuple;

        tuple<int> one(42);
        tuple<int, double> two(42, 42.0);
        tuple<int, double, char> three(42, 42.0, 'a');

        std::cout << one.value << '\n';
        std::cout << two.value << ','
                  << two.rest.value << '\n';
        std::cout << three.value << ','
                  << three.rest.value << ','
                  << three.rest.rest.value << '\n';

        std::cout << get<0>(one) << '\n';
        std::cout << get<0>(two) << ','
                  << get<1>(two) << '\n';
        std::cout << get<0>(three) << ','
                  << get<1>(three) << ','
                  << get<2>(three) << '\n';
    }
}