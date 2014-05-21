/*
 * test_hash.cpp
 *
 *  Created on: 06.02.2013
 *      Author: mike_gresens
 */

#include "hash.hpp"
#include <vector>
#include <boost/test/unit_test.hpp>

namespace test {

BOOST_AUTO_TEST_SUITE(test_hash)

using namespace chess;

BOOST_AUTO_TEST_CASE(test_hash_disjoined)
{
	std::vector<hash_t> hashes;

	for (piece_t piece = K; piece <= p; ++piece)
		for (square_t square = a1; square <= h8; ++square)
			hashes.emplace_back(piece_square_hash()(piece, square));

	for (square_t square = a3; square <= h3; ++square)
		hashes.emplace_back(en_passant_hash()(square));
	for (square_t square = a6; square <= h6; ++square)
		hashes.emplace_back(en_passant_hash()(square));

	for (castle_t castle = 0; castle <= 15; ++castle)
		hashes.emplace_back(castle_hash()(castle));

	hashes.emplace_back(color_hash()());

	BOOST_CHECK_EQUAL
	(
		std::distance(hashes.begin(), hashes.end()),
		std::distance(hashes.begin(), std::adjacent_find(hashes.begin(), hashes.end()))
	);
}

BOOST_AUTO_TEST_CASE(test_hash_zeroed)
{
	std::vector<hash_t> hashes;

	for (square_t square = a1; square <= h8; ++square)
		hashes.emplace_back(piece_square_hash()(no_piece, square));

	for (square_t square = a1; square <= h2; ++square)
		hashes.emplace_back(en_passant_hash()(square));
	for (square_t square = a4; square <= h5; ++square)
		hashes.emplace_back(en_passant_hash()(square));
	for (square_t square = a7; square <= h8; ++square)
		hashes.emplace_back(en_passant_hash()(square));

	BOOST_CHECK
	(
		std::all_of(hashes.begin(), hashes.end(), std::bind(std::equal_to<hash_t>(), std::placeholders::_1, 0))
	);
}

BOOST_AUTO_TEST_SUITE_END()

}
