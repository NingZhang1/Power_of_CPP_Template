#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/mult.hpp>
#include <boost/hana/negate.hpp>
#include <boost/hana/plus.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/fwd/define_struct.hpp>

#include <string>
#include <vector>
namespace hana = boost::hana;

// template <typename P1, typename P2>
// constexpr auto distance(P1 p1, P2 p2)
// {
//     auto xs = p1.x - p2.x;
//     auto ys = p1.y - p2.y;
//     return sqrt(xs * xs + ys * ys);
// }

// template <int i>
// constexpr integral_constant<int, i> int_c{};

// struct point
// {
//     BOOST_HANA_DEFINE_STRUCT(point, (int, x), (int, y));
// };
// int main()
// {
//     BOOST_HANA_CONSTANT_CHECK(distance(point(hana::3_c, 5_c), point(7_c, 2_c)) == 5_c);
// }

int main()
{
}