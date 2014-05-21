/*
 * test_square_rotation.cpp
 *
 *  Created on: 06.02.2013
 *      Author: mike_gresens
 */

#include "square_rotation.hpp"
#include <boost/test/unit_test.hpp>

namespace test {

BOOST_AUTO_TEST_SUITE(test_square_rotation)

using namespace chess;

BOOST_AUTO_TEST_CASE(test_square_rotation_west)
{
	BOOST_CHECK_EQUAL(c4, square_rotation<west>()(c4));
}

BOOST_AUTO_TEST_CASE(test_square_rotation_east)
{
	BOOST_CHECK_EQUAL(c4, square_rotation<east>()(c4));
}

BOOST_AUTO_TEST_CASE(test_square_rotation_north)
{
	BOOST_CHECK_EQUAL(d3, square_rotation<north>()(c4));
}

BOOST_AUTO_TEST_CASE(test_square_rotation_south)
{
	BOOST_CHECK_EQUAL(d3, square_rotation<south>()(c4));
}

BOOST_AUTO_TEST_CASE(test_square_rotation_north_west)
{
	BOOST_CHECK_EQUAL(c3, square_rotation<north_west>()(c4));
}

BOOST_AUTO_TEST_CASE(test_square_rotation_north_east)
{
	BOOST_CHECK_EQUAL(g5, square_rotation<north_east>()(c4));
}

BOOST_AUTO_TEST_CASE(test_square_rotation_south_west)
{
	BOOST_CHECK_EQUAL(g6, square_rotation<south_west>()(c4));
}

BOOST_AUTO_TEST_CASE(test_square_rotation_south_east)
{
	BOOST_CHECK_EQUAL(b6, square_rotation<south_east>()(c4));
}

BOOST_AUTO_TEST_SUITE_END()

}
