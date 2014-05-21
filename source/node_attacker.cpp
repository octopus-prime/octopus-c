/*
 * node_attacker.hpp
 *
 *  Created on: 20.06.2013
 *      Author: mike_gresens
 */

#include "node_attacker.hpp"
#include "attack.hpp"
#include <emmintrin.h>
#include <immintrin.h>

namespace chess {

template <typename piece_tag>
class node_attacker::attacker
{
public:
	board_wb_t operator()(const node_t& node) const;
};

template <>
class node_attacker::attacker<king_piece_tag>
{
private:
	attack_t<king_piece_tag> attack;

public:
	board_wb_t operator()(const node_t& node) const
	{
		return _mm_set_epi64x
		(
			attack(node, node.square_king_black()),
			attack(node, node.square_king_white())
		);
	}
};

template <>
class node_attacker::attacker<rook_queen_piece_tag>
{
private:
	template <typename board, size_t size>
	inline __m128i make_positive(__m128i rooks, __m128i empty) const
	{
		static const auto exclude = _mm_set1_epi64x(~board()());
		empty &= exclude;
		rooks |= empty & _mm_slli_epi64(rooks, 1 * size);
		empty &= _mm_slli_epi64(empty, 1 * size);
		rooks |= empty & _mm_slli_epi64(rooks, 2 * size);
		empty &= _mm_slli_epi64(empty, 2 * size);
		rooks |= empty & _mm_slli_epi64(rooks, 4 * size);
		return exclude & _mm_slli_epi64(rooks, size);
	}

	template <typename board, size_t size>
	inline __m128i make_negative(__m128i rooks, __m128i empty) const
	{
		static const auto exclude = _mm_set1_epi64x(~board()());
		empty &= exclude;
		rooks |= empty & _mm_srli_epi64(rooks, 1 * size);
		empty &= _mm_srli_epi64(empty, 1 * size);
		rooks |= empty & _mm_srli_epi64(rooks, 2 * size);
		empty &= _mm_srli_epi64(empty, 2 * size);
		rooks |= empty & _mm_srli_epi64(rooks, 4 * size);
		return exclude & _mm_srli_epi64(rooks, size);
	}

public:
	board_wb_t operator()(const node_t& node) const
	{
		const auto rooks = _mm_set_epi64x(node.board_rooks_queens_black(), node.board_rooks_queens_white());
		const auto empty = _mm_set1_epi64x(~node.board_occupy());
		return	make_positive<board_of_file<0>, 1>(rooks, empty) |	// west
				make_negative<board_of_file<7>, 1>(rooks, empty) |	// east
				make_positive<board_of_rank<0>, 8>(rooks, empty) |	// north
				make_negative<board_of_rank<7>, 8>(rooks, empty) ;	// south
	}
};
/*
template <>
class node_attacker::attacker<rook_queen_piece_tag>
{
private:
	inline __m256i make(__m256i rooks, __m256i empty) const
	{
		static const auto exclude = _mm_set1_epi64x(~board()());
		empty &= exclude;
		rooks |= empty & _mm_slli_epi64(rooks, 1 * size);
		empty &= _mm_slli_epi64(empty, 1 * size);
		rooks |= empty & _mm_slli_epi64(rooks, 2 * size);
		empty &= _mm_slli_epi64(empty, 2 * size);
		rooks |= empty & _mm_slli_epi64(rooks, 4 * size);
		return exclude & _mm_slli_epi64(rooks, size);
	}

public:
	board_wb_t operator()(const node_t& node) const
	{
		const auto rooks = _mm256_set1_epi64x(node.board_rooks_queens_black(), node.board_rooks_queens_white());
		const auto empty = _mm256_set1_epi64x(~node.board_occupy());
		return	make_positive<board_of_file<0>, 1>(rooks, empty) |	// west
				make_negative<board_of_file<7>, 1>(rooks, empty) |	// east
				make_positive<board_of_rank<0>, 8>(rooks, empty) |	// north
				make_negative<board_of_rank<7>, 8>(rooks, empty) ;	// south
	}
};
*/
template <>
class node_attacker::attacker<bishop_queen_piece_tag>
{
private:
	template <typename rank, typename file, size_t size>
	inline __m128i make_positive(__m128i bishops, __m128i empty) const
	{
		constexpr rank r;
		constexpr file f;
//		static const auto exclude = _mm_set1_epi64x(~(rank()() | file()()));
		static const auto exclude = _mm_set1_epi64x(~(r() | f()));
		empty &= exclude;
		bishops |= empty & _mm_slli_epi64(bishops, 1 * size);
		empty &= _mm_slli_epi64(empty, 1 * size);
		bishops |= empty & _mm_slli_epi64(bishops, 2 * size);
		empty &= _mm_slli_epi64(empty, 2 * size);
		bishops |= empty & _mm_slli_epi64(bishops, 4 * size);
		empty &= _mm_slli_epi64(empty, 4 * size);
		bishops |= empty & _mm_slli_epi64(bishops, 8 * size);
		return exclude & _mm_slli_epi64(bishops, size);
	}

	template <typename rank, typename file, size_t size>
	inline __m128i make_negative(__m128i bishops, __m128i empty) const
	{
		constexpr rank r;
		constexpr file f;
//		static const auto exclude = _mm_set1_epi64x(~(rank()() | file()()));
		static const auto exclude = _mm_set1_epi64x(~(r() | f()));
		empty &= exclude;
		bishops |= empty & _mm_srli_epi64(bishops, 1 * size);
		empty &= _mm_srli_epi64(empty, 1 * size);
		bishops |= empty & _mm_srli_epi64(bishops, 2 * size);
		empty &= _mm_srli_epi64(empty, 2 * size);
		bishops |= empty & _mm_srli_epi64(bishops, 4 * size);
		empty &= _mm_srli_epi64(empty, 4 * size);
		bishops |= empty & _mm_srli_epi64(bishops, 8 * size);
		return exclude & _mm_srli_epi64(bishops, size);
	}

public:
	board_wb_t operator()(const node_t& node) const
	{
		const auto bishops = _mm_set_epi64x(node.board_bishops_queens_black(), node.board_bishops_queens_white());
		const auto empty = _mm_set1_epi64x(~node.board_occupy());
		return	make_positive<board_of_file<7>, board_of_rank<0>, 7>(bishops, empty) |	// north-east
				make_negative<board_of_file<0>, board_of_rank<7>, 7>(bishops, empty) |	// south-west
				make_positive<board_of_file<0>, board_of_rank<0>, 9>(bishops, empty) |	// north-east
				make_negative<board_of_file<7>, board_of_rank<7>, 9>(bishops, empty) ;	// south-west
	}
};

template <>
class node_attacker::attacker<knight_piece_tag>
{
private:
	attack_t<knight_piece_tag> _attack;

protected:
	board_t generate(const node_t& node, board_t board) const
	{
		board_t attack = 0ULL;
		while (board)
		{
			const square_t square = board_clear_square_forward(board);
			attack |= _attack(node, square);
		}
		return attack;
	}

public:
	board_wb_t operator()(const node_t& node) const
	{
		return _mm_set_epi64x
		(
			generate(node, node.board_knights_black()),
			generate(node, node.board_knights_white())
		);
	}
};

template <>
class node_attacker::attacker<pawn_piece_tag>
{
private:
	static constexpr board_t not_file_a = ~board_of_file<0>()();
	static constexpr board_t not_file_h = ~board_of_file<7>()();

protected:
	inline board_t white(const node_t& node) const
	{
		const board_t board = node.board_pawns_white();
		return ((board & not_file_a) << 7) | ((board & not_file_h) << 9);
	}

	inline board_t black(const node_t& node) const
	{
		const board_t board = node.board_pawns_black();
		return ((board & not_file_a) >> 9) | ((board & not_file_h) >> 7);
	}

public:
	board_wb_t operator()(const node_t& node) const
	{
		return _mm_set_epi64x
		(
			black(node),
			white(node)
		);
	}
};

board_wb_t
node_attacker::operator()(const node_t& node) const
{
	return	attacker<king_piece_tag>()(node) |
			attacker<rook_queen_piece_tag>()(node) |
			attacker<bishop_queen_piece_tag>()(node) |
			attacker<knight_piece_tag>()(node) |
			attacker<pawn_piece_tag>()(node);
}

}
