/*
 * square.hpp
 *
 *  Created on: 06.02.2013
 *      Author: mike_gresens
 */

#pragma once

#include <cstdint>

/**
 * Chess
 */
namespace chess {

/**
 * Defines square type.
 */
typedef std::uint_fast8_t square_t;

/**
 * Defines square rank type.
 */
typedef std::uint_fast8_t square_rank_t;

/**
 * Defines square file type.
 */
typedef std::uint_fast8_t square_file_t;

/**
 * Gets square of file and rank.
 * @param file The file.
 * @param rank The rank.
 * @return The square.
 */
constexpr square_t
square_of(const square_file_t file, const square_rank_t rank)
{
	return rank << 3 | file;
}

/**
 * Gets rank of square.
 * @param square The square.
 * @return The rank.
 */
constexpr square_rank_t
rank_of(const square_t square)
{
	return square >> 3;
}

/**
 * Gets file of square.
 * @param square The square.
 * @return The file.
 */
constexpr square_file_t
file_of(const square_t square)
{
	return square & 7;
}

/**
 * Defines square values.
 */
enum square : square_t
{
	a1, b1, c1, d1, e1, f1, g1, h1,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a8, b8, c8, d8, e8, f8, g8, h8
};

}
