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

template <typename T>
class wrapper1
{ /* ... */
};

template <typename T = int> // NOTE: default type parameter
class wrapper2
{ /* ... */
};

template <typename>         // NOTE: T can be omitted
class wrapper3;

template <typename = int> 
class wrapper4;

template <typename... T>    // NOTE: variadic template, type parameter pack
class wrapper5
{ /* ... */
};

#if __cplusplus >= 202002L

template <typename T>
concept WrappableType = std::is_trivial_v<T>;  // NOTE: concept， a new feature in C++20 

template <WrappableType T>
class wrapper6
{ /* ... */
};

template <WrappableType T = int>
class wrapper7
{ /* ... */
};

template <WrappableType... T>
class wrapper8
{ /* ... */
};

#endif

int main()
{
    return 0;
}