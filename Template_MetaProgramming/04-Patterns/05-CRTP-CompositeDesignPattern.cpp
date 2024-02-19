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

// composite design pattern
// it is a structural pattern that allows composing objects into tree structures to represent part-whole hierarchies
// this pattern allows clients to treat individual objects and compositions of objects uniformly
// CRTP here is used to achieve this pattern

namespace examples
{
    struct hero
    {
        hero(std::string_view n) : name(n) {}

        void ally_with(hero &u)
        {
            connections.insert(&u);
            u.connections.insert(this);
        }

    private:
        std::string name;
        std::set<hero *> connections;

        friend std::ostream &operator<<(std::ostream &os, hero const &obj);
    };

    std::ostream &operator<<(std::ostream &os, hero const &obj)
    {
        for (hero *u : obj.connections)
            os << obj.name << " --> [" << u->name << "]" << '\n';

        return os;
    }

    struct hero_party : std::vector<hero>
    {
    };

    /// question how to treat the hero_party and hero uniformly ?
};

namespace examples2
{
    template <typename T>
    struct base_unit
    {
        template <typename U>
        void ally_with(U &other);
    };

    struct hero : base_unit<hero>
    {
        hero(std::string_view n) : name(n) {}

        hero *begin() { return this; } /// make it iterable
        hero *end() { return this + 1; }

    private:
        std::string name;
        std::set<hero *> connections;

        template <typename U>
        friend struct base_unit;

        template <typename U>
        friend std::ostream &operator<<(std::ostream &os, base_unit<U> &object);
    };

    struct hero_party : std::vector<hero>, base_unit<hero_party>
    {
    };

    template <typename T>
    template <typename U>
    void base_unit<T>::ally_with(U &other)
    {
        for (auto &from : *static_cast<T *>(this)) // this is the way to make it treated uniformly !!!!
        {                                          // in some sense, hero is a hero_party with only one element, but here we do not do any type conversion
            for (auto &to : other)
            {
                from.connections.insert(&to);
                to.connections.insert(&from);
            }
        }
    }

    template <typename T>
    std::ostream &operator<<(std::ostream &os, base_unit<T> &object)
    {
        for (auto &obj : *static_cast<T *>(&object)) /// *auto* here is also very important 
        {
            for (auto *n : obj.connections)
                os << obj.name << " --> [" << n->name << "]" << '\n';
        }
        return os;
    }
};

int main()
{
    using namespace examples2;

    hero k1("Arthur");
    hero k2("Sir Lancelot");

    hero_party p1;
    p1.emplace_back("Bors");

    hero_party p2;
    p2.emplace_back("Cador");
    p2.emplace_back("Constantine");

    k1.ally_with(k2);
    k1.ally_with(p1);

    p1.ally_with(k2);
    p1.ally_with(p2);

    std::cout << k1 << '\n';
    std::cout << k2 << '\n';
    std::cout << p1 << '\n';
    std::cout << p2 << '\n';
}