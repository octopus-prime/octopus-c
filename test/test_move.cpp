/*
 * test_move.cpp
 *
 *  Created on: 07.02.2013
 *      Author: mike_gresens
 */

#include "move.hpp"
#include <boost/test/unit_test.hpp>

namespace test {

BOOST_AUTO_TEST_SUITE(test_move)

using namespace chess;

BOOST_AUTO_TEST_CASE(test_move_default)
{
	constexpr move_t move;
	BOOST_CHECK_EQUAL(0, move);
	BOOST_CHECK_EQUAL(false, move.check());
}

BOOST_AUTO_TEST_CASE(test_move_standard)
{
	constexpr move_t move(f3, e5);
	BOOST_CHECK_EQUAL(f3 + (e5 << 8), move);
	BOOST_CHECK_EQUAL(false, move.check());
}

BOOST_AUTO_TEST_CASE(test_move_standard_promote)
{
	constexpr move_t move(f3, e5, 2);
	BOOST_CHECK_EQUAL(f3 + (e5 << 8) + (2 << 16), move);
	BOOST_CHECK_EQUAL(false, move.check());
}

BOOST_AUTO_TEST_CASE(test_move_standard_promote_check)
{
	move_t move(f3, e5, 2);
	move.check(true);
	BOOST_CHECK_EQUAL(f3 + (e5 << 8) + (2 << 16), move);
	BOOST_CHECK_EQUAL(true, move.check());
}

BOOST_AUTO_TEST_CASE(test_move_from)
{
	constexpr move_t move(f3, e5, 2);
	BOOST_CHECK_EQUAL(f3, move.from());
	BOOST_CHECK_EQUAL(false, move.check());
}

BOOST_AUTO_TEST_CASE(test_move_to)
{
	constexpr move_t move(f3, e5, 2);
	BOOST_CHECK_EQUAL(e5, move.to());
	BOOST_CHECK_EQUAL(false, move.check());
}

BOOST_AUTO_TEST_CASE(test_move_promote)
{
	constexpr move_t move(f3, e5, 2);
	BOOST_CHECK_EQUAL(2, move.promoted());
	BOOST_CHECK_EQUAL(false, move.check());
}

BOOST_AUTO_TEST_CASE(test_move_check)
{
	move_t move(f3, e5, 2);
	move.check(true);
	BOOST_CHECK_EQUAL(true, move.check());
	move.check(false);
	BOOST_CHECK_EQUAL(false, move.check());
}

BOOST_AUTO_TEST_SUITE_END()

}
