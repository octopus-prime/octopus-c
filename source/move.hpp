/*
 * move.hpp
 *
 *  Created on: 07.02.2013
 *      Author: mike_gresens
 */

#pragma once

#include "square.hpp"

namespace chess {

/**
 * Implements move type.
 */
class move_t
{
private:
	square_t		_from;
	square_t		_to;
	std::uint8_t	_promoted;
	std::uint8_t	_check;

public:
	/**
	 * Constructor.
	 */
	constexpr move_t()
	:
		_from(),
		_to(),
		_promoted(),
		_check()
	{
	}

	/**
	 * Constructor.
	 * @param from The from square.
	 * @param to The to square.
	 * @param promoted The promotion index.
	 */
	constexpr move_t(const square_t from, const square_t to, const std::uint8_t promoted = 0)
	:
		_from(from),
		_to(to),
		_promoted(promoted),
		_check()
	{
	}

	/**
	 * Gets from square.
	 * @return The square.
	 */
	constexpr square_t from() const
	{
		return _from;
	}

	/**
	 * Gets to square.
	 * @return The square.
	 */
	constexpr square_t to() const
	{
		return _to;
	}

	/**
	 * Gets promotion index.
	 * @return The index.
	 */
	constexpr std::uint8_t promoted() const
	{
		return _promoted;
	}

	/**
	 * Gets check flag.
	 * @return The flag.
	 */
	constexpr bool check() const
	{
		return _check;
	}

	/**
	 * Sets check flag.
	 * @param check The flag.
	 */
	inline void check(const bool check)
	{
		_check = check;
	}

	/**
	 * Casts to index.
	 * @return The index.
	 */
	inline operator std::uint32_t() const
	{
		constexpr std::uint32_t _mask = (1 << 24) - 1;
		return *reinterpret_cast<const std::uint32_t*>(this) & _mask;
	}
};

}
