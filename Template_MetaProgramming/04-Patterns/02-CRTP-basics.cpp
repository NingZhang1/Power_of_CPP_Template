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

// Curiously Recurring Template Pattern (CRTP)
// it is a template design pattern where a base class is a template parameterized by the derived class
// it is used to add functionality to the derived class
// it is used to add *static* polymorphism to the derived class

// Example of static polymorphism

namespace examples_basic
{
    template <typename Derived>
    struct game_unit
    {
        void attack() /// member function in the base class and the derived class should not have the same name!
        {
            static_cast<Derived *>(this)->attack_impl(); /// this is the key point of the CRTP, the polymorphism is resolved at compile time
        }
    };

    struct knight : game_unit<knight>
    {
        void attack_impl()
        {
            std::cout << "draw sword\n";
        }
    };

    struct mage : game_unit<mage>
    {
        void attack_impl()
        {
            std::cout << "spell magic curse\n";
        }
    };

    template <typename Derived>
    void fight(std::vector<game_unit<Derived> *> const &units)
    {
        for (auto unit : units)
        {
            unit->attack();
        }
    }
};

int main()
{
    {
        using namespace examples_basic;

        knight k;
        mage m;
        fight<knight>({&k});
        fight<mage>({&m});
    }
}

/// Why is CRTP useful ? 

/// you can solve the following three problems with CRTP:
/// 1. you can limiting the number of times a type can be instantiated
/// 2. you can add functionality to a class without modifying it
/// 3. you can implement the composite design pattern
