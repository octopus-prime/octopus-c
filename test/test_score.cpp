/*
 * test_score.cpp
 *
 *  Created on: 06.02.2013
 *      Author: mike_gresens
 */

#include "score.hpp"
#include <boost/test/unit_test.hpp>

namespace test {

BOOST_AUTO_TEST_SUITE(test_score)

using namespace chess;

BOOST_AUTO_TEST_CASE(test_score_of_king)
{
	BOOST_CHECK_EQUAL(+30000, score_of_piece()(K));
	BOOST_CHECK_EQUAL(+30000, score_of_piece()(k));
}

BOOST_AUTO_TEST_CASE(test_score_of_queen)
{
	BOOST_CHECK_EQUAL(+900, score_of_piece()(Q));
	BOOST_CHECK_EQUAL(+900, score_of_piece()(q));
}

BOOST_AUTO_TEST_CASE(test_score_of_rook)
{
	BOOST_CHECK_EQUAL(+500, score_of_piece()(R));
	BOOST_CHECK_EQUAL(+500, score_of_piece()(r));
}

BOOST_AUTO_TEST_CASE(test_score_of_bishop)
{
	BOOST_CHECK_EQUAL(+333, score_of_piece()(B));
	BOOST_CHECK_EQUAL(+333, score_of_piece()(b));
}

BOOST_AUTO_TEST_CASE(test_score_of_knight)
{
	BOOST_CHECK_EQUAL(+325, score_of_piece()(N));
	BOOST_CHECK_EQUAL(+325, score_of_piece()(n));
}

BOOST_AUTO_TEST_CASE(test_score_of_pawn)
{
	BOOST_CHECK_EQUAL(+100, score_of_piece()(P));
	BOOST_CHECK_EQUAL(+100, score_of_piece()(p));
}

BOOST_AUTO_TEST_SUITE_END()

}
