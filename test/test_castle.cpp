/*
 * test_castle.cpp
 *
 *  Created on: 06.02.2013
 *      Author: mike_gresens
 */

#include "castle.hpp"
#include <boost/test/unit_test.hpp>

namespace test {

BOOST_AUTO_TEST_SUITE(test_castle)

using namespace chess;

BOOST_AUTO_TEST_CASE(test_castle_value)
{
	BOOST_CHECK_EQUAL(1, white_castle_king);
	BOOST_CHECK_EQUAL(8, black_castle_queen);

	BOOST_CHECK_EQUAL(3, white_castle);
	BOOST_CHECK_EQUAL(12, black_castle);
}

BOOST_AUTO_TEST_SUITE_END()

}
