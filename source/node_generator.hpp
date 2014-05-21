/*
 * node_generator.hpp
 *
 *  Created on: 07.07.2013
 *      Author: mike_gresens
 */

#pragma once

#include "move_generator.hpp"
#include <boost/iterator/transform_iterator.hpp>
#include <boost/iterator/filter_iterator.hpp>

namespace chess {

// NOTE:
// Not usable because filter_iterator is forward_iterator,
// we need a random access iterator. Sure?!
// tbb::parallel_for_each implies input iteration...
template <typename moves_tag>
class node_generator
{
	typedef typename move_generator<moves_tag>::entry_t move_entry_t;
	typedef std::tuple<node_t,move_t> node_entry_t;

	class move_filter_function : public std::unary_function<move_entry_t, bool>
	{
	private:
		move_t best;

	public:
		constexpr move_filter_function(const move_t best)
		:
			std::unary_function<move_entry_t, bool>(),
			best(best)
		{
		}

		constexpr bool operator()(const move_entry_t entry) const
		{
			return std::get<0>(entry) != best;
		}
	};

	class transform_function : public std::unary_function<move_entry_t, node_entry_t>
	{
	private:
		const node_t& node;

	public:
		constexpr transform_function(const node_t& node)
		:
			std::unary_function<move_entry_t, node_entry_t>(),
			node(node)
		{
		}

		inline node_entry_t operator()(const move_entry_t entry) const
		{
			const move_t move = std::get<0>(entry);
			const node_t successor(node, move, moves_tag());
			return std::make_tuple(successor, move);
//			return std::move(std::make_tuple(successor, move));
		}
	};

	class node_filter_function : public std::unary_function<node_entry_t, bool>
	{
	private:
		score_t color;

	public:
		constexpr node_filter_function(const score_t color)
		:
			std::unary_function<node_entry_t, bool>(),
			color(color)
		{
		}

		inline bool operator()(const node_entry_t entry) const
		{
			return !(std::get<0>(entry).check(color));
		}
	};

	typedef typename move_generator<moves_tag>::iterator generate_iterator;
	typedef boost::filter_iterator<move_filter_function, generate_iterator> move_filter_iterator;
	typedef boost::transform_iterator<transform_function, move_filter_iterator, node_entry_t> transform_iterator;
	typedef boost::filter_iterator<node_filter_function, transform_iterator> node_filter_iterator;

private:
	move_generator<moves_tag> generate;
	move_filter_function move_filter;
	transform_function transform;
	node_filter_function node_filter;

public:
	typedef node_filter_iterator iterator;
	typedef node_entry_t entry_t;

	inline node_generator(const node_t& node, const history_table_t& table, const move_t best)
	:
		generate(node, table),
		move_filter(best),
		transform(node),
		node_filter(node.color())
	{
	}

	inline iterator begin() const
	{
//		return boost::make_filter_iterator(node_filter, _begin, _end);
		return boost::make_filter_iterator
		(
			node_filter,
			boost::make_transform_iterator
			(
				boost::make_filter_iterator
				(
					move_filter,
					generate.begin(),
					generate.end()
				),
				transform
			),
			boost::make_transform_iterator
			(
				boost::make_filter_iterator
				(
					move_filter,
					generate.end(),
					generate.end()
				),
				transform
			)
		);
	}

	inline iterator end() const
	{
//		return filter_iterator();
//		return boost::make_filter_iterator(node_filter, _end, _end);
		return boost::make_filter_iterator
		(
			node_filter,
			boost::make_transform_iterator
			(
				boost::make_filter_iterator
				(
					move_filter,
					generate.end(),
					generate.end()
				),
				transform
			),
			boost::make_transform_iterator
			(
				boost::make_filter_iterator
				(
					move_filter,
					generate.end(),
					generate.end()
				),
				transform
			)
		);
	}
};

}
