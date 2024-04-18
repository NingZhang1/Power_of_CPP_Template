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

/// a very complex example to determine what random_access_iterator is

template <typename T>
concept random_access_iterator =
    std::bidirectional_iterator<T> &&
    std::derived_from<typename std::iterator_traits<T>::iterator_category, std::random_access_iterator_tag> &&
    std::totally_ordered<T> &&
    std::sized_sentinel_for<T, T> &&
    requires(T i, const T j, const std::iter_difference_t<T> n) {
        {
            i += n
        } -> std::same_as<T &>;
        {
            i -= n
        } -> std::same_as<T &>;
        {
            j + n
        } -> std::same_as<T>;
        {
            n + j
        } -> std::same_as<T>;
        {
            j - n
        } -> std::same_as<T>;
        {
            j[n]
        } -> std::same_as<std::iter_reference_t<T>>;
    };

int main()
{
}