/*
 * test_node_generator.cpp
 *
 *  Created on: 07.07.2013
 *      Author: mike_gresens
 */

#include "node_generator.hpp"
#include <boost/test/unit_test.hpp>

namespace test {

BOOST_AUTO_TEST_SUITE(test_node_generator)

using namespace chess;

static history_table_t h_table;

static inline bool
equal_nodes(const node_generator<passive_moves_tag>::entry_t& node1, const node_generator<passive_moves_tag>::entry_t& node2)
{
	return std::get<0>(node1).hash_node() == std::get<0>(node2).hash_node();
}

BOOST_AUTO_TEST_CASE(test_node_generator_foo)
{
	const node_t node("4k3/8/8/8/8/8/r7/4K3 w - - 0 1");
	const std::vector<node_generator<passive_moves_tag>::entry_t> expected_p
	{{
		std::make_tuple(node_t(node, move_t(e1, d1), passive_moves_tag()), move_t(e1, d1)),
		std::make_tuple(node_t(node, move_t(e1, f1), passive_moves_tag()), move_t(e1, f1)),
	}};

	const node_generator<active_moves_tag> generator_a(node, h_table, move_t());
	BOOST_CHECK(generator_a.begin() == generator_a.end());

	const node_generator<passive_moves_tag> generator_p(node, h_table, move_t());
	BOOST_CHECK(generator_p.begin() != generator_p.end());
	BOOST_CHECK(std::is_permutation(expected_p.begin(), expected_p.end(), generator_p.begin(), equal_nodes));
	BOOST_CHECK(std::is_permutation(generator_p.begin(), generator_p.end(), expected_p.begin(), equal_nodes));
}

BOOST_AUTO_TEST_CASE(test_node_generator_foo_with_actives)
{
	const node_t node("4k3/8/8/8/8/8/r4b2/4K3 w - - 0 1");
	const std::vector<node_generator<passive_moves_tag>::entry_t> expected_p
	{{
		std::make_tuple(node_t(node, move_t(e1, d1), passive_moves_tag()), move_t(e1, d1)),
		std::make_tuple(node_t(node, move_t(e1, f1), passive_moves_tag()), move_t(e1, f1)),
	}};

	const node_generator<active_moves_tag> generator_a(node, h_table, move_t());
	BOOST_CHECK(generator_a.begin() == generator_a.end());

	const node_generator<passive_moves_tag> generator_p(node, h_table, move_t());
	BOOST_CHECK(generator_p.begin() != generator_p.end());
	BOOST_CHECK(std::is_permutation(expected_p.begin(), expected_p.end(), generator_p.begin(), equal_nodes));
	BOOST_CHECK(std::is_permutation(generator_p.begin(), generator_p.end(), expected_p.begin(), equal_nodes));
}

BOOST_AUTO_TEST_CASE(test_node_generator_bar)
{
	const node_t node("4k3/8/8/8/8/8/2q5/4K3 w - - 0 1");
	const std::vector<node_generator<passive_moves_tag>::entry_t> expected_p
	{{
		std::make_tuple(node_t(node, move_t(e1, f1), passive_moves_tag()), move_t(e1, f1)),
	}};

	const node_generator<active_moves_tag> generator_a(node, h_table, move_t());
	BOOST_CHECK(generator_a.begin() == generator_a.end());

	const node_generator<passive_moves_tag> generator_p(node, h_table, move_t());
	BOOST_CHECK(generator_p.begin() != generator_p.end());
	BOOST_CHECK(std::is_permutation(expected_p.begin(), expected_p.end(), generator_p.begin(), equal_nodes));
	BOOST_CHECK(std::is_permutation(generator_p.begin(), generator_p.end(), expected_p.begin(), equal_nodes));
}

BOOST_AUTO_TEST_CASE(test_node_generator_bar_with_actives)
{
	const node_t node("4k3/8/8/8/8/8/2q2b2/4K3 w - - 0 1");
	const std::vector<node_generator<passive_moves_tag>::entry_t> expected_p
	{{
		std::make_tuple(node_t(node, move_t(e1, f1), passive_moves_tag()), move_t(e1, f1)),
	}};

	const node_generator<active_moves_tag> generator_a(node, h_table, move_t());
	BOOST_CHECK(generator_a.begin() == generator_a.end());

	const node_generator<passive_moves_tag> generator_p(node, h_table, move_t());
	BOOST_CHECK(generator_p.begin() != generator_p.end());
	BOOST_CHECK(std::is_permutation(expected_p.begin(), expected_p.end(), generator_p.begin(), equal_nodes));
	BOOST_CHECK(std::is_permutation(generator_p.begin(), generator_p.end(), expected_p.begin(), equal_nodes));
}

BOOST_AUTO_TEST_SUITE_END()

}
