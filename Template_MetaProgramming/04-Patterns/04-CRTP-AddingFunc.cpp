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
    // suppose we can to do the following things, for both of the classes knight and mage:

    // struct knight
    // {
    //     void step_forth();
    //     void step_back();
    // };

    // struct mage
    // {
    //     void step_forth();
    //     void step_back();
    // };

    // we can use the CRTP to add the same functionality to both of the classes

    template <typename T>
    struct movable_unit
    {
        void advance(size_t steps)
        {
            while (steps--)
                static_cast<T *>(this)->step_forth();
        }

        void retreat(size_t steps)
        {
            while (steps--)
                static_cast<T *>(this)->step_back();
        }
    };

    struct knight : movable_unit<knight>
    {
        void step_forth()
        {
            std::cout << "knight moves forward\n";
        }

        void step_back()
        {
            std::cout << "knight moves back\n";
        }
    };

    struct mage : movable_unit<mage>
    {
        void step_forth()
        {
            std::cout << "mage moves forward\n";
        }

        void step_back()
        {
            std::cout << "mage moves back\n";
        }
    };

};

int main()
{
    using namespace examples;

    knight k;
    mage m;

    k.advance(3);
    m.advance(2);
    k.retreat(1);
    m.retreat(2);
}
