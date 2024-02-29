// Fusion provides functions for manipulating iterators, analogous to the similar functions from the MPL library.

#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/include/deref.hpp>
#include <boost/fusion/iterator/deref_data.hpp>
#include <boost/fusion/include/deref_data.hpp>

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/include/map.hpp>

#include <boost/fusion/iterator/next.hpp>

using namespace boost::fusion;

#include <cassert>

int main()
{
    /// *deref* is a function
    /// it Deferences an iterator.

    typedef vector<int, int &> vec;

    int i(0);
    vec v(1, i);
    assert(deref(begin(v)) == 1);
    assert(deref(next(begin(v))) == 0);
    assert(&(deref(next(begin(v)))) == &i);

    /// next is a function
    /// Moves an iterator 1 position forwards.

    typedef vector<int, int, int> vec2;

    vec2 v1(1, 2, 3);
    assert(deref(begin(v1)) == 1);
    assert(deref(next(begin(v1))) == 2);
    assert(deref(next(next(begin(v1)))) == 3);

    /// prior is a function
    /// Moves an iterator 1 position backwards.

    vec2 v2(1, 2, 4);
    assert(deref(next(begin(v2))) == 2);
    assert(deref(prior(next(begin(v2)))) == 1);

    /// distance is a function
    /// Returns the number of positions between two iterators.

    vec2 v3(1, 2, 3);
    assert(distance(begin(v3), next(next(begin(v3)))) == 2);

    /// advance is a function
    /// Moves an iterator by a specified distance.

    assert(deref(advance<boost::mpl::int_<2>>(begin(v3))) == 3);

    /// advance_c is a function
    /// Moves an iterator by a specified distance.

    assert(deref(advance_c<2>(begin(v3))) == 3);

    /// deref_data is a function
    /// Deferences the data property associated with the element referenced by an associative iterator.

    typedef map<pair<float, int &>> map;

    int i(0);
    map m(1.0f, i);
    assert(deref_data(begin(m)) == 0);
    assert(&(deref_data(begin(m))) == &i);
}