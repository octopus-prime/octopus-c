/*
 * hash.hpp
 *
 *  Created on: 08.02.2013
 *      Author: mike_gresens
 */

#pragma once

#include "piece.hpp"
#include "square.hpp"
#include "castle.hpp"
#include <array>

namespace chess {

/**
 * Defines hash type.
 */
typedef std::uint64_t hash_t;

/**
 * Implements hash function.
 */
class piece_square_hash
:
	public std::binary_function<piece_t, square_t, hash_t>
{
private:
	static const std::array<std::array<hash_t, 64>, 13> hash;

public:
	/**
	 * Gets hash.
	 * @param piece The piece.
	 * @param square The square.
	 * @return The hash.
	 */
	hash_t operator()(const piece_t piece, const square_t square) const
	{
		return hash[piece][square];
	}
};

/**
 * Implements hash function.
 */
class en_passant_hash
:
	public std::unary_function<square_t, hash_t>
{
private:
	static const std::array<hash_t, 64> hash;

public:
	/**
	 * Gets hash.
	 * @param square The square.
	 * @return The hash.
	 */
	hash_t operator()(const square_t square) const
	{
		return hash[square];
	}
};

/**
 * Implements hash function.
 */
class castle_hash
:
	public std::unary_function<castle_t, hash_t>
{
private:
	static const std::array<hash_t, 16> hash;

public:
	/**
	 * Gets hash.
	 * @param castle The castle.
	 * @return The hash.
	 */
	hash_t operator()(const castle_t castle) const
	{
		return hash[castle];
	}
};

/**
 * Implements hash function.
 */
class color_hash
{
private:
	static const hash_t hash;

public:
	/**
	 * Gets hash.
	 * @return The hash.
	 */
	hash_t operator()() const
	{
		return hash;
	}
};

}
