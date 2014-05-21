/*
 * board_rotation.hpp
 *
 *  Created on: 07.02.2013
 *      Author: mike_gresens
 */

#pragma once

#include "board.hpp"
#include "square_rotation.hpp"

namespace chess {

/**
 * Board rotation.
 * @tparam direction Direction of rotation.
 */
template <direction_t direction>
class board_rotation
:
	public std::unary_function<board_t, board_t>
{
private:
	square_rotation<direction> rotate;

public:
	/**
	 * Rotates board.
	 * @param board The board.
	 * @return The rotated board.
	 */
	board_t operator()(board_t board) const
	{
		board_t rotated = 0ULL;
		while (board)
		{
			const square_t square = board_clear_square_forward(board);
			rotated |= board_of(rotate(square));
		}
		return rotated;
	}
};

// Optimizing specialization for east
template <>
class board_rotation<east>
{
public:
	constexpr board_t operator()(board_t board) const
	{
		return board;
	}
};

// Optimizing specialization for west
template <>
class board_rotation<west>
{
public:
	constexpr board_t operator()(board_t board) const
	{
		return board;
	}
};

}
