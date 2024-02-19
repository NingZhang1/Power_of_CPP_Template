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

// type erasure
// hiding the type of an object behind an interface, so that leverage common functionality from unrelated types

namespace example_CType
{
    // this is a C type of type erasure, using void * and function pointers
    // it is not type safe, and it is not exception safe, and should never be recommended to use in C++

    struct knight
    {
        void attack() { std::cout << "draw sword\n"; }
    };

    struct mage
    {
        void attack() { std::cout << "spell magic curse\n"; }
    };

    void fight_knight(void *k)
    {
        reinterpret_cast<knight *>(k)->attack();
    }

    void fight_mage(void *m)
    {
        reinterpret_cast<mage *>(m)->attack();
    }

    using fight_fn = void (*)(void *);

    void fight(std::vector<std::pair<void *, fight_fn>> const &units)
    {
        for (auto &u : units)
        {
            u.second(u.first);
        }
    }

};

// another way of type erasure is through inheritance and virtual functions,
// we can only operator on the base class

// but sometimes it is impossible, since there may not be the base class, or we can't modify the base class
// sometimes we want to unrelated types in a similar manner (duck typing)

// a possible solution is as following:

namespace examples_Cpp
{
    struct knight
    {
        void attack() { std::cout << "draw sword\n"; }
    };

    struct mage
    {
        void attack() { std::cout << "spell magic curse\n"; }
    };

    struct game_unit /// we create a base class to deal with the unrelated types
    {
        virtual void attack() = 0;
        virtual ~game_unit() = default;
    };

    template <typename T>
    struct game_unit_wrapper : public game_unit
    {
        game_unit_wrapper(T &unit) : t(unit) {}

        void attack() override { t.attack(); }

    private:
        T &t; /// only hold a reference to the original object!!!
    };

    void fight(std::vector<game_unit *> const &units)
    {
        for (auto u : units)
            u->attack();
    }
};

// hence we can put the abstract base class (concept) and the wrapper template (model) into a big struct (interface)
// to achieve type erasure

namespace example_Cpp2
{
    struct knight
    {
        void attack() { std::cout << "draw sword\n"; }
    };

    struct mage
    {
        void attack() { std::cout << "spell magic curse\n"; }
    };

    struct game  //// excellent example of type erasure
    {
        struct game_unit /// concept
        {
            virtual void attack() = 0;
            virtual ~game_unit() = default;
        };

        template <typename T>
        struct game_unit_wrapper : public game_unit /// model
        {
            game_unit_wrapper(T &unit) : t(unit) {}

            void attack() override { t.attack(); }

        private:
            T &t;
        };

        template <typename T>
        void addUnit(T &unit)
        {
            units.push_back(std::make_unique<game_unit_wrapper<T>>(unit));
        }

        void fight()
        {
            for (auto &u : units)
                u->attack();
        }

    private:
        std::vector<std::unique_ptr<game_unit>> units; /// hold the abstract base class, but indeed all the elements!
    };
};

int main()
{
    {
        using namespace example_CType;

        knight k;
        mage m;

        std::vector<std::pair<void *, fight_fn>> units{
            {&k, &fight_knight},
            {&m, &fight_mage},
        };

        fight(units);
    }

    {
        using namespace examples_Cpp;

        knight k;
        mage m;

        game_unit_wrapper ku{k}; /// the compiler can deduce the type of the template parameter
        game_unit_wrapper mu{m};

        std::vector<game_unit *> v{&ku, &mu};
        fight(v);
    }

    {
        using namespace example_Cpp2;

        knight k;
        mage m;

        game g;
        g.addUnit(k);
        g.addUnit(m);  

        g.fight();
    }
}