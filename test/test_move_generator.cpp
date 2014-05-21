/*
 * test_move_generator.cpp
 *
 *  Created on: 07.02.2013
 *      Author: mike_gresens
 */

#include "move_generator.hpp"
#include <boost/test/unit_test.hpp>

namespace test {

BOOST_AUTO_TEST_SUITE(test_move_generator)

using namespace chess;

static history_table_t h_table;
//static killer_table k_table;

BOOST_AUTO_TEST_CASE(test_move_generator_initial)
{
	const node_t node(node_t::INITIAL_POSITION);
	const std::vector<move_generator<passive_moves_tag>::entry_t> expected_p
	{{
		std::make_tuple(move_t(a2, a3)), std::make_tuple(move_t(a2, a4)),
		std::make_tuple(move_t(b2, b3)), std::make_tuple(move_t(b2, b4)),
		std::make_tuple(move_t(c2, c3)), std::make_tuple(move_t(c2, c4)),
		std::make_tuple(move_t(d2, d3)), std::make_tuple(move_t(d2, d4)),
		std::make_tuple(move_t(e2, e3)), std::make_tuple(move_t(e2, e4)),
		std::make_tuple(move_t(f2, f3)), std::make_tuple(move_t(f2, f4)),
		std::make_tuple(move_t(g2, g3)), std::make_tuple(move_t(g2, g4)),
		std::make_tuple(move_t(h2, h3)), std::make_tuple(move_t(h2, h4)),
		std::make_tuple(move_t(b1, a3)), std::make_tuple(move_t(b1, c3)),
		std::make_tuple(move_t(g1, f3)), std::make_tuple(move_t(g1, h3))
	}};

	const move_generator<active_moves_tag> generator_a(node, h_table);
	BOOST_CHECK_EQUAL(generator_a.begin(), generator_a.end());

	const move_generator<passive_moves_tag> generator_p(node, h_table);//, k_table[0], k_table[-2], k_table[+2]);
	BOOST_CHECK(std::is_permutation(expected_p.begin(), expected_p.end(), generator_p.begin()));
	BOOST_CHECK(std::is_permutation(generator_p.begin(), generator_p.end(), expected_p.begin()));
}

BOOST_AUTO_TEST_CASE(test_move_generator_foo)
{
	const node_t node("4k3/8/8/3r1b2/4P3/8/8/4K3 w - - 0 1");
	const std::vector<move_generator<active_moves_tag>::entry_t> expected_a
	{{
		std::make_tuple(move_t(e4, d5), 16 * score_of_piece()(r) - score_of_piece()(P)),
		std::make_tuple(move_t(e4, f5), 16 * score_of_piece()(b) - score_of_piece()(P))
	}};
	const std::vector<move_generator<passive_moves_tag>::entry_t> expected_p
	{{
		std::make_tuple(move_t(e1, d1)), std::make_tuple(move_t(e1, d2)),
		std::make_tuple(move_t(e1, e2)), std::make_tuple(move_t(e1, f1)),
		std::make_tuple(move_t(e1, f2)), std::make_tuple(move_t(e4, e5))
	}};

	const move_generator<active_moves_tag> generator_a(node, h_table);
	BOOST_CHECK(std::is_permutation(expected_a.begin(), expected_a.end(), generator_a.begin()));
	BOOST_CHECK(std::is_permutation(generator_a.begin(), generator_a.end(), expected_a.begin()));
	BOOST_CHECK
	(
		std::is_sorted
		(
			generator_a.begin(), generator_a.end(),
			[](const move_generator<active_moves_tag>::entry_t entry1, const move_generator<active_moves_tag>::entry_t entry2)
			{
				return std::get<1>(entry1) > std::get<1>(entry1);
			}
		)
	);

	const move_generator<passive_moves_tag> generator_p(node, h_table);//, k_table[0], k_table[-2], k_table[+2]);
	BOOST_CHECK(std::is_permutation(expected_p.begin(), expected_p.end(), generator_p.begin()));
	BOOST_CHECK(std::is_permutation(generator_p.begin(), generator_p.end(), expected_p.begin()));
}

BOOST_AUTO_TEST_SUITE_END()

}
