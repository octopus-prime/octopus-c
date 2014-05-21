/*
 * transposition_table.hpp
 *
 *  Created on: 19.01.2013
 *      Author: mike_gresens
 */

#pragma once

#include "node.hpp"
#include "move.hpp"
#include "score.hpp"
#include <vector>
#include <algorithm>
#include <iostream>
#include <emmintrin.h>

namespace chess {

class transposition_entry
{
public:
	enum flag_t : std::uint8_t
	{
		UNKNOWN,	//!< UNKNOWN
		UPPER,		//!< UPPER
		LOWER,		//!< LOWER
		EXACT		//!< EXACT
//		EGTB		//!< EGTB
	};

private:
	hash_t			_hash;
	move_t			_move;
	score_t			_score;
	std::uint8_t	_depth;
	flag_t			_flag;

public:
	inline constexpr transposition_entry() noexcept
	:
		_hash(),
		_move(),
		_score(),
		_depth(),
		_flag()
	{
	}

	inline constexpr transposition_entry(const hash_t hash, const move_t move, const score_t score, const flag_t flag, const std::uint8_t depth) noexcept
	:
		_hash(hash),
		_move(move),
		_score(score),
		_depth(depth),
		_flag(flag)
	{
	}

	inline transposition_entry& operator=(const transposition_entry& entry) noexcept
	{
		// Try to make operator=() an atomic instruction.
		*reinterpret_cast<__m128*>(this) = *reinterpret_cast<const __m128* const>(&entry);
		return *this;
	}

	inline transposition_entry& operator=(transposition_entry&& entry) noexcept
	{
		// Try to make operator=() an atomic instruction.
		*reinterpret_cast<__m128*>(this) = *reinterpret_cast<__m128*>(&entry);
		return *this;
	}

	inline constexpr hash_t hash() const noexcept
	{
		return _hash;
	}

	inline constexpr move_t move() const noexcept
	{
		return _move;
	}

	inline constexpr score_t score() const noexcept
	{
		return _score;
	}

	inline constexpr flag_t flag() const noexcept
	{
		return _flag;
	}

	inline constexpr std::uint8_t depth() const noexcept
	{
		return _depth;
	}
};

typedef std::tuple<transposition_entry, transposition_entry> transposition_pair;
/*
class depth_replacement_tag;
class always_replacement_tag;
*/
//template <typename replacement_tag>
class transposition_table_t
{
private:
	std::vector<transposition_entry> _entries;
//	std::size_t _size;
//	std::vector<transposition_entry> _white;
//	std::vector<transposition_entry> _black;

public:
	inline transposition_table_t(const std::size_t size)
	:
		_entries(size / sizeof(transposition_entry) + 13)
//		_size(size / sizeof(transposition_entry) / 2 + 13),//1234547),
//		_white(_size),//3571)
//		_black(_size)
	{
	}

	inline void clear()
	{
		std::fill(_entries.begin(), _entries.end(), transposition_entry());
//		std::fill(_white.begin(), _white.end(), transposition_entry());
//		std::fill(_black.begin(), _black.end(), transposition_entry());
	}

	void put(const node_t& node, const move_t move, const score_t score, const transposition_entry::flag_t flag, const std::uint8_t depth)
	{
		transposition_entry& entry = operator[](node);
		if (entry.depth() < depth)
			entry = transposition_entry(node.hash_node(), move, score, flag, depth);
	}

	inline transposition_entry& operator[](const node_t& node)
	{
		return _entries[node.hash_node() % _entries.size()];
//		const std::size_t index = node.hash_node() % _size;
//		return node.color() == white ? _white[index] : _black[index];
	}

	inline const transposition_entry& operator[](const node_t& node) const
	{
		return _entries[node.hash_node() % _entries.size()];
//		const std::size_t index = node.hash_node() % _size;
//		return node.color() == white ? _white[index] : _black[index];
	}
};

//template <>
//inline void
//transposition_table<depth_replacement_tag>::put(const node_t& node, const move_t move, const score_t score, const transposition_entry::flag_t flag, const std::uint8_t depth)
//{
//	transposition_entry& entry = operator[](node);
//	if (entry.depth() < depth)
//		entry = transposition_entry(node.hash_node(), move, score, flag, depth);
//}
//
//template <>
//inline void
//transposition_table<always_replacement_tag>::put(const node_t& node, const move_t move, const score_t score, const transposition_entry::flag_t flag, const std::uint8_t depth)
//{
//	transposition_entry& entry = operator[](node);
//	entry = transposition_entry(node.hash_node(), move, score, flag, depth);
//}

}

//namespace chess {
//
//class transposition_entry
//{
//public:
//	enum flag_t : std::uint8_t
//	{
//		UNKNOWN,	//!< UNKNOWN
//		UPPER,		//!< UPPER
//		LOWER,		//!< LOWER
//		EXACT		//!< EXACT
////		EGTB		//!< EGTB
//	};
//
//private:
//	hash_t			_hash;
//	move_t			_move;
//	score_t			_score;
//	std::uint8_t	_depth;
//	flag_t			_flag;
//
//public:
//	inline constexpr transposition_entry() noexcept
//	:
//		_hash(),
//		_move(),
//		_score(),
//		_depth(),
//		_flag()
//	{
//	}
//
//	inline constexpr transposition_entry(const hash_t hash, const move_t move, const score_t score, const flag_t flag, const std::uint8_t depth) noexcept
//	:
//		_hash(hash),
//		_move(move),
//		_score(score),
//		_depth(depth),
//		_flag(flag)
//	{
//	}
//
//	inline transposition_entry& operator=(const transposition_entry& entry) noexcept
//	{
//		// Try to make operator=() an atomic instruction.
//		*reinterpret_cast<__m128*>(this) = *reinterpret_cast<const __m128* const>(&entry);
//		return *this;
//	}
//
//	inline transposition_entry& operator=(transposition_entry&& entry) noexcept
//	{
//		// Try to make operator=() an atomic instruction.
//		*reinterpret_cast<__m128*>(this) = *reinterpret_cast<__m128*>(&entry);
//		return *this;
//	}
//
//	inline constexpr hash_t hash() const noexcept
//	{
//		return _hash;
//	}
//
//	inline constexpr move_t move() const noexcept
//	{
//		return _move;
//	}
//
//	inline constexpr score_t score() const noexcept
//	{
//		return _score;
//	}
//
//	inline constexpr flag_t flag() const noexcept
//	{
//		return _flag;
//	}
//
//	inline constexpr std::uint8_t depth() const noexcept
//	{
//		return _depth;
//	}
//};
//
//typedef std::tuple<transposition_entry, transposition_entry> transposition_pair;
///*
//class depth_replacement_tag;
//class always_replacement_tag;
//*/
////template <typename replacement_tag>
//class transposition_table
//{
//
//
//private:
//	std::size_t _size;
//	std::vector<transposition_pair> _white;
//	std::vector<transposition_pair> _black;
//
//public:
//	inline transposition_table(const std::size_t size)
//	:
//		_size(size / sizeof(transposition_pair) / 2 + 13),//1234547),
//		_white(_size),//3571)
//		_black(_size)
//	{
//	}
//
//	inline void clear()
//	{
//		std::fill(_white.begin(), _white.end(), transposition_pair());
//		std::fill(_black.begin(), _black.end(), transposition_pair());
//	}
//
//	void put(const node_t& node, const move_t move, const score_t score, const transposition_entry::flag_t flag, const std::uint8_t depth)
//	{
//		transposition_pair& entry = operator[](node);
//		if (std::get<0>(entry).depth() < depth)
//		{
////			if (std::get<1>(entry).hash() != node.hash_node())
//				std::get<1>(entry) = std::get<0>(entry);
//			std::get<0>(entry) = transposition_entry(node.hash_node(), move, score, flag, depth);
//		}
//		else
//		{
////			if (std::get<0>(entry).hash() != node.hash_node())
//				std::get<1>(entry) = transposition_entry(node.hash_node(), move, score, flag, depth);
//		}
//	}
//
//	inline transposition_pair& operator[](const node_t& node)
//	{
//		const std::size_t index = node.hash_node() % _size;
//		return node.color() == white ? _white[index] : _black[index];
//	}
//
//	inline const transposition_pair& operator[](const node_t& node) const
//	{
//		const std::size_t index = node.hash_node() % _size;
//		return node.color() == white ? _white[index] : _black[index];
//	}
//};
//
//}

//namespace chess {
//
//class transposition_entry
//{
//public:
//	enum flag_t : std::uint8_t
//	{
//		UNKNOWN,	//!< UNKNOWN
//		UPPER,		//!< UPPER
//		LOWER,		//!< LOWER
//		EXACT		//!< EXACT
////		EGTB		//!< EGTB
//	};
//
//private:
//	hash_t			_hash;
//	move_t			_move;
//	score_t			_score;
//	std::uint8_t	_depth;
//	flag_t			_flag;
//
//public:
//	inline constexpr transposition_entry() noexcept
//	:
//		_hash(),
//		_move(),
//		_score(),
//		_depth(),
//		_flag()
//	{
//	}
//
//	inline constexpr transposition_entry(const hash_t hash, const move_t move, const score_t score, const flag_t flag, const std::uint8_t depth) noexcept
//	:
//		_hash(hash),
//		_move(move),
//		_score(score),
//		_depth(depth),
//		_flag(flag)
//	{
//	}
//
//	inline transposition_entry& operator=(const transposition_entry& entry) noexcept
//	{
//		// Try to make operator=() an atomic instruction.
//		*reinterpret_cast<__m128*>(this) = *reinterpret_cast<const __m128* const>(&entry);
//		return *this;
//	}
//
//	inline transposition_entry& operator=(transposition_entry&& entry) noexcept
//	{
//		// Try to make operator=() an atomic instruction.
//		*reinterpret_cast<__m128*>(this) = *reinterpret_cast<__m128*>(&entry);
//		return *this;
//	}
//
//	inline constexpr hash_t hash() const noexcept
//	{
//		return _hash;
//	}
//
//	inline constexpr move_t move() const noexcept
//	{
//		return _move;
//	}
//
//	inline constexpr score_t score() const noexcept
//	{
//		return _score;
//	}
//
//	inline constexpr flag_t flag() const noexcept
//	{
//		return _flag;
//	}
//
//	inline constexpr std::uint8_t depth() const noexcept
//	{
//		return _depth;
//	}
//};
//
//typedef std::tuple<std::reference_wrapper<transposition_entry>, std::reference_wrapper<transposition_entry>> transposition_pair;
///*
//class depth_replacement_tag;
//class always_replacement_tag;
//*/
////template <typename replacement_tag>
//class transposition_table_t
//{
//	static constexpr std::size_t entry_size_quotient = 16 * sizeof(transposition_entry);
//	static constexpr std::size_t table_size_offset = 1234547;
//
//private:
//	std::size_t _size1;
//	std::size_t _size2;
//	std::array<std::vector<transposition_entry>, 2> _table1;
//	std::array<std::vector<transposition_entry>, 2> _table2;
////	std::vector<transposition_entry> _black1;
////	std::vector<transposition_entry> _white2;
////	std::vector<transposition_entry> _black2;
//
//public:
//	inline transposition_table_t(const std::size_t size)
//	:
//		_size1(6 * size / entry_size_quotient + table_size_offset),
//		_size2(2 * size / entry_size_quotient + table_size_offset),
//		_table1
//		{{
//			std::vector<transposition_entry>(_size1),	// T1w
//			std::vector<transposition_entry>(_size1)	// T1b
//		}},
//		_table2
//		{{
//			std::vector<transposition_entry>(_size2),	// T2w
//			std::vector<transposition_entry>(_size2)	// T2b
//		}}
////		_black1(_size1),
////		_white2(_size2),
////		_black2(_size2)
//	{
//	}
//
//	inline void clear()
//	{
//		static constexpr transposition_entry empty;
//		std::fill(_table1[0].begin(), _table1[0].end(), empty);
//		std::fill(_table1[1].begin(), _table1[1].end(), empty);
//		std::fill(_table2[0].begin(), _table2[0].end(), empty);
//		std::fill(_table2[1].begin(), _table2[1].end(), empty);
//	}
//
//	void put(const node_t& node, const move_t move, const score_t score, const transposition_entry::flag_t flag, const std::uint8_t depth)
//	{
//		transposition_pair entry = operator[](node);
//		if (std::get<0>(entry).get().depth() < depth)
//		{
//			if (std::get<1>(entry).get().hash() != node.hash_node())
//				std::get<1>(entry).get() = std::get<0>(entry).get();
//			std::get<0>(entry).get() = transposition_entry(node.hash_node(), move, score, flag, depth);
//		}
//		else
//		{
//			if (std::get<0>(entry).get().hash() != node.hash_node())
//				std::get<1>(entry).get() = transposition_entry(node.hash_node(), move, score, flag, depth);
//		}
//	}
//
//	inline transposition_pair operator[](const node_t& node)
//	{
//		const std::size_t index1 = node.hash_node() % _size1;
//		const std::size_t index2 = node.hash_node() % _size2;
//		const std::size_t indexc = node.color() != white;
//		return std::make_tuple
//		(
//			std::ref(_table1[indexc][index1]),
//			std::ref(_table2[indexc][index2])
//		);
//	}
//};
//
//}
