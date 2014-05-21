/*
 * movegenerator.cpp
 *
 *  Created on: 21.06.2013
 *      Author: mike_gresens
 */

#include "move_generator.hpp"
#include "attack.hpp"
#include "occupy.hpp"
#include <algorithm>
#include <iostream>

namespace chess {

class king_piece_tag_with_rochade;

template <typename piece_tag, typename color_tag, typename moves_tag>
class mask
:
	public std::unary_function<node_t, board_t>
{
public:
	board_t operator()(const node_t& node) const;
};

template <typename piece_tag>
class mask<piece_tag, white_color_tag, all_moves_tag>
{
public:
	constexpr board_t operator()(const node_t& node) const
	{
		return ~node.board_occupy_white();
	}
};

template <typename piece_tag>
class mask<piece_tag, black_color_tag, all_moves_tag>
{
public:
	constexpr board_t operator()(const node_t& node) const
	{
		return ~node.board_occupy_black();
	}
};

template <typename piece_tag>
class mask<piece_tag, white_color_tag, active_moves_tag>
{
public:
	constexpr board_t operator()(const node_t& node) const
	{
		return node.board_occupy_black();
	}
};

template <typename piece_tag>
class mask<piece_tag, black_color_tag, active_moves_tag>
{
public:
	constexpr board_t operator()(const node_t& node) const
	{
		return node.board_occupy_white();
	}
};

template <typename piece_tag, typename color_tag>
class mask<piece_tag, color_tag, passive_moves_tag>
{
public:
	constexpr board_t operator()(const node_t& node) const
	{
		return ~node.board_occupy();
	}
};

template <typename piece_tag, typename color_tag, typename moves_tag>
class generator
{
private:
	attack_t<piece_tag> _attack;
	occupy<piece_tag, color_tag> _occupy;
	mask<piece_tag, color_tag, moves_tag> _mask;

protected:
	template <typename iter_type>
	iter_type generate(iter_type moves, const square_t fromSquare, const square_t toSquare) const
	{
		std::get<0>(*moves++) = move_t(fromSquare, toSquare);
		return moves;
	}

	template <typename iter_type>
	iter_type generate(const node_t& node, iter_type moves, board_t fromBoard) const
	{
		const board_t maskBoard = _mask(node);
		while (fromBoard)
		{
			const square_t fromSquare = board_clear_square_forward(fromBoard);
			board_t toBoard = _attack(node, fromSquare) & maskBoard;
			while (toBoard)
			{
				const square_t toSquare = board_clear_square_forward(toBoard);
				moves = generate(moves, fromSquare, toSquare);
			}
		}
		return moves;
	}

public:
	template <typename iter_type>
	iter_type operator()(const node_t& node, iter_type moves) const
	{
		return generate(node, moves, _occupy(node));
	}
};

template <typename color_tag>
class generator<king_piece_tag_with_rochade, color_tag, active_moves_tag>
{
private:
	generator<king_piece_tag, color_tag, active_moves_tag> _generate;

public:
	template <typename iter_type>
	iter_type operator()(const node_t& node, iter_type moves) const
	{
		return _generate(node, moves);
	}
};

template <>
class generator<king_piece_tag_with_rochade, white_color_tag, passive_moves_tag>
{
private:
	generator<king_piece_tag, white_color_tag, passive_moves_tag> _generate;

protected:
	template <typename iter_type>
	iter_type castles(const node_t& node, iter_type moves) const
	{
		constexpr move_t king(e1, g1);
		constexpr move_t queen(e1, c1);

		if (node.can_castle_white_king())
			std::get<0>(*moves++) = king;
		if (node.can_castle_white_queen())
			std::get<0>(*moves++) = queen;
		return moves;
	}

public:
	template <typename iter_type>
	iter_type operator()(const node_t& node, iter_type moves) const
	{
		moves = _generate(node, moves);
		moves = castles(node, moves);
		return moves;
	}
};

template <>
class generator<king_piece_tag_with_rochade, black_color_tag, passive_moves_tag>
{
private:
	generator<king_piece_tag, black_color_tag, passive_moves_tag> _generate;

protected:
	template <typename iter_type>
	iter_type castles(const node_t& node, iter_type moves) const
	{
		constexpr move_t king(e8, g8);
		constexpr move_t queen(e8, c8);

		if (node.can_castle_black_king())
			std::get<0>(*moves++) = king;
		if (node.can_castle_black_queen())
			std::get<0>(*moves++) = queen;
		return moves;
	}

public:
	template <typename iter_type>
	iter_type operator()(const node_t& node, iter_type moves) const
	{
		moves = _generate(node, moves);
		moves = castles(node, moves);
		return moves;
	}
};

/**
 * Shifts board.
 */
template <typename color_tag, std::int8_t delta>
class shift
:
	public std::unary_function<board_t, board_t>
{
public:
	board_t operator()(const board_t board) const;
};

template <std::int8_t delta>
class shift<white_color_tag, delta>
{
public:
	constexpr board_t operator()(const board_t board) const
	{
		return board << std::abs(static_cast<int>(delta));
	}
};

template <std::int8_t delta>
class shift<black_color_tag, delta>
{
public:
	constexpr board_t operator()(const board_t board) const
	{
		return board >> std::abs(static_cast<int>(delta));
	}
};

/**
 * Traits of pawn generators
 */
template <typename color_tag>
class pawn_generator_traits
{
};

template <>
class pawn_generator_traits<white_color_tag>
{
public:
	typedef board_of_rank<6> rank_single;
	typedef board_of_rank<1> rank_double;
	typedef board_of_rank<7> rank_promote;
	typedef board_of_file<0> file_left;
	typedef board_of_file<7> file_right;
	static constexpr std::int8_t delta = -8;
	static constexpr std::int8_t delta_left = -7;
	static constexpr std::int8_t delta_right = -9;
};

template <>
class pawn_generator_traits<black_color_tag>
{
public:
	typedef board_of_rank<1> rank_single;
	typedef board_of_rank<6> rank_double;
	typedef board_of_rank<0> rank_promote;
	typedef board_of_file<7> file_left;
	typedef board_of_file<0> file_right;
	static constexpr std::int8_t delta = +8;
	static constexpr std::int8_t delta_left = +7;
	static constexpr std::int8_t delta_right = +9;
};

template <std::int8_t delta>
struct passive_generator
{
	template <typename iter_type>
	iter_type operator()(iter_type moves, const square_t fromSquare, const square_t toSquare) const
	{
		std::get<0>(*moves++) = move_t(fromSquare, toSquare);
		return moves;
	}

	template <typename iter_type>
	iter_type operator()(iter_type moves, board_t toBoard) const
	{
		while (toBoard)
		{
			const square_t toSquare = board_clear_square_forward(toBoard);
			const square_t fromSquare = toSquare + delta;
			moves = operator()(moves, fromSquare, toSquare);
		}
		return moves;
	}
};

template <typename color_tag>
class passive_single_generator
{
private:
	occupy<pawn_piece_tag, color_tag> _occupy;
	shift<color_tag, pawn_generator_traits<color_tag>::delta> _shift;
	typename pawn_generator_traits<color_tag>::rank_single _rank;
	passive_generator<pawn_generator_traits<color_tag>::delta> _generator;

public:
	template <typename iter_type>
	iter_type operator()(const node_t& node, iter_type moves) const
	{
		const board_t board = _shift(_occupy(node) & ~_rank()) & ~node.board_occupy();
		return _generator(moves, board);
	}
};

template <typename color_tag>
class passive_double_generator
{
private:
	occupy<pawn_piece_tag, color_tag> _occupy;
	shift<color_tag, pawn_generator_traits<color_tag>::delta> _shift;
	typename pawn_generator_traits<color_tag>::rank_double _rank;
	passive_generator<pawn_generator_traits<color_tag>::delta * 2> _generator;

public:
	template <typename iter_type>
	iter_type operator()(const node_t& node, iter_type moves) const
	{
		const board_t pawns  = _shift(_occupy(node) & _rank()) & ~node.board_occupy();
		const board_t board  = _shift(pawns) & ~node.board_occupy();
		return _generator(moves, board);
	}
};

template <std::int8_t delta>
class promote_generator
{
private:
public:
	template <typename iter_type>
	iter_type operator()(iter_type moves, const square_t fromSquare, const square_t toSquare) const
	{
		for (std::int_fast8_t promotion = 1; promotion <= 4; ++promotion)
			std::get<0>(*moves++) = move_t(fromSquare, toSquare, promotion);
//			*moves++ = move_t(fromSquare, toSquare, promotion);
		return moves;
	}

	template <typename iter_type>
	iter_type operator()(iter_type moves, board_t toBoard) const
	{
		while (toBoard)
		{
			const square_t toSquare = board_clear_square_forward(toBoard);
			const square_t fromSquare = toSquare + delta;
			moves = operator()(moves, fromSquare, toSquare);
		}
		return moves;
	}
};

template <typename color_tag>
class active_promote_generator
{
private:
	occupy<pawn_piece_tag, color_tag> _occupy;
	shift<color_tag, pawn_generator_traits<color_tag>::delta> _shift;
	typename pawn_generator_traits<color_tag>::rank_single _rank;
	promote_generator<pawn_generator_traits<color_tag>::delta> _generator;

public:
	template <typename iter_type>
	iter_type operator()(const node_t& node, iter_type moves) const
	{
		const board_t board = _shift(_occupy(node) & _rank()) & ~node.board_occupy();
		return _generator(moves, board);
	}
};

template <std::int8_t delta, typename rank>
class capture_generator
{
private:
	promote_generator<delta> _promote_generator;
	passive_generator<delta> _passive_generator;
	rank _rank;

public:
	template <typename iter_type>
	iter_type operator()(iter_type moves, board_t toBoard, const board_t pawns, const square_t enPassant) const
	{
		moves = _promote_generator(moves, toBoard & _rank());
		moves = _passive_generator(moves, toBoard & ~_rank());
		if (enPassant)
		{
			const square_t fromSquare = enPassant + delta;
			if (board_test_square(pawns, fromSquare))
				moves = _passive_generator(moves, fromSquare, enPassant);
		}
		return moves;
	}
};

template <typename color_tag>
class active_capture_generator
{
private:
	occupy<pawn_piece_tag, color_tag> _occupy;
	mask<pawn_piece_tag, color_tag, active_moves_tag> _mask;

	typedef typename pawn_generator_traits<color_tag>::rank_promote rank;
	typedef typename pawn_generator_traits<color_tag>::file_left file_left;
	typedef typename pawn_generator_traits<color_tag>::file_right file_right;
	typedef capture_generator<pawn_generator_traits<color_tag>::delta_left, rank> generator_left;
	typedef capture_generator<pawn_generator_traits<color_tag>::delta_right, rank> generator_right;
	typedef shift<color_tag, pawn_generator_traits<color_tag>::delta_left> shift_left;
	typedef shift<color_tag, pawn_generator_traits<color_tag>::delta_right> shift_right;

protected:
	template <typename generator, typename shift, typename file, typename iter_type>
	iter_type generate(const node_t& node, iter_type moves) const
	{
		const board_t pawns = _occupy(node) & ~file()();
		const board_t board = shift()(pawns) & _mask(node);
		return generator()(moves, board, pawns, node.square_en_passant());
	}

public:
	template <typename iter_type>
	iter_type operator()(const node_t& node, iter_type moves) const
	{
		moves = generate<generator_left, shift_left, file_left>(node, moves);
		moves = generate<generator_right, shift_right, file_right>(node, moves);
		return moves;
	}
};

template <typename color_tag>
class generator<pawn_piece_tag, color_tag, passive_moves_tag>
{
private:
	passive_single_generator<color_tag> _single_generator;
	passive_double_generator<color_tag> _double_generator;

public:
	template <typename iter_type>
	iter_type operator()(const node_t& node, iter_type moves) const
	{
		moves = _single_generator(node, moves);
		moves = _double_generator(node, moves);
		return moves;
	}
};

template <typename color_tag>
class generator<pawn_piece_tag, color_tag, active_moves_tag>
{
private:
	active_promote_generator<color_tag> _promote_generator;
	active_capture_generator<color_tag> _capture_generator;

public:
	template <typename iter_type>
	iter_type operator()(const node_t& node, iter_type moves) const
	{
		moves = _promote_generator(node, moves);
		moves = _capture_generator(node, moves);
		return moves;
	}
};

//template <typename color_tag>
//class generator<pawn_piece_tag, color_tag, all_moves_tag>
//{
//private:
//	generator<pawn_piece_tag, color_tag, active_moves_tag> _active_generator;
//	generator<pawn_piece_tag, color_tag, passive_moves_tag> _passive_generator;
//
//public:
//	template <typename iter_type>
//	iter_type operator()(const node_t& node, iter_type moves) const
//	{
//		moves = _active_generator(node, moves);
//		moves = _passive_generator(node, moves);
//		return moves;
//	}
//};

template <typename color_tag, typename moves_tag>
class colored_generator
{
private:
	generator<king_piece_tag_with_rochade, color_tag, moves_tag> _king_generator;
	generator<rook_queen_piece_tag, color_tag, moves_tag> _rook_generator;
	generator<bishop_queen_piece_tag, color_tag, moves_tag> _bishop_generator;
	generator<knight_piece_tag, color_tag, moves_tag> _knight_generator;
	generator<pawn_piece_tag, color_tag, moves_tag> _pawn_generator;

public:
	template <typename iter_type>
	iter_type operator()(const node_t& node, iter_type moves) const
	{
		moves = _king_generator(node, moves);
		moves = _rook_generator(node, moves);
		moves = _bishop_generator(node, moves);
		moves = _knight_generator(node, moves);
		moves = _pawn_generator(node, moves);
		return moves;
	}
};

template <typename moves_tag>
class uncolored_generator
{
private:
	colored_generator<white_color_tag, moves_tag> white_generator;
	colored_generator<black_color_tag, moves_tag> black_generator;

public:
	template <typename iter_type>
	iter_type operator()(const node_t& node, iter_type moves) const
	{
		if (node.color() == white)
			return white_generator(node, moves);
		else
			return black_generator(node, moves);
	}
};

// TODO: replace by static_exchange_evaluator
class move_evaluator
:
	public std::unary_function<move_t, score_t>
{
private:
	const node_t& _node;
	score_of_piece _score_of_piece;
	score_of_promotion_t _score_of_promotion;

public:
	constexpr move_evaluator(const node_t& node)
	:
		_node(node),
		_score_of_piece(),
		_score_of_promotion()
	{
	}

	constexpr score_t operator()(const move_t move) const
	{
//		const piece_t piece_moved = _node[move.from()];
//		const piece_t piece_captured = _node[move.to()];
//		return ((_score_of_promotion(move.promoted()) + _score_of_piece(piece_captured)) << 4) - _score_of_piece(piece_moved);
//		const piece_t piece_moved = _node[move.from()];
//		const piece_t piece_captured = _node[move.to()];
		return ((_score_of_promotion(move.promoted()) + _score_of_piece(_node[move.to()])) << 4) - _score_of_piece(_node[move.from()]);
	}
};

constexpr std::uint32_t counts_mask = 16383;
typedef std::array<std::uint64_t, counts_mask + 1> counts_t;

template<typename moves_type, bool use_count>
class comparator
{
public:
	template <typename value_type>
	bool operator()(const value_type& move1, const value_type& move2) const;
};

template<>
class comparator<passive_moves_tag, true>
{
	typedef move_generator<passive_moves_tag>::entry_t value_type;

private:
	const counts_t& _counts;

public:
	constexpr comparator(const counts_t& counts)
	:
		_counts(counts)
	{
	}

//	constexpr bool operator()(const value_type& move1, const value_type& move2) const
	inline bool operator()(const value_type& move1, const value_type& move2) const
	{
		return _counts[std::get<0>(move1) & counts_mask] > _counts[std::get<0>(move2) & counts_mask];
	}
};

template<>
class comparator<active_moves_tag, false>
{
	typedef move_generator<active_moves_tag>::entry_t value_type;

public:
	constexpr comparator()
	{
	}

	constexpr bool operator()(const value_type& move1, const value_type& move2) const
	{
		return std::get<1>(move1) > std::get<1>(move2);
	}
};

template<>
class comparator<active_moves_tag, true>
{
	typedef move_generator<active_moves_tag>::entry_t value_type;

private:
	const counts_t& _counts;

public:
	constexpr comparator(const counts_t& counts)
	:
		_counts(counts)
	{
	}

	bool operator()(const value_type& move1, const value_type& move2) const
	{
		if (std::get<1>(move1) != std::get<1>(move2))
			return std::get<1>(move1) > std::get<1>(move2);
//		std::cout << _counts[std::get<0>(move1) & counts_mask] << ", " << _counts[std::get<0>(move2) & counts_mask] << std::endl;
		return _counts[std::get<0>(move1) & counts_mask] > _counts[std::get<0>(move2) & counts_mask];
	}
};

move_generator<passive_moves_tag>::move_generator(const node_t& node, const history_table_t& table)//, const killer_table::entry_t& entry, const killer_table::entry_t& entry_m2, const killer_table::entry_t& entry_p2)
:
	_moves(),
	_end(uncolored_generator<passive_moves_tag>()(node, _moves.begin()))
{
	counts_t counts;
	std::for_each
	(
		_moves.begin(), _end,
		[&table, /*entry, entry_m2, entry_p2,*/ &counts](const entry_t move)
		{
//			if (entry.first == std::get<0>(move))
//				counts[std::get<0>(move) & counts_mask] = std::numeric_limits<std::uint64_t>::max();
//			else if (entry.second == std::get<0>(move))
//				counts[std::get<0>(move) & counts_mask] = std::numeric_limits<std::uint64_t>::max() - 1;
//			else if (entry_m2.first == std::get<0>(move))
//				counts[std::get<0>(move) & counts_mask] = std::numeric_limits<std::uint64_t>::max() - 2;
//			else if (entry_p2.first == std::get<0>(move))
//				counts[std::get<0>(move) & counts_mask] = std::numeric_limits<std::uint64_t>::max() - 3;
//			else if (entry_m2.second == std::get<0>(move))
//				counts[std::get<0>(move) & counts_mask] = std::numeric_limits<std::uint64_t>::max() - 4;
//			else if (entry_p2.second == std::get<0>(move))
//				counts[std::get<0>(move) & counts_mask] = std::numeric_limits<std::uint64_t>::max() - 5;
//			else
				counts[std::get<0>(move) & counts_mask] = table[std::get<0>(move)];
		}
	);
	const comparator<passive_moves_tag, true> compare(counts);
	std::__insertion_sort
	(
		_moves.begin(),_end,
		compare
	);
}

move_generator<active_moves_tag>::move_generator(const node_t& node)
:
	_moves(),
	_end(uncolored_generator<active_moves_tag>()(node, _moves.begin()))
{
	const move_evaluator evaluate(node);
	std::for_each
	(
		_moves.begin(), _end,
		[&evaluate](entry_t& move)
		{
			std::get<1>(move) = evaluate(std::get<0>(move));
		}
	);
	const comparator<active_moves_tag, false> compare;
	std::__insertion_sort
	(
		_moves.begin(), _end,
		compare
	);
}

move_generator<active_moves_tag>::move_generator(const node_t& node, const history_table_t& table)
:
	_moves(),
	_end(uncolored_generator<active_moves_tag>()(node, _moves.begin()))
{
	const move_evaluator evaluate(node);
	std::for_each
	(
		_moves.begin(), _end,
		[&evaluate](entry_t& move)
		{
			std::get<1>(move) = evaluate(std::get<0>(move));
		}
	);
	counts_t counts;
	std::for_each
	(
		_moves.begin(), _end,
		[&table, &counts](const entry_t move)
		{
			counts[std::get<0>(move) & counts_mask] = table[std::get<0>(move)];
		}
	);
	const comparator<active_moves_tag, true> compare(counts);
	std::__insertion_sort
	(
		_moves.begin(), _end,
		compare
	);
}

}
