#include <boost/fusion/sequence.hpp>
#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/vector.hpp>
#include <iostream>

using namespace boost::fusion;

#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/include/algorithm.hpp>

struct print_xml
{
    template <typename T>
    void operator()(T const &x) const
    {
        std::cout
            << '<' << typeid(x).name() << '>'
            << x
            << "</" << typeid(x).name() << '>'
            << std::endl;
    }
};

#include <boost/type_traits/is_pointer.hpp>

template <typename Sequence>
void xml_print_pointers(Sequence const &seq)
{
    for_each(filter_if<boost::is_pointer<mpl_::_>>(seq), print_xml()); /// generic function
    // filter_if is another Fusion algorithm.
    // It returns a filter_view, a conforming Fusion sequence.
    // This view reflects only those elements that pass the given predicate.
    // In this case, the predicate is boost::is_pointer<_>.
    // This "filtered view" is then passed to the for_each algorithm, which then prints the "filtered view" as XML.
}

int main()
{
    vector<int, char, std::string> stuff(1, 'x', "howdy"); // fusion's vector, holding arbitrary types
    int i = at_c<0>(stuff);
    char ch = at_c<1>(stuff);
    std::string s = at_c<2>(stuff);

    for_each(stuff, print_xml()); // a fusion algorithm, a generic algorithm

    vector<int, char, std::string, int *> stuff2(1, 'x', "howdy", nullptr);
    xml_print_pointers(stuff2);
}