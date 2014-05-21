/*
 * environment.hpp
 *
 *  Created on: 30.06.2013
 *      Author: mike_gresens
 */

#pragma once

#include "table/transposition_table.hpp"
#include "table/history_table.hpp"
#include "egtb/tablebase.hpp"
#include <atomic>

namespace chess {

class environment_t
{
private:
	transposition_table_t _transposition_table;
//	transposition_table<depth_replacement_tag> _transposition_table1;
//	transposition_table<always_replacement_tag> _transposition_table2;
	history_table_t _history_table;
	tablebase_t _tablebase;

	std::atomic_uint_fast64_t _node_count;
	std::atomic_uint_fast64_t _tt_lookup;
	std::atomic_uint_fast64_t _tt_hit;
	std::atomic_uint_fast64_t _tt_cutoff;
	std::atomic_uint_fast64_t _nm_search;
	std::atomic_uint_fast64_t _nm_prune;
	std::atomic_uint_fast64_t _ab_cutoff;
	std::atomic_uint_fast64_t _tb_lookup;
	std::atomic_uint_fast64_t _tb_hit;

	std::atomic_uint_fast8_t _max_height;

public:
	inline environment_t(const std::size_t tt_size, const std::string& tb_path, const std::size_t tb_size)
	:
		_transposition_table(tt_size),
//		_transposition_table1(tt_size >> 1),
//		_transposition_table2(tt_size >> 1),
		_history_table(),
		_tablebase(tb_path, tb_size),
		_node_count(),
		_tt_lookup(),
		_tt_hit(),
		_tt_cutoff(),
		_nm_search(),
		_nm_prune(),
		_ab_cutoff(),
		_tb_lookup(),
		_tb_hit(),
		_max_height()
	{
	}

	inline void clear()
	{
		_transposition_table.clear();
//		_transposition_table1.clear();
//		_transposition_table2.clear();
		_history_table.clear();
		_node_count = 0;
		_tt_lookup = 0;
		_tt_hit = 0;
		_tt_cutoff = 0;
		_nm_search = 0;
		_nm_prune = 0;
		_ab_cutoff = 0;
		_tb_lookup = 0;
		_tb_hit = 0;
		_max_height = 0;
	}

	inline transposition_table_t& transposition_table()
	{
		return _transposition_table;
	}

//	inline transposition_table<depth_replacement_tag>& transposition_table1()
//	{
//		return _transposition_table1;
//	}
//
//	inline transposition_table<always_replacement_tag>& transposition_table2()
//	{
//		return _transposition_table2;
//	}

	inline history_table_t& history_table()
	{
		return _history_table;
	}

	inline tablebase_t& tablebase()
	{
		return _tablebase;
	}

	inline std::atomic_uint_fast64_t& node_count()
	{
		return _node_count;
	}

	inline std::atomic_uint_fast64_t& tt_lookup()
	{
		return _tt_lookup;
	}

	inline std::atomic_uint_fast64_t& tt_hit()
	{
		return _tt_hit;
	}

	inline std::atomic_uint_fast64_t& tt_cutoff()
	{
		return _tt_cutoff;
	}

	inline std::atomic_uint_fast64_t& nm_search()
	{
		return _nm_search;
	}

	inline std::atomic_uint_fast64_t& nm_prune()
	{
		return _nm_prune;
	}

	inline std::atomic_uint_fast64_t& ab_cutoff()
	{
		return _ab_cutoff;
	}

	inline std::atomic_uint_fast64_t& tb_lookup()
	{
		return _tb_lookup;
	}

	inline std::atomic_uint_fast64_t& tb_hit()
	{
		return _tb_hit;
	}

	inline std::atomic_uint_fast8_t& max_height()
	{
		return _max_height;
	}
};

}
