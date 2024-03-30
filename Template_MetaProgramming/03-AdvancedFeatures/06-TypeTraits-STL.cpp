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

// Categories of Type Traits

// 1. Querying the type category

/// is_void
/// is_null_pointer
/// is_integral
/// is_floating_point
/// is_arithmetic
/// is_array
/// is_enum
/// is_union
/// is_class
/// is_function
/// is_pointer
/// is_member_pointer
/// is_member_object_pointer
/// is_member_function_pointer
/// is_lvalue_reference
/// is_rvalue_reference
/// is_reference

/// is_fundamental
/// is_scalar
/// is_object
/// is_compound

/// _v is a variable template that is used to get the value of the type trait.

namespace Example01
{
    template <class T>
    constexpr bool always_false = std::false_type::value;

    template <typename T>
    std::string as_string(T &&value)
    {
        using value_type = std::decay_t<T>;
        // using value_type = T;

        if constexpr (std::is_null_pointer_v<value_type>)
            return "null";
        else if constexpr (std::is_same_v<value_type, bool>)
            return value ? "true" : "false";
        else if constexpr (std::is_arithmetic_v<value_type>)
            return std::to_string(value);
        else
            static_assert(always_false<T>);
    }
};

// 2. querying type properties

/// is_const
/// is_volatile
/// is_trivial
/// is_trivially_copyable
/// is_standard_layout
/// is_pod
/// is_empty
/// is_polymorphic
/// is_abstract
/// is_final
/// is_aggregate
/// is_signed
/// is_unsigned
/// is_bounded_array
/// is_unbounded_array
/// is_scoped_enum
/// has_unique_object_representations

namespace Example02
{
    struct foo
    {
        int a;
    };

    struct bar
    {
        int a = 0;
    };

    struct tar
    {
        int a = 0;
        tar() : a(0) {}
    };
};

// 3. querying supported operations

/// is_constructible
/// is_trivially_constructible
/// is_nothrow_constructible
/// is_default_constructible
/// is_trivially_default_constructible
/// is_nothrow_default_constructible
/// is_copy_constructible
/// is_trivially_copy_constructible
/// is_nothrow_copy_constructible
/// is_move_constructible
/// is_trivially_move_constructible
/// is_nothrow_move_constructible
/// is_assignable
/// is_trivially_assignable
/// is_nothrow_assignable
/// is_copy_assignable
/// is_trivially_copy_assignable
/// is_nothrow_copy_assignable
/// is_move_assignable
/// is_trivially_move_assignable
/// is_nothrow_move_assignable
/// is_destructible
/// is_trivially_destructible
/// is_nothrow_destructible
/// has_virtual_destructor
/// is_swappable_with
/// is_swappable
/// is_nothrow_swappable_with
/// is_nothrow_swappable

// 4. querying type relationships

/// is_same (including cv-qualifiers)
/// is_base_of
/// is_convertible
/// is_nothrow_convertible
/// is_invocable
/// is_invocable_r
/// is_nothrow_invocable
/// is_nothrow_invocable_r
/// is_layout_compatible
/// is_pointer_interconvertible_base_of

// 5. modifying cv-qualifiers, references, pointers, or a sign

/// add_cv
/// add_const
/// add_volatile
/// remove_cv
/// remove_const
/// remove_volatile
/// add_lvalue_reference
/// add_rvalue_reference
/// remove_reference
/// remove_cvref
/// add_pointer
/// remove_pointer
/// make_signed
/// make_unsigned
/// remove_extent
/// remove_all_extents

// 6. transforming types

/// enable_if
/// conditional
/// decay
/// common_type
/// common_reference
/// underlying_type
/// void_t
/// type_identity

namespace Example03
{
    template <typename T, size_t S>
    using list_t = typename std::conditional<S == 1, T, std::vector<T>>::type;
    /// using list_t = std::conditional_t<S == 1, T, std::vector<T>>; /// the same as above, in C++14
    /// the above trick is used to define a type alias that is either T or std::vector<T> based on the value of S.
    /// it avoids the need to define a new type trait as the specialization of alias templates is not allowed.
}

int main()
{
    {
        using namespace Example01;
        std::cout << as_string(nullptr) << '\n';
        std::cout << as_string(true) << '\n';
        std::cout << as_string('a') << '\n';
        std::cout << as_string(42) << '\n';
        std::cout << as_string(42.0) << '\n';

        // bool f = true;
        // std::cout << as_string(f) << '\n';

        int n = 42;
        std::cout << as_string(n) << '\n'; // error if without removing reference, because n is an lvalue reference.
    }

    {
        using namespace Example02;

        std::cout << std::boolalpha;
        std::cout << std::is_trivial_v<foo> << '\n';
        std::cout << std::is_trivial_v<bar> << '\n';
        std::cout << std::is_trivial_v<tar> << '\n';

        std::cout << std::is_trivially_copyable_v<foo> << '\n';
        std::cout << std::is_trivially_copyable_v<bar> << '\n';
        std::cout << std::is_trivially_copyable_v<tar> << '\n';
    }

    {
        using namespace Example03;

        static_assert(std::is_same_v<list_t<int, 1>, int>);
        static_assert(std::is_same_v<list_t<int, 2>, std::vector<int>>);
    }
}