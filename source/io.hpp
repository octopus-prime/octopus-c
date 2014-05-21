/*
 * io.hpp
 *
 *  Created on: 26.01.2013
 *      Author: mike_gresens
 */

#pragma once

#include "node.hpp"
#include "move.hpp"
#include <iostream>
#include <bitset>

namespace chess {

/**
 * Output operator.
 * @param stream The stream.
 * @param pair The node and move.
 * @return The stream.
 */
std::ostream&
operator<<(std::ostream& stream, const std::pair<const node_t&, const move_t&>& pair);

/**
 * Output operator.
 * @param stream The stream.
 * @param node The node.
 * @return The stream.
 */
std::ostream&
operator<<(std::ostream& stream, const node_t& node);

/**
 * Output operator.
 * @param stream The stream.
 * @param board The board.
 * @return The stream.
 */
std::ostream&
operator<<(std::ostream& stream, const std::bitset<64>& board);

}
