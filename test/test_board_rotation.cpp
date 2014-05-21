/*
 * test_board_rotation.cpp
 *
 *  Created on: 07.02.2013
 *      Author: mike_gresens
 */

#include "board_rotation.hpp"
#include <boost/test/unit_test.hpp>

namespace test {

BOOST_AUTO_TEST_SUITE(test_board_rotation)

using namespace chess;

BOOST_AUTO_TEST_CASE(test_board_rotation_west)
{
	BOOST_CHECK_EQUAL(C4 | F3 | G7, board_rotation<west>()(C4 | F3 | G7));
}

BOOST_AUTO_TEST_CASE(test_board_rotation_east)
{
	BOOST_CHECK_EQUAL(C4 | F3 | G7, board_rotation<east>()(C4 | F3 | G7));
}

BOOST_AUTO_TEST_CASE(test_board_rotation_north)
{
	BOOST_CHECK_EQUAL(D3 | C6 | G7, board_rotation<north>()(C4 | F3 | G7));
}

BOOST_AUTO_TEST_CASE(test_board_rotation_south)
{
	BOOST_CHECK_EQUAL(D3 | C6 | G7, board_rotation<south>()(C4 | F3 | G7));
}

BOOST_AUTO_TEST_CASE(test_board_rotation_north_east)
{
	BOOST_CHECK_EQUAL(G5 | E2 | C5, board_rotation<north_east>()(C4 | F3 | G7));
}

BOOST_AUTO_TEST_CASE(test_board_rotation_north_west)
{
	BOOST_CHECK_EQUAL(C3 | G4 | D8, board_rotation<north_west>()(C4 | F3 | G7));
}

BOOST_AUTO_TEST_CASE(test_board_rotation_south_east)
{
	BOOST_CHECK_EQUAL(B6 | G1 | H5, board_rotation<south_east>()(C4 | F3 | G7));
}

BOOST_AUTO_TEST_CASE(test_board_rotation_south_west)
{
	BOOST_CHECK_EQUAL(G6 | B1 | A5, board_rotation<south_west>()(C4 | F3 | G7));
}

BOOST_AUTO_TEST_SUITE_END()

}
