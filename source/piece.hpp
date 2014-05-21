/*
 * piece.hpp
 *
 *  Created on: 06.02.2013
 *      Author: mike_gresens
 */

#pragma once

#include <cstdint>

namespace chess {

/**
 * Defines piece type.
 */
typedef std::uint_fast8_t piece_t;

/**
 * Defines piece values.
 */
enum piece : piece_t
{
	no_piece,
	K, Q, R, B, N, P,
	k, q, r, b, n, p
};

}
