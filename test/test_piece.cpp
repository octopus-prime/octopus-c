/*
 * test_piece.cpp
 *
 *  Created on: 06.02.2013
 *      Author: mike_gresens
 */

#include "piece.hpp"
#include <boost/test/unit_test.hpp>

namespace test {

BOOST_AUTO_TEST_SUITE(test_piece)

using namespace chess;

BOOST_AUTO_TEST_CASE(test_piece_value)
{
	BOOST_CHECK_EQUAL(0, no_piece);
	BOOST_CHECK_EQUAL(1, K);
	BOOST_CHECK_EQUAL(12, p);
}

BOOST_AUTO_TEST_SUITE_END()

}
