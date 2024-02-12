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

// Definition of non-type template parameter :
// A non-type template parameter is template parameter that is a value rather than a type, which
// can be an integer, a pointer, a reference, or an enumeration.
// The value of a non-type template parameter must be known at compile time.
// This category of parameters can only have a structural type, which includes
// 1. integral types
// 2. float-point types, starting from C++20
// 3. enumeration types
// 4. pointer types (either to object or to function)
// 5. pointer to member types (eith to member object or to member function)
// 6. Lvalue reference types
// 7. a literal class that meets the following requirements:
//    a. all base classes are public and non-mutable
//    b. all non-static data members are public and non-mutable
//    c. the types of all base classes and non-static data members are structural types or arrays of structural types
// 8. cv-qualified versions of the above types

// a concrete examples

template <typename T, size_t S>
class buffer
{
    T data_[S];

public:
    constexpr const T *data() const { return data_; }

    T &operator[](size_t const index)
    {
        return data_[index];
    }

    const T &operator[](size_t const index) const
    {
        return data_[index];
    }
};

template <typename T, size_t S>
buffer<T, S> make_buffer()
{
    return {};
}

int main()
{
    buffer<int, 10> b1;
    b1[0] = 42;
    std::cout << b1[0] << '\n';

    auto b2 = make_buffer<int, 10>();

    std::cout << std::is_same_v< buffer<int, 10>, buffer<int, 2 * 5> > << '\n';

    buffer<int, 3 * 5> b3;

    static_assert(std::is_same_v<decltype(b2), buffer<int, 10>>);
    static_assert(std::is_same_v<decltype(b1), decltype(b2)>);
    static_assert(!std::is_same_v<decltype(b1), decltype(b3)>);
}