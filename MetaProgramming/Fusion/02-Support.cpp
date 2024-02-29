#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/support/is_view.hpp>
#include <boost/fusion/include/is_view.hpp>
#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/include/tag_of.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/include/category_of.hpp>

#include <vector>

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/container/list.hpp>
#include <boost/fusion/include/list.hpp>

#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/include/algorithm.hpp>

using namespace boost::fusion;

int main()
{
    // *is_sequence* is a metafunction

    // that returns true if the given type is a Fusion sequence.

    BOOST_MPL_ASSERT_NOT((traits::is_sequence<std::vector<int>>));
    BOOST_MPL_ASSERT_NOT((traits::is_sequence<int>));
    BOOST_MPL_ASSERT((traits::is_sequence<list<>>));
    BOOST_MPL_ASSERT((traits::is_sequence<list<int>>));
    BOOST_MPL_ASSERT((traits::is_sequence<vector<>>));
    BOOST_MPL_ASSERT((traits::is_sequence<vector<int>>));

    // *is_view* is a metafunction

    // that returns true if the given type is a Fusion view.
    // A view is a specialized sequence that does not actually contain data.
    // Views hold sequences which may be other views.
    // In general, views are held by other views by value,
    // while non-views are held by other views by reference.

    BOOST_MPL_ASSERT_NOT((traits::is_view<std::vector<int>>));
    BOOST_MPL_ASSERT_NOT((traits::is_view<int>));

    using boost::is_pointer;
    using boost::mpl::_;
    typedef vector<int *, char, long *, bool, double> vector_type;
    typedef filter_view<vector_type, is_pointer<_>> filter_view_type;
    BOOST_MPL_ASSERT((traits::is_view<filter_view_type>));

    // *tag_of* is a metafunction

    // All conforming Fusion sequences and iterators have an associated tag type.
    // The purpose of the tag is to enable tag dispatching from Intrinsic functions
    // to implementations appropriate for the type.

    // it could return any types

    typedef traits::tag_of<list<>>::type tag1;
    typedef traits::tag_of<list<int>>::type tag2;
    typedef traits::tag_of<vector<>>::type tag3;
    typedef traits::tag_of<vector<int>>::type tag4;

    BOOST_MPL_ASSERT((boost::is_same<tag1, tag2>));
    BOOST_MPL_ASSERT((boost::is_same<tag3, tag4>));

    // *category_of* is a metafunction

    // A metafunction that establishes the conceptual classification of a particular Sequence or Iterator

    using boost::is_base_of;
    typedef traits::category_of<list<>>::type list_category;
    typedef traits::category_of<vector<>>::type vector_category;
    BOOST_MPL_ASSERT((is_base_of<forward_traversal_tag, list_category>));
    BOOST_MPL_ASSERT((is_base_of<random_access_traversal_tag, vector_category>));

    // *deduce*

    // *deduce_sequence*
    
    // *pair*
    // Fusion pair type is a half runtime pair. 
    // A half runtime pair is similar to a std::pair, 
    // but, unlike std::pair, the *first type does not have data*. 
    // It is used as elements in maps,
}