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

// mixin design pattern
// adding functionality to a class without inheritance (without being the base class)

namespace example
{
    struct knight
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

    struct mage
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

    template <typename T>
    struct movable_unit : T
    {
        void advance(size_t steps)
        {
            while (steps--)
                T::step_forth();
        }

        void retreat(size_t steps)
        {
            while (steps--)
                T::step_back();
        }
    };
};

// it is possible to combine the static polymorphism with the mixin design pattern and the dynamic polymorphism
// with interfaces and virtual functions

namespace examples2
{
    /// define lots of different styles of mixins, so that we can combine them in different ways, resulting in different classes

    struct aggressive_style
    {
        void fight()
        {
            std::cout << "attack! attack attack!\n";
        }
    };

    struct moderate_style
    {
        void fight()
        {
            std::cout << "attack then defend\n";
        }
    };

    template <typename T>
    struct lone_warrior : T
    {
        void fight()
        {
            std::cout << "fighting alone.";
            T::fight();
        }
    };

    template <typename T>
    struct team_warrior : T
    {
        void fight()
        {
            std::cout << "fighting with a team.";
            T::fight();
        }
    };

    struct game_unit
    {
        virtual void attack() = 0;
        virtual ~game_unit() = default;
    };

    template <typename T>
    struct knight : T, game_unit
    {
        void attack()
        {
            std::cout << "draw sword.";
            T::fight();
        }
    };

    template <typename T>
    struct mage : T, game_unit
    {
        void attack()
        {
            std::cout << "spell magic curse.";
            T::fight();
        }
    };
};

int main()
{
    {
        using namespace example;

        movable_unit<knight> k;
        k.advance(3);
        k.retreat(2);

        movable_unit<mage> m;
        m.advance(2);
        m.retreat(1);
    }

    {
        using namespace examples2;

        std::vector<std::unique_ptr<game_unit>> units;

        units.emplace_back(new knight<aggressive_style>());
        units.emplace_back(new knight<moderate_style>());
        units.emplace_back(new mage<aggressive_style>());
        units.emplace_back(new mage<moderate_style>());
        units.emplace_back(new knight<lone_warrior<aggressive_style>>());
        units.emplace_back(new knight<lone_warrior<moderate_style>>());
        units.emplace_back(new knight<team_warrior<aggressive_style>>());
        units.emplace_back(new knight<team_warrior<moderate_style>>());
        units.emplace_back(new mage<lone_warrior<aggressive_style>>());
        units.emplace_back(new mage<lone_warrior<moderate_style>>());
        units.emplace_back(new mage<team_warrior<aggressive_style>>());
        units.emplace_back(new mage<team_warrior<moderate_style>>());

        for (auto &u : units)
            u->attack();
    }
}