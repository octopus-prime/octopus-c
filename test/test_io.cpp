/*
 * test_io.cpp
 *
 *  Created on: 06.02.2013
 *      Author: mike_gresens
 */

#include "io.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>

namespace test {

BOOST_AUTO_TEST_SUITE(test_io)

using namespace chess;

BOOST_AUTO_TEST_CASE(test_io_move)
{
	const node_t node("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	const move_t move(e2, e4);

	BOOST_CHECK_EQUAL
	(
		"Pe2-e4",
		boost::lexical_cast<std::string>(std::make_pair(std::ref(node), std::ref(move)))
	);
}

BOOST_AUTO_TEST_CASE(test_io_node)
{
	const node_t node("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

	BOOST_CHECK_EQUAL
	(
		"\n abcdefgh\n8rnbqkbnr8\n7pppppppp7\n6        6\n5        5\n4        4\n3        3\n2PPPPPPPP2\n1RNBQKBNR1\n abcdefgh\nColor = 1\nEnPassant = a1\n",
		boost::lexical_cast<std::string>(node)
	);
}

BOOST_AUTO_TEST_SUITE_END()

}
