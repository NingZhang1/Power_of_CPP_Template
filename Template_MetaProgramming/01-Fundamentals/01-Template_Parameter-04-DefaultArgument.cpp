
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

// the rule of default argument

// 1. they can be used with any kind of template parameter except for parameter pack

// some examples

template <typename T = int>
class foo
{
};

template <typename T = int, typename U = double>
class bar
{
};

// template <typename T = int, typename U> ## wrong because the default argument must be the last parameter

// default template arguments is that the same template parameter
// cannot be given multiple defaults in the same scope.

int main()
{
}