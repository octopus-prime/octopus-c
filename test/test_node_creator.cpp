/*
 * test_node_creator.cpp
 *
 *  Created on: 06.02.2013
 *      Author: mike_gresens
 */

#include "node.hpp"
#include <boost/test/unit_test.hpp>

namespace test {

BOOST_AUTO_TEST_SUITE(test_node_creator)

using namespace chess;

BOOST_AUTO_TEST_CASE(test_node_creator_initial)
{
	const node_t node(node_t::INITIAL_POSITION);
	BOOST_CHECK(nullptr == node.parent());
	BOOST_CHECK_EQUAL(board_of_rank<0>()() | board_of_rank<1>()(), node.board_occupy_white());
	BOOST_CHECK_EQUAL(board_of_rank<7>()() | board_of_rank<6>()(), node.board_occupy_black());
	BOOST_CHECK_EQUAL(e1, node.square_king_white());
	BOOST_CHECK_EQUAL(e8, node.square_king_black());
	BOOST_CHECK_EQUAL(D1, node.board_queens_white());
	BOOST_CHECK_EQUAL(D8, node.board_queens_black());
	BOOST_CHECK_EQUAL(A1 | H1, node.board_rooks_white());
	BOOST_CHECK_EQUAL(A8 | H8, node.board_rooks_black());
	BOOST_CHECK_EQUAL(B1 | G1, node.board_knights_white());
	BOOST_CHECK_EQUAL(B8 | G8, node.board_knights_black());
	BOOST_CHECK_EQUAL(C1 | F1, node.board_bishops_white());
	BOOST_CHECK_EQUAL(C8 | F8, node.board_bishops_black());
	BOOST_CHECK_EQUAL(board_of_rank<1>()(), node.board_pawns_white());
	BOOST_CHECK_EQUAL(board_of_rank<6>()(), node.board_pawns_black());
	BOOST_CHECK_EQUAL(color_t(white), color_t(node.color()));
	BOOST_CHECK_EQUAL(16, node.count_pieces_white());
	BOOST_CHECK_EQUAL(16, node.count_pieces_black());
	BOOST_CHECK_EQUAL(0, node.count_half_moves());
	BOOST_CHECK_EQUAL(1, node.count_full_moves());

	BOOST_CHECK_EQUAL
	(
		A2 | A3 | B1 | B2 | B3 | C1 | C2 | C3 | D1 | D2 | D3 | E1 | E2 | E3 | F1 | F2 | F3 | G1 | G2 | G3 | H2 | H3,
		node.board_attack_white()
	);

	BOOST_CHECK_EQUAL
	(
		A7 | A6 | B8 | B7 | B6 | C8 | C7 | C6 | D8 | D7 | D6 | E8 | E7 | E6 | F8 | F7 | F6 | G8 | G7 | G6 | H7 | H6,
		node.board_attack_black()
	);
}

BOOST_AUTO_TEST_SUITE_END()

}
