/*
 * quiescence_search.hpp
 *
 *  Created on: 30.06.2013
 *      Author: mike_gresens
 */

#pragma once

#include "environment.hpp"
#include "move_generator.hpp"
#include "node_evaluator.hpp"

namespace chess {

class quiescence_search
{
private:
	environment_t& environment;
	node_evaluator evaluate;

public:
	quiescence_search(environment_t& environment)
	:
		environment(environment),
		evaluate()
	{
	}

	score_t operator()(const node_t& node, score_t alpha, score_t beta, const std::uint_fast8_t height)
	{
		if (height > environment.max_height())
			environment.max_height() = height;

		environment.node_count()++;

		if (node.count_pieces() <= static_cast<std::size_t>(environment.tablebase()))
		{
			environment.tb_lookup()++;
			score_t score;
			if (environment.tablebase()(node, height, score))
			{
				environment.tb_hit()++;
				return score;
			}
		}

		const score_t score = evaluate(node);
		if (score > alpha)
		{
			if (score >= beta)
				return beta;
			alpha = score;
		}

		const move_generator<active_moves_tag> generator(node);
		for (const auto entry : generator)
		{
			const move_t move = std::get<0>(entry);
			const node_t successor(node, move, active_moves_tag());
			if (successor.check(node.color()))
				continue;

			const score_t score = -operator()(successor, -beta, -alpha, height + 1);
			if (score > alpha)
			{
				if (score >= beta)
					return beta;
				alpha = score;
			}
		}

		return alpha;
	}
};

}
