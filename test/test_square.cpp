/*
 * test_square.cpp
 *
 *  Created on: 06.02.2013
 *      Author: mike_gresens
 */

#include "square.hpp"
#include <boost/test/unit_test.hpp>

namespace test {

BOOST_AUTO_TEST_SUITE(test_square)

using namespace chess;

BOOST_AUTO_TEST_CASE(test_square_value)
{
	BOOST_CHECK_EQUAL(26, c4);
}

BOOST_AUTO_TEST_CASE(test_square_of)
{
	BOOST_CHECK_EQUAL(c4, square_of(2, 3));
}

BOOST_AUTO_TEST_CASE(test_file_of)
{
	BOOST_CHECK_EQUAL(2, file_of(c4));
}

BOOST_AUTO_TEST_CASE(test_rank_of)
{
	BOOST_CHECK_EQUAL(3, rank_of(c4));
}

BOOST_AUTO_TEST_SUITE_END()

}
