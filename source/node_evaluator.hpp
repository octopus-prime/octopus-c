/*
 * node_evaluator.hpp
 *
 *  Created on: 25.01.2013
 *      Author: mike_gresens
 */

#pragma once

#include "node.hpp"

namespace chess {

class node_evaluator
:
	public std::unary_function<node_t, score_t>
{
public:
	score_t operator()(const node_t& node) const;
};

}
