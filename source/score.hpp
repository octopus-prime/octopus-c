/*
 * score.hpp
 *
 *  Created on: 06.02.2013
 *      Author: mike_gresens
 */

#pragma once

#include "piece.hpp"
#include <cstdint>
#include <array>

namespace chess {

/**
 * Defines score type.
 */
typedef std::int_least16_t score_t;

/**
 * Implements scoring function.
 */
class score_of_piece
:
	public std::unary_function<piece_t, score_t>
{
private:
	static constexpr std::array<score_t, 13> scoring
	{{
		0,
		+30000, +900, +500, +333, +325, +100,
		+30000, +900, +500, +333, +325, +100
	}};

public:
	/**
	 * Gets score of piece.
	 * @param piece The piece.
	 * @return The score.
	 */
	constexpr score_t operator()(const piece_t piece) const
	{
		return scoring[piece];
	}
};

/**
 * Implements scoring function.
 */
class score_of_promotion_t
:
	public std::unary_function<std::uint8_t, score_t>
{
private:
	static constexpr std::array<score_t, 5> scoring
	{{
		0,
		score_of_piece()(N) - score_of_piece()(P),
		score_of_piece()(B) - score_of_piece()(P),
		score_of_piece()(R) - score_of_piece()(P),
		score_of_piece()(Q) - score_of_piece()(P)
	}};

public:
	/**
	 * Gets score of promotion.
	 * @param promotion The promotion.
	 * @return The score.
	 */
	constexpr score_t operator()(const std::uint8_t promotion) const
	{
		return scoring[promotion];
	}
};

}
