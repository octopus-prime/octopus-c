/*
 * history_table.hpp
 *
 *  Created on: 20.01.2013
 *      Author: mike_gresens
 */

#pragma once

#include "node.hpp"
#include "move.hpp"
#include <array>
#include <atomic>

namespace chess {
/*
class history_table
{
private:
	std::array<std::atomic_uint_least64_t, 64> _entries;

public:
	inline history_table()
	:
		_entries()
	{
		clear();
	}

	inline const std::atomic_uint_least64_t& operator[](const move_t move) const
	{
		return _entries[move.to()];
	}

	inline void put(const node_t& node, const move_t move, const std::uint_fast8_t depth)
	{
		_entries[move.to()] += std::atomic_uint_least64_t(1ULL) << (depth - 1);
//		_entries[move.from()][move.to()] += std::atomic_uint_least64_t(node[move.to()] == Piece::no && move.promoted() == 0) << (depth - 1);
	}

	inline void clear()
	{
		std::fill(_entries.begin(), _entries.end(), 0);
	}
};
*/

class history_table_t
{
private:
	std::array<std::array<std::atomic_uint_least64_t, 64>, 64> _entries;

public:
	inline history_table_t()
	:
		_entries()
	{
		clear();
	}

	inline const std::atomic_uint_least64_t& operator[](const move_t move) const
	{
		return _entries[move.from()][move.to()];
	}

	inline void put(const node_t& node, const move_t move, const std::uint_fast8_t depth)
	{
		_entries[move.from()][move.to()] += std::atomic_uint_least64_t(1ULL) << (depth >> 1);
//		_entries[move.from()][move.to()] += std::atomic_uint_least64_t(1ULL) << (depth - 1);
//		_entries[move.from()][move.to()] += std::atomic_uint_least64_t(node[move.to()] == Piece::no && move.promoted() == 0) << (depth - 1);
	}

	inline void clear()
	{
		for (auto& temp : _entries)
			std::fill(temp.begin(), temp.end(), 0);
	}
};

}
