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

namespace examples
{
    template <typename T, size_t N>
    struct limited_instances
    {
        static std::atomic<size_t> count;
        limited_instances()
        {
            if (count >= N)
                throw std::logic_error{"Too many instances"};
            ++count;
        }
        ~limited_instances() { --count; }
    };

    template <typename T, size_t N>
    std::atomic<size_t> limited_instances<T, N>::count = 0;

    struct excalibur : limited_instances<excalibur, 1>
    {
    };
    struct book_of_magic : limited_instances<book_of_magic, 3>
    {
    };
};

int main()
{
    using namespace examples;

    excalibur e1;
    try
    {
        excalibur e2;
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << '\n';
    }

    book_of_magic b1;
    book_of_magic b2;
    book_of_magic b3;
    try
    {
        book_of_magic b4;
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << '\n';
    }
}
