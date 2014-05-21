/*
 * hash.cpp
 *
 *  Created on: 08.02.2013
 *      Author: mike_gresens
 */

#include "hash.hpp"
#include <random>
#include <algorithm>

namespace chess {

static inline hash_t
random()
{
	static std::mt19937_64 generator;
	static std::uniform_int_distribution<hash_t> distribution;

	return distribution(generator);
}

static std::array<std::array<hash_t, 64>, 13>
create_piece_square_hash()
{
	std::array<std::array<hash_t, 64>, 13> hash;

	std::fill(hash[no_piece].begin(), hash[no_piece].end(), 0ULL);
	for (piece_t piece = K; piece <= p; ++piece)
		std::generate(hash[piece].begin(), hash[piece].end(), random);

	return hash;
}

static std::array<hash_t, 64>
create_en_passant_hash()
{
	std::array<hash_t, 64> hash;

	std::fill_n(&hash[0], 16, 0);
	std::generate_n(&hash[16], 8, random);
	std::fill_n(&hash[24], 16, 0);
	std::generate_n(&hash[40], 8, random);
	std::fill_n(&hash[48], 16, 0);

	return hash;
}

static std::array<hash_t, 16>
create_castle_hash()
{
	std::array<hash_t, 16> hash;

	std::generate(hash.begin(), hash.end(), random);

	return hash;
}

const std::array<std::array<hash_t, 64>, 13> piece_square_hash::hash(create_piece_square_hash());
const std::array<hash_t, 64> en_passant_hash::hash(create_en_passant_hash());
const std::array<hash_t, 16> castle_hash::hash(create_castle_hash());
const hash_t color_hash::hash(random());

}
