/*
 * test_node_executor.cpp
 *
 *  Created on: 06.02.2013
 *      Author: mike_gresens
 */

#include "node.hpp"
#include <boost/test/unit_test.hpp>

namespace test {

BOOST_AUTO_TEST_SUITE(test_node_executor)

using namespace chess;

BOOST_AUTO_TEST_CASE(test_node_executor_passive)
{
	const node_t node1(node_t::INITIAL_POSITION);
	const node_t node2(node1, move_t(e2, e4), passive_moves_tag());
	BOOST_CHECK(&node1 == node2.parent());
	BOOST_CHECK_EQUAL(A1 | H1, node2.board_rooks_white());
	BOOST_CHECK_EQUAL(A8 | H8, node2.board_rooks_black());
	BOOST_CHECK_EQUAL(P, node2[e4]);
	BOOST_CHECK_EQUAL(no_piece, node2[e2]);
	BOOST_CHECK_EQUAL(e3, node2.square_en_passant());
	BOOST_CHECK_EQUAL(color_t(black), color_t(node2.color()));
}

BOOST_AUTO_TEST_CASE(test_node_executor_en_passant)
{
	const node_t node1("4k3/8/8/8/3p4/8/4P3/4K3 w - - 0 1");
	const node_t node2(node1, move_t(e2, e4), passive_moves_tag());
	BOOST_CHECK_EQUAL(e3, node2.square_en_passant());

	BOOST_CHECK_EQUAL
	(
		D1 | F1 | D2 | E2 | F2 | D5 | F5,
		node2.board_attack_white()
	);

	BOOST_CHECK_EQUAL
	(
		D8 | F8 | D7 | E7 | F7 | E3 | C3,
		node2.board_attack_black()
	);
}

BOOST_AUTO_TEST_CASE(test_node_executor_capture_king)
{
	const node_t node("4k2R/8/4K3/8/8/8/8/8 w - - 0 1");
	BOOST_CHECK_THROW(node_t(node, move_t(h8, e8), active_moves_tag()), std::exception);
}

BOOST_AUTO_TEST_CASE(test_node_executor_promote_king)
{
	const node_t node("4k3/P7/4K3/8/8/8/8/8 w - - 0 1");
	BOOST_CHECK_THROW(node_t(node, move_t(a7, a8, 5), active_moves_tag()), std::exception);
}

BOOST_AUTO_TEST_SUITE_END()

}
