/*
 * principal_variation_search.hpp
 *
 *  Created on: 30.06.2013
 *      Author: mike_gresens
 */

#pragma once

#include "environment.hpp"
#include "quiescence_search.hpp"
#include "attack.hpp"
#include "score.hpp"
#include "io.hpp"
#include <algorithm>
#include <thread>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <tbb/parallel_for_each.h>

#include "node_generator.hpp"

namespace chess {

constexpr std::uint_fast8_t min_parallel_depth = 4;

//typedef std::array<move_t, 64> line_t;

class principal_variation_search
{
private:
	environment_t& environment;
	quiescence_search search;
	attack_t<king_piece_tag> king_attack;
	score_of_promotion_t score_of_promotion;
	std::size_t threads;

protected:
	static inline std::uint_fast8_t repetition(const node_t& leaf)
	{
		std::uint_fast8_t count = 1;
		for (const node_t* node = leaf.parent(); node && node->count_half_moves(); node = node->parent())
			count += (leaf.hash_node() == node->hash_node());
		return count;
	}

	inline bool try_null(const node_t& node) const
	{
		if (node.color() == white)
			return node.count_pieces_white() > 3 && node.boardNBRWhite() && !(node.board_attack_black() & king_attack(node, node.square_king_white()));
		else
			return node.count_pieces_black() > 3 && node.boardNBRBlack() && !(node.board_attack_white() & king_attack(node, node.square_king_black()));
	}

	static inline std::uint_fast8_t reduce(const std::uint_fast8_t depth)
	{
		return (3 + (depth > 6));
	}

public:
	principal_variation_search(environment_t& environment)
	:
		environment(environment),
		search(environment),
		king_attack(),
		score_of_promotion(),
		threads(std::thread::hardware_concurrency())
	{
	}

	score_t operator()(const node_t& node, score_t alpha, score_t beta, std::uint_fast8_t depth, const std::uint_fast8_t height, const move_t moved)//, line_t& pline)
	{
//		line_t line;

		if (height > environment.max_height())
			environment.max_height() = height;

		if (repetition(node) >= 3 || node.count_half_moves() >= 50)
		{
			environment.node_count()++;
			return 0;
		}

		const bool check = node.check(node.color());
		depth += check;

//		if (depth == 0)
//			return search(node, alpha, beta, height);

		environment.node_count()++;

		move_t best;
		score_t old = alpha;

		environment.tt_lookup()++;
//		const transposition_pair& tt_pair = environment.transposition_table()[node];
//		transposition_entry& entry = std::get<0>(tt_pair);
//		if (entry.hash() != node.hash_node())
//			entry = std::get<1>(tt_pair);
		const transposition_entry& entry = environment.transposition_table()[node];
		if (entry.hash() == node.hash_node())
		{
			environment.tt_hit()++;
			if (entry.depth() >= depth)
			{
				const score_t score = entry.score();
				switch (entry.flag())
				{
				case transposition_entry::EXACT:
					return score;
				case transposition_entry::LOWER:
					if (score > alpha)
						alpha = score;
					break;
				case transposition_entry::UPPER:
					if (score < beta)
						beta = score;
					break;
				default:
					break;
				}
				if (alpha >= beta)
				{
					environment.tt_cutoff()++;
					return beta;
				}
			}
			best = entry.move();
//			if (score > alpha)
//				pline.front() = entry.move();
		}
/*
		if (!best)
		{
			environment.tt_lookup()++;
			const transposition_entry& entry = environment.transposition_table2()[node];
			if (entry.hash() == node.hash_node())
			{
				environment.tt_hit()++;
				if (entry.depth() >= depth)
				{
					const score_t score = entry.score();
					switch (entry.flag())
					{
					case transposition_entry::EXACT:
						return score;
					case transposition_entry::LOWER:
						if (score > alpha)
							alpha = score;
						break;
					case transposition_entry::UPPER:
						if (score < beta)
							beta = score;
						break;
					default:
						break;
					}
					if (alpha >= beta)
					{
						environment.tt_cutoff()++;
						return beta;
					}
				}
				best = entry.move();
			}
		}
*/
//		const bool check = node.check(node.color());
//		depth += check;
//
		if (depth == 0)
			return search(node, alpha, beta, height);

		const std::uint_fast8_t reduction = reduce(depth);
		if (depth >= reduction && moved && !check && try_null(node))
		{
			environment.nm_search()++;
			const square_t enPassant = const_cast<node_t&>(node).flip(0);
			const score_t score = -operator()(node, -beta, -beta + 1, depth - reduction, height + 1, move_t());//, line);
			const_cast<node_t&>(node).flip(enPassant);
			if (score >= beta)
			{
				environment.nm_prune()++;
				return beta;
			}
		}

//		if (!best && depth > 2)
//		{
//			operator()(node, alpha, beta, depth - 2, height, move_t());
//			const transposition_entry& entry = environment.transposition_table1()[node];
//			if (entry.hash() == node.hash_node())
//				best = entry.move();
//			else
//			{
//				const transposition_entry& entry = environment.transposition_table2()[node];
//				if (entry.hash() == node.hash_node())
//					best = entry.move();
//			}
//		}

		if (!best && depth > 2)
		{
			operator()(node, alpha, beta, depth - 2, height, move_t());//, line);
//			const transposition_pair& tt_pair = environment.transposition_table()[node];
//			transposition_entry& entry = std::get<0>(tt_pair);
//			if (entry.hash() != node.hash_node())
//				entry = std::get<1>(tt_pair);
			const transposition_entry& entry = environment.transposition_table()[node];
			if (entry.hash() == node.hash_node())
				best = entry.move();
//			else
//			{
//				const transposition_entry& entry = environment.transposition_table2()[node];
//				if (entry.hash() == node.hash_node())
//					best = entry.move();
//			}
		}

		if (best)
		{
			try
			{
				const node_t successor(node, best, active_moves_tag());
				const score_t score = -operator()(successor, -beta, -alpha, depth - 1, height + 1, best);//, line);
				if (score > alpha)
				{
//					pline.front() = best;
//					std::copy(line.begin(), line.end() - 1, pline.begin() + 1);
					if (score >= beta)
					{
						environment.transposition_table().put(node, best, beta, transposition_entry::LOWER, depth);
//						environment.transposition_table2().put(node, best, beta, transposition_entry::LOWER, depth);
						environment.history_table().put(node, best, depth);
						environment.ab_cutoff()++;
						return beta;
					}
					alpha = score;
				}
			}
			catch (...)
			{
				// Illegal move detected. Could come from type-1 collision in TT. Discard "best" move.
				best = move_t();
			}
		}

		const move_t pv_move = best;

		bool prune = false;
		score_t scoreX;
		const score_t scoreM = node.score_material() * node.color();
		switch (depth)
		{
		case 3:
			if (scoreM + 900 <= alpha)
				depth -= 1;
			break;
		case 2:
			scoreX = scoreM + 500;
			if (scoreX <= alpha)
				prune = true;
			break;
		case 1:
			scoreX = scoreM + 333;
			if (scoreX <= alpha)
				prune = true;
			break;
		}

		std::atomic_uint_fast8_t legal(static_cast<bool>(best));

		tbb::task_group_context context;

		const move_generator<active_moves_tag> generator_a(node);//, environment.history_table());

//		const node_generator<active_moves_tag> generator_a(node, environment.history_table(), pv_move);
//
//		const auto search_a = [&](const node_generator<active_moves_tag>::entry_t entry)
//		{
//			if (!context.is_group_execution_cancelled())
//			{
//				const node_t successor(std::get<0>(entry));
//				move_t move(std::get<1>(entry));
//
//				legal++;
//				move.check(successor.check(successor.color()));
//
////				if (!check && !move.check() && prune && score_of_piece()(node[move.to()]) + score_of_promotion(move.promoted()) + scoreX <= alpha)
////					return;
//
//				if (!context.is_group_execution_cancelled())
//				{
//					score_t score = -operator()(successor, -alpha - 1, -alpha, depth - 1, height + 1, move);
//
//					if (!context.is_group_execution_cancelled())
//					{
//						if (score > alpha && score < beta)
//							score = -operator()(successor, -beta, -alpha, depth - 1, height + 1, move);
//						if (score > alpha)
//						{
//							alpha = score;
//							best = move;
//							if (score >= beta)
//								context.cancel_group_execution();
//						}
//					}
//				}
//			}
//		};

		const auto search_a = [&](const move_generator<active_moves_tag>::entry_t entry)
		{
			if (!context.is_group_execution_cancelled())
			{
				move_t move = std::get<0>(entry);
				if (move == pv_move)
					return;

				const node_t successor(node, move, active_moves_tag());
				if (successor.check(node.color()))
					return;

				legal++;
				move.check(successor.check(successor.color()));

				if (!check && !move.check() && prune && score_of_piece()(node[move.to()]) + score_of_promotion(move.promoted()) + scoreX <= alpha)
					return;

				if (!context.is_group_execution_cancelled())
				{
					score_t score = -operator()(successor, -alpha - 1, -alpha, depth - 1, height + 1, move);//, line);

					if (!context.is_group_execution_cancelled())
					{
						if (score > alpha && score < beta)
							score = -operator()(successor, -beta, -alpha, depth - 1, height + 1, move);//, line);

						if (/*!context.is_group_execution_cancelled() &&*/ score > alpha)
						{
//							if (score < beta)
//							{
//								pline.front() = move;
//								std::copy(line.begin(), line.end() - 1, pline.begin() + 1);
//							}
//							if (score > alpha)
							{
								alpha = score;
								best = move;
								if (score >= beta)
									context.cancel_group_execution();
							}
						}
					}
				}
			}
		};

		if (/*!check &&*/ depth > min_parallel_depth && generator_a.size() >= threads)
			tbb::parallel_for_each(generator_a.begin(), generator_a.end(), search_a, context);
		else
			std::for_each(generator_a.begin(), generator_a.end(), search_a);

		if (alpha >= beta)
		{
			environment.transposition_table().put(node, best, beta, transposition_entry::LOWER, depth);
//			environment.transposition_table2().put(node, best, beta, transposition_entry::LOWER, depth);
			environment.history_table().put(node, best, depth);
			environment.ab_cutoff()++;
			return beta;
		}

		const move_generator<passive_moves_tag> generator_p(node, environment.history_table());
//		const node_generator<passive_moves_tag> generator_p(node, environment.history_table(), pv_move);
//
//		const auto search_p = [&](const node_generator<passive_moves_tag>::entry_t entry)
//		{
//			if (!context.is_group_execution_cancelled())
//			{
//				const node_t successor(std::get<0>(entry));
//				move_t move = std::get<1>(entry);
//
//				legal++;
//				move.check(successor.check(successor.color()));
//
////				if (!check && !move.check() && prune)
////					return;
//
//				if (!context.is_group_execution_cancelled())
//				{
//					const std::size_t reduction = 0;//(!check && !move.check()) && (depth > 2) && (legal > 6 * depth);
//
//					score_t score = -operator()(successor, -alpha - 1, -alpha, depth - 1 - reduction, height + 1, move);
//
//					if (!context.is_group_execution_cancelled())
//					{
//						if (score > alpha && score < beta)
//							score = -operator()(successor, -beta, -alpha, depth - 1 - reduction, height + 1, move);
//						if (score > alpha)
//						{
//							alpha = score;
//							best = move;
//							if (score >= beta)
//								context.cancel_group_execution();
//						}
//					}
//				}
//			}
//		};

		const auto search_p = [&](const move_generator<passive_moves_tag>::entry_t entry)
		{
			if (!context.is_group_execution_cancelled())
			{
				move_t move = std::get<0>(entry);
				if (move == pv_move)
					return;

				const node_t successor(node, move, passive_moves_tag());
				if (successor.check(node.color()))
					return;

				legal++;
				move.check(successor.check(successor.color()));

				if (!check && !move.check() && prune)
					return;

				if (!context.is_group_execution_cancelled())
				{
					const std::size_t reduction = (!check && !move.check()) && (depth > 4) && (legal > 4 * depth);// + 4);

					score_t score = -operator()(successor, -alpha - 1, -alpha, depth - 1 - reduction, height + 1, move);//, line);

					if (!context.is_group_execution_cancelled())
					{
						if (score > alpha && score < beta)
							score = -operator()(successor, -beta, -alpha, depth - 1 - reduction, height + 1, move);//, line);
						if (/*!context.is_group_execution_cancelled() &&*/ score > alpha)
						{
//							if (score > alpha)
							{
//								if (score < beta)
//								{
//									pline.front() = move;
//									std::copy(line.begin(), line.end() - 1, pline.begin() + 1);
//								}
								alpha = score;
								best = move;
								if (score >= beta)
									context.cancel_group_execution();
							}
						}
					}
				}
			}
		};

		if (/*!check &&*/ depth > min_parallel_depth && generator_p.size() >= threads)
			tbb::parallel_for_each(generator_p.begin(), generator_p.end(), search_p, context);
		else
			std::for_each(generator_p.begin(), generator_p.end(), search_p);

		if (alpha >= beta)
		{
			environment.transposition_table().put(node, best, beta, transposition_entry::LOWER, depth);
//			environment.transposition_table2().put(node, best, beta, transposition_entry::LOWER, depth);
			environment.history_table().put(node, best, depth);
			environment.ab_cutoff()++;
			return beta;
		}

		if (!legal)
		{
			const score_t score = check ? -30000 + height : 0;
			environment.transposition_table().put(node, move_t(), score, transposition_entry::EXACT, depth);
//			environment.transposition_table2().put(node, move_t(), score, transposition_entry::EXACT, depth);
			return score;
		}

		const transposition_entry::flag_t flag = alpha > old ? transposition_entry::EXACT : transposition_entry::UPPER;
		environment.transposition_table().put(node, best, alpha, flag, depth);
		if (flag == transposition_entry::EXACT)
		{
//		environment.transposition_table().put(node, best, alpha, flag, depth);
//		environment.transposition_table2().put(node, best, alpha, flag, depth);
//		environment.history_table().put(node, best, depth);
		}
//		if (alpha > old && alpha < beta)
//		{
//		pline.front() = best;
//		std::copy(line.begin(), line.end() - 1, pline.begin() + 1);
//		}
		return alpha;
	}

	template <typename iterator>
	iterator operator()(const node_t& root, const std::uint_fast8_t depth, iterator pv) const
	{
		node_t node = root;
		for (std::uint_fast8_t index = 0; index < depth; index++)
		{
			const transposition_entry& entry = environment.transposition_table()[node];
//			transposition_pair tt_pair = environment.transposition_table()[node];
//			transposition_entry& entry = std::get<0>(tt_pair);
//			if (entry.hash() != node.hash_node())
//				entry = std::get<1>(tt_pair);
			if (entry.hash() != node.hash_node())
				break;
			const move_t move = entry.move();
			if (!move)
				break;
//			boost::format format("%d. %s");
//			format % int(node.count_full_moves()) % std::make_pair(std::ref(node), std::ref(move));
//			*pv++ = format.str();
			*pv++ = boost::lexical_cast<std::string>(std::make_pair(std::ref(node), std::ref(move)));
			node = node_t(node, move, active_moves_tag());
		}
		return pv;
	}
};

}
