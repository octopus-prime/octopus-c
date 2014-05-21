/*
 * move_generator.hpp
 *
 *  Created on: 21.06.2013
 *      Author: mike_gresens
 */

#pragma once

#include "node.hpp"
#include "move.hpp"
#include "table/history_table.hpp"
#include <tuple>

namespace chess {

template <typename moves_tag>
class move_generator
{
public:
	std::size_t size() const;

	template <typename iterator>
	iterator begin() const;

	template <typename iterator>
	iterator end() const;
};

template <>
class move_generator<passive_moves_tag>
{
public:
	typedef std::tuple<move_t> entry_t;

private:
	typedef std::array<entry_t, 128> entries_t;

public:
	typedef entries_t::const_iterator iterator;

private:
	entries_t _moves;
	entries_t::iterator _end;

public:
	move_generator(const node_t& node, const history_table_t& table);//, const killer_table::entry_t& entry, const killer_table::entry_t& entry_m2, const killer_table::entry_t& entry_p2);

	iterator begin() const
	{
		return _moves.begin();
	}

	iterator end() const
	{
		return _end;
	}

	std::size_t size() const
	{
		return std::distance(begin(), end());
	}
};

template <>
class move_generator<active_moves_tag>
{
public:
	typedef std::tuple<move_t, score_t> entry_t;

private:
	typedef std::array<entry_t, 128> entries_t;

public:
	typedef entries_t::const_iterator iterator;

private:
	entries_t _moves;
	entries_t::iterator _end;

public:
	move_generator(const node_t& node);
	move_generator(const node_t& node, const history_table_t& table);

	iterator begin() const
	{
		return _moves.begin();
	}

	iterator end() const
	{
		return _end;
	}

	std::size_t size() const
	{
		return std::distance(begin(), end());
	}
};

}
