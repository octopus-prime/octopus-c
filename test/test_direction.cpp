/*
 * test_direction.cpp
 *
 *  Created on: 06.02.2013
 *      Author: mike_gresens
 */

#include "direction.hpp"
#include <boost/test/unit_test.hpp>

namespace test {

BOOST_AUTO_TEST_SUITE(test_direction)

using namespace chess;

BOOST_AUTO_TEST_CASE(test_direction_value)
{
	BOOST_CHECK_EQUAL(+8, north);
	BOOST_CHECK_EQUAL(-8, south);
	BOOST_CHECK_EQUAL(+1, east);
	BOOST_CHECK_EQUAL(-1, west);

	BOOST_CHECK_EQUAL(+9, north_east);
	BOOST_CHECK_EQUAL(+7, north_west);
	BOOST_CHECK_EQUAL(-7, south_east);
	BOOST_CHECK_EQUAL(-9, south_west);
}

BOOST_AUTO_TEST_SUITE_END()

}
