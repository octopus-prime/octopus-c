/*
 * color.hpp
 *
 *  Created on: 19.06.2013
 *      Author: mike_gresens
 */

#pragma once

#include <cstdint>

namespace chess {

/**
 * Defines color type.
 */
typedef std::int16_t color_t;

/**
 * Defines color values.
 */
enum color : color_t
{
	white = +1,
	black = -1
};

}
