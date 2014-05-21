/*
 * direction.hpp
 *
 *  Created on: 06.02.2013
 *      Author: mike_gresens
 */

#pragma once

#include <cstdint>

namespace chess {

/**
 * Defines direction type.
 */
typedef std::int_fast8_t direction_t;

/**
 * Defines direction values.
 */
enum direction : direction_t
{
	north = +8,
	south = -8,
	east = +1,
	west = -1,
	north_east = north + east,
	north_west = north + west,
	south_east = south + east,
	south_west = south + west
};

}
