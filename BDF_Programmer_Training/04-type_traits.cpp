/*
 * @Author: Ning Zhang
 * @Date: 2024-11-29 19:43:21
 * @Last Modified by: Ning Zhang
 * @Last Modified time: 2024-11-29 20:02:34
 */

#include <iostream>
#include <type_traits>

int main()
{
    static_assert(std::is_integral<int>::value, "int is integral");
    static_assert(std::is_integral_v<int>, "int is integral");
    static_assert(std::is_floating_point<float>::value, "float is floating point");
    static_assert(std::is_floating_point_v<float>, "float is floating point");
    static_assert(std::is_same<int, int>::value, "int is same as int");
    static_assert(std::is_same_v<int, int>, "int is same as int");
    static_assert(!std::is_same_v<int, float>, "int is not same as float");
}