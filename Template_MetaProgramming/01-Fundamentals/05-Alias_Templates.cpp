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

// The primary purpose of aliases is to give shorter names to types, especially those that are complex or deeply nested.
// THis can be done with a typedef or a using declaration.

// examples

namespace example_01
{
    // typedef
    typedef int index_t;
    typedef std::vector<std::pair<int, std::string>> NameValueList;
    typedef int (*fn_ptr)(int, char);
};

// which is equivalent to

namespace example_02
{
    using index_t = int;
    using NameValueList = std::vector<std::pair<int, std::string>>;
    using fn_ptr = int (*)(int, char);
};

// A alias template is a template that defines an alias for a family of types.

// example

namespace example_03
{
    template <typename T>
    using customer_addresses_t = std::map<int, std::vector<T>>;

    struct delivery_address_t
    {
    };
    struct invoice_address_t
    {
    };

    using customer_delivery_addresses_t = customer_addresses_t<delivery_address_t>;
    using customer_invoice_addresses_t = customer_addresses_t<invoice_address_t>;
};

// Alias templates can neither by fully nor partially specialized.
// which means that the following is not allowed:

namespace example_04
{
    template <typename T, size_t S>
    using list_t = std::vector<T>;

    // template <typename T>
    // using list_t<T, 1> = T; // error: partial specialization of alias template
};

// a possible solution is to use a helper struct

namespace example_05
{
    template <typename T, size_t S>
    struct list
    {
        using type = std::vector<T>;
    };

    template <typename T>
    struct list<T, 1>
    {
        using type = T;
    };

    template <typename T, size_t S>
    using list_t = typename list<T, S>::type;
};

int main()
{
}