#include <boost/hana.hpp>
namespace hana = boost::hana;

#include <cassert>
#include <iostream>
#include <string>

struct Fish
{
    std::string name;
};
struct Cat
{
    std::string name;
};
struct Dog
{
    std::string name;
};

using namespace hana::literals;

/// examples for serializer

struct Person
{
    BOOST_HANA_DEFINE_STRUCT(Person,
                             (std::string, name),
                             (int, age)); /// todo interface this macro!
};

int main()
{
    /// Define a tuple of animals via Hana's tuple

    auto animals = hana::make_tuple(Fish{"Nemo"}, Cat{"Garfield"}, Dog{"Snoopy"});

    /// Hana provides a lot of algorithms to work with tuples than the standard library

    // Access tuple elements with operator[] instead of std::get.
    Cat garfield = animals[1_c]; /// 1_c is a compile-time integer, this makes the code more natural !

    // Perform high level algorithms on tuples (this is like std::transform)

    auto names = hana::transform(animals, [](auto a)
                                 { return a.name; });
    assert(hana::reverse(names) == hana::make_tuple("Snoopy", "Garfield", "Nemo"));

    /// compile-time type computation

    auto animal_types = hana::make_tuple(hana::type_c<Fish *>, hana::type_c<Cat &>, hana::type_c<Dog>);

    /// NOTE: type_c<...> is not a type! It is a C++14 variable template yielding an object representing a type for Hana.

    auto no_pointers = hana::remove_if(animal_types, [](auto a)
                                       { return hana::traits::is_pointer(a); });
    static_assert(no_pointers == hana::make_tuple(hana::type_c<Cat &>, hana::type_c<Dog>), ""); // done at compile-time

    /// In addition to heterogeneous and compile-time sequences,
    /// Hana provides several features to make your metaprogramming nightmares a thing of the past.
    /// one can check for the existence of a struct member with one easy line !!!!

    auto has_name = hana::is_valid([](auto &&x) -> decltype((void)x.name)
                                   { return {}; }); /// auto + lambda ?

    static_assert(has_name(garfield), "");
    static_assert(!has_name(1), "");

    /// a final example: serialization

    auto serialize = [](std::ostream &os, auto const &object)
    {
        hana::for_each(hana::members(object), [&](auto member)
                       { os << member << std::endl; });
    };

    Person john{"John", 30};
    serialize(std::cout, john);
    
}