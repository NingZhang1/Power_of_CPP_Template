#include <iostream>
#include <array>
#include <string>
#include <cstring>
#include <string_view>
#include <vector>
#include <map>
#include <type_traits>
#include <array>
#include <numeric>
#include <algorithm>
#include <functional>
#include <memory>

namespace ExampleCopy
{
    namespace detail /// detail namespace is used to hide the implementation details
    {
        template <bool b>
        struct copy_fn
        {
            template <typename InputIt, typename OutputIt>
            constexpr static OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
            {
                while (first != last)
                {
                    *d_first++ = *first++;
                }
                return d_first;
            }
        };

        template <>
        struct copy_fn<true>
        {
            template <typename InputIt, typename OutputIt>
            constexpr static OutputIt *copy(InputIt *first, InputIt *last, OutputIt *d_first)
            {
                std::memmove(d_first, first, (last - first) * sizeof(InputIt));
                return d_first + (last - first);
            }
        };
    }

    template <typename InputIt, typename OutputIt>
    constexpr OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
    {
        // c v must be removed first!
        using input_type = std::remove_cv_t<typename std::iterator_traits<InputIt>::value_type>;
        using output_type = std::remove_cv_t<typename std::iterator_traits<OutputIt>::value_type>;

        constexpr bool opt =
            std::is_same_v<input_type, output_type> &&       // judge whether these two types are the same
            std::is_pointer_v<InputIt> &&                    // the input type must be a pointer
            std::is_pointer_v<OutputIt> &&                   // the output type must be a pointer
            std::is_trivially_copy_assignable_v<input_type>; // can be trivially copy assigned!
        // determine when to call memmove !

        return detail::copy_fn<opt>::copy(first, last, d_first);
    }
}

int main()
{
    {
        std::vector<int> v1{1, 2, 3, 4, 5};
        std::vector<int> v2(5);

        ExampleCopy::copy(std::begin(v1), std::end(v1), std::begin(v2)); // the un-optimized version is called

        int a1[5] = {1, 2, 3, 4, 5};
        int a2[5];

        ExampleCopy::copy(a1, a1 + 5, a2); // the optimized version is called
    }
}