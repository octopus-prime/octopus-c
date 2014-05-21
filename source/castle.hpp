/*
 * castle.hpp
 *
 *  Created on: 08.02.2013
 *      Author: mike_gresens
 */

#pragma once

#include <cstdint>

namespace chess {

/**
 * Defines castle type.
 */
typedef std::uint_fast8_t castle_t;

/**
 * Defines castle values.
 */
enum castle : castle_t
{
	white_castle_king = 1,
	white_castle_queen = 2,
	black_castle_king = 4,
	black_castle_queen = 8,

	white_castle = white_castle_king | white_castle_queen,
	black_castle = black_castle_king | black_castle_queen
};

}
