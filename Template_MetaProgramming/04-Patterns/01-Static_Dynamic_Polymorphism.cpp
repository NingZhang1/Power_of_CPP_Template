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

// dynamic polymorphism or late binding
// it occurs at runtime with the help of interfaces and virtual functions,
// it requires a pointer or a reference to the base class
// but it incurs a performance cost be case of the virtual function table !!!
// and the compiler can't optimize the code !!!

// static polymorphism or early binding
// it occurs at compile time with the help of templates and function overloading, which are resolved at compile time
// it requires the actual type of the object, not a pointer or a reference

// Example of dynamic polymorphism

namespace examples
{
    struct game_unit
    {
        virtual void attack() = 0; // pure virtual function
    };

    struct knight : game_unit // polymorphic class
    {
        void attack() override
        {
            std::cout << "draw sword\n";
        }
    };

    struct mage : game_unit // polymorphic class
    {
        void attack() override
        {
            std::cout << "spell magic curse\n";
        }
    };

    void fight(std::vector<game_unit *> const &units) // this functions deal with sets of class uniformly
    {
        for (auto unit : units)
        {
            unit->attack();
        }
    }

    // what if we want to create a new class representing mage + knight ?

    struct knight_mage : game_unit
    {
        void attack() override
        {
            std::cout << "draw magic sword\n";
        }
    };

    knight_mage operator+(knight const &k, mage const &m) // polymorphic function, operator overloading, static polymorphism
    {
        return knight_mage{};
    }

    // we can also overload functions

    struct attack
    {
        int value;
    };

    struct defence
    {
        int value;
    };

    template <typename T>
    T operator+(T const &k, int const &m) // operator overloading by templating, static polymorphism
    {
        T res;
        res.value = k.value + m;
        return res;
    }

    // void increment(attack &a) { a.value++; }
    // void increment(defence &d) { d.value++; }

    // or

    template <typename T>
    void increment(T &t) { t.value++; }

};

int main()
{
    // examples of dynamic polymorphism
    {
        using namespace examples;

        knight k;
        mage m;
        fight({&k, &m});

        knight_mage km = k + m;
        km.attack();

        attack a{42};
        a = a + 2;

        defence d{50};
        increment(a);
        increment(d);
    }
}

// Policy-based design
// Policy-based design is a C++ idiom that uses templates to define a family of algorithms,
// which are then selected at compile time using inheritance.