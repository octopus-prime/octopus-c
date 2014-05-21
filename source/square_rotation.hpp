/*
 * square_rotation.hpp
 *
 *  Created on: 17.06.2013
 *      Author: mike_gresens
 */

#pragma once

#include "square.hpp"
#include "direction.hpp"
#include <array>

namespace chess {

/**
 * Square rotation.
 * @tparam direction Direction of rotation.
 */
template <direction_t direction>
class square_rotation
:
	public std::unary_function<square_t, square_t>
{
public:
	/**
	 * Rotates square.
	 * @param square The square.
	 * @return The rotated square.
	 */
	square_t operator()(const square_t square) const;
};

// Optimizing specialization for east
template <>
class square_rotation<east>
{
public:
	constexpr square_t operator()(const square_t square) const
	{
		return square;
	}
};

// Optimizing specialization for west
template <>
class square_rotation<west>
{
public:
	constexpr square_t operator()(const square_t square) const
	{
		return square;
	}
};

// Optimizing specialization for north
template <>
class square_rotation<north>
{
public:
	constexpr square_t operator()(const square_t square) const
	{
		return square_of(rank_of(square), file_of(square));
	}
};

// Optimizing specialization for south
template <>
class square_rotation<south>
{
public:
	constexpr square_t operator()(const square_t square) const
	{
		return square_of(rank_of(square), file_of(square));
	}
};

// Lookup specialization for north_west
template <>
class square_rotation<north_west>
{
private:
	static constexpr std::array<square_t, 64> rotation
	{{
		a1, b1, d1, g1, c2, h2, f3, e4,
		c1, e1, h1, d2, a3, g3, f4, e5,
		f1, a2, e2, b3, h3, g4, f5, d6,
		b2, f2, c3, a4, h4, g5, e6, b7,
		g2, d3, b4, a5, h5, f6, c7, g7,
		e3, c4, b5, a6, g6, d7, h7, c8,
		d4, c5, b6, h6, e7, a8, d8, f8,
		d5, c6, a7, f7, b8, e8, g8, h8
	}};

public:
	constexpr square_t operator()(const square_t square) const
	{
		return rotation[square];
	}
};

// Lookup specialization for north_east
template <>
class square_rotation<north_east>
{
private:
	static constexpr std::array<square_t, 64> rotation
	{{
		e4, f3, h2, c2, g1, d1, b1, a1,
		e5, f4, g3, a3, d2, h1, e1, c1,
		d6, f5, g4, h3, b3, e2, a2, f1,
		b7, e6, g5, h4, a4, c3, f2, b2,
		g7, c7, f6, h5, a5, b4, d3, g2,
		c8, h7, d7, g6, a6, b5, c4, e3,
		f8, d8, a8, e7, h6, b6, c5, d4,
		h8, g8, e8, b8, f7, a7, c6, d5
	}};

public:
	constexpr square_t operator()(const square_t square) const
	{
		return rotation[square];
	}
};

// Lookup specialization for south_east
template <>
class square_rotation<south_east>
{
private:
	static constexpr std::array<square_t, 64> rotation
	{{
		a1, b1, a2, c1, b2, a3, d1, c2,
		b3, a4, e1, d2, c3, b4, a5, f1,
		e2, d3, c4, b5, a6, g1, f2, e3,
		d4, c5, b6, a7, h1, g2, f3, e4,
		d5, c6, b7, a8, h2, g3, f4, e5,
		d6, c7, b8, h3, g4, f5, e6, d7,
		c8, h4, g5, f6, e7, d8, h5, g6,
		f7, e8, h6, g7, f8, h7, g8, h8
	}};

public:
	constexpr square_t operator()(const square_t square) const
	{
		return rotation[square];
	}
};

// Lookup specialization for south_west
template <>
class square_rotation<south_west>
{
private:
	static constexpr std::array<square_t, 64> rotation
	{{
		h1, g1, h2, f1, g2, h3, e1, f2,
		g3, h4, d1, e2, f3, g4, h5, c1,
		d2, e3, f4, g5, h6, b1, c2, d3,
		e4, f5, g6, h7, a1, b2, c3, d4,
		e5, f6, g7, h8, a2, b3, c4, d5,
		e6, f7, g8, a3, b4, c5, d6, e7,
		f8, a4, b5, c6, d7, e8, a5, b6,
		c7, d8, a6, b7, c8, a7, b8, a8
	}};

public:
	constexpr square_t operator()(const square_t square) const
	{
		return rotation[square];
	}
};

}
