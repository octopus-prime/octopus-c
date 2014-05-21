/*
 * node_attacker.hpp
 *
 *  Created on: 20.06.2013
 *      Author: mike_gresens
 */

#pragma once

#include "node.hpp"
#include <emmintrin.h>

namespace chess {

typedef __m128i board_wb_t;

class node_attacker
{
	template <typename piece_tag>
	class attacker;

public:
	board_wb_t operator()(const node_t& node) const;
};

}
