/*
 * test_board.cpp
 *
 *  Created on: 07.02.2013
 *      Author: mike_gresens
 */

#include "board.hpp"
#include <boost/test/unit_test.hpp>

namespace test {

BOOST_AUTO_TEST_SUITE(test_board)

using namespace chess;

BOOST_AUTO_TEST_CASE(test_board_of)
{
	BOOST_CHECK_EQUAL(1ULL << e4, board_of(e4));
}

BOOST_AUTO_TEST_CASE(test_board_find_square_forward)
{
	BOOST_CHECK_EQUAL(e4, board_find_square_forward(E4));
}

BOOST_AUTO_TEST_CASE(test_board_find_square_backward)
{
	BOOST_CHECK_EQUAL(e4, board_find_square_backward(E4));
}

BOOST_AUTO_TEST_CASE(test_board_clear_square_forward)
{
	board_t board = E4;
	const square_t square = board_clear_square_forward(board);
	BOOST_CHECK_EQUAL(e4, square);
	BOOST_CHECK_EQUAL(board_t(), board);
}

BOOST_AUTO_TEST_CASE(test_board_count_squares)
{
	BOOST_CHECK_EQUAL(2, board_count_squares(E4 | D5));
	BOOST_CHECK_EQUAL(0, board_count_squares(board_t()));
}

BOOST_AUTO_TEST_CASE(test_board_clear_square_backward)
{
	board_t board = E4;
	const square_t square = board_clear_square_backward(board);
	BOOST_CHECK_EQUAL(e4, square);
	BOOST_CHECK_EQUAL(board_t(), board);
}

BOOST_AUTO_TEST_CASE(test_board_test_square)
{
	BOOST_CHECK_EQUAL(true, board_test_square(E4, e4));
	BOOST_CHECK_EQUAL(false, board_test_square(D5, e4));
}

BOOST_AUTO_TEST_CASE(test_board_of_file)
{
	BOOST_CHECK_EQUAL(D1 | D2 | D3 | D4 | D5 | D6 | D7 | D8, board_of_file<3>()());
}

BOOST_AUTO_TEST_CASE(test_board_of_rank)
{
	BOOST_CHECK_EQUAL(A4 | B4 | C4 | D4 | E4 | F4 | G4 | H4, board_of_rank<3>()());
}

BOOST_AUTO_TEST_SUITE_END()

}
