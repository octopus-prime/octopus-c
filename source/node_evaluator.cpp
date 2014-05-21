/*
 * Evaluator.cpp
 *
 *  Created on: 25.01.2013
 *      Author: mike_gresens
 */

#include "node_evaluator.hpp"
#include "attack.hpp"
#include <array>
#include <emmintrin.h>

namespace chess {

class weighted_pop_count : public std::unary_function<std::array<board_t, 8>, score_t>
{
private:
	const std::array<score_t, 8>& _weights;

public:
	constexpr weighted_pop_count(const std::array<score_t, 8>& weights) noexcept
	:
		_weights(weights)
	{
	}

	score_t operator()(const std::array<board_t, 8>& boards) const
	{
		static const std::array<__m128i, 3> masks
		{{
			_mm_set1_epi8(0x55),
			_mm_set1_epi8(0x33),
			_mm_set1_epi8(0x0f)
//			{0x5555555555555555, 0x5555555555555555},
//			{0x3333333333333333, 0x3333333333333333},
//			{0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f}
		}};
		static const __m128i z = _mm_setzero_si128();

//		__m128i v = _mm_set_epi64x(boards[0], boards[1]);
//		__m128i w = _mm_set_epi64x(boards[2], boards[3]);
//		__m128i x = _mm_set_epi64x(boards[4], boards[5]);
//		__m128i y = _mm_set_epi64x(boards[6], boards[7]);
		__m128i v = reinterpret_cast<const __m128i* const>(boards.data())[0];
		__m128i w = reinterpret_cast<const __m128i* const>(boards.data())[1];
		__m128i x = reinterpret_cast<const __m128i* const>(boards.data())[2];
		__m128i y = reinterpret_cast<const __m128i* const>(boards.data())[3];

		v -= _mm_srli_epi64(v, 1) & masks[0];
		w -= _mm_srli_epi64(w, 1) & masks[0];
		x -= _mm_srli_epi64(x, 1) & masks[0];
		y -= _mm_srli_epi64(y, 1) & masks[0];

		v = (v & masks[1]) + (_mm_srli_epi64(v, 2) & masks[1]);
		w = (w & masks[1]) + (_mm_srli_epi64(w, 2) & masks[1]);
		x = (x & masks[1]) + (_mm_srli_epi64(x, 2) & masks[1]);
		y = (y & masks[1]) + (_mm_srli_epi64(y, 2) & masks[1]);

		v = (v + _mm_srli_epi64(v, 4)) & masks[2];
		w = (w + _mm_srli_epi64(w, 4)) & masks[2];
		x = (x + _mm_srli_epi64(x, 4)) & masks[2];
		y = (y + _mm_srli_epi64(y, 4)) & masks[2];

		v = _mm_packs_epi16(_mm_sad_epu8(v, z), _mm_sad_epu8(w, z));
		x = _mm_packs_epi16(_mm_sad_epu8(x, z), _mm_sad_epu8(y, z));

		v = _mm_madd_epi16 (_mm_packs_epi16(v, x), *reinterpret_cast<const __m128i* const>(_weights.data()));
		v += _mm_srli_si128(v, 4);
		v += _mm_srli_si128(v, 8);

		return _mm_cvtsi128_si32(v);
	}
};
/*
class weighted_pop_count_x : public std::unary_function<std::array<__m128i, 4>, score_t>
{
private:
	const std::array<__m128i, 4>& _weights;

public:
	constexpr weighted_pop_count_x(const std::array<__m128i, 4>& weights) noexcept
	:
		_weights(weights)
	{
	}

	score_t operator()(const std::array<board_t, 8>& boards) const
	{
		static const std::array<__m128i, 3> masks
		{{
			_mm_set1_epi8(0x55),
			_mm_set1_epi8(0x33),
			_mm_set1_epi8(0x0f)
//			{0x5555555555555555, 0x5555555555555555},
//			{0x3333333333333333, 0x3333333333333333},
//			{0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f}
		}};
		static const __m128i z = _mm_setzero_si128();

//		__m128i v = _mm_set_epi64x(boards[0], boards[1]);
//		__m128i w = _mm_set_epi64x(boards[2], boards[3]);
//		__m128i x = _mm_set_epi64x(boards[4], boards[5]);
//		__m128i y = _mm_set_epi64x(boards[6], boards[7]);
		__m128i v = boards[0];
		__m128i w = boards[1];
		__m128i x = boards[2];
		__m128i y = boards[3];

		v -= _mm_srli_epi64(v, 1) & masks[0];
		w -= _mm_srli_epi64(w, 1) & masks[0];
		x -= _mm_srli_epi64(x, 1) & masks[0];
		y -= _mm_srli_epi64(y, 1) & masks[0];

		v = (v & masks[1]) + (_mm_srli_epi64(v, 2) & masks[1]);
		w = (w & masks[1]) + (_mm_srli_epi64(w, 2) & masks[1]);
		x = (x & masks[1]) + (_mm_srli_epi64(x, 2) & masks[1]);
		y = (y & masks[1]) + (_mm_srli_epi64(y, 2) & masks[1]);

		v = (v + _mm_srli_epi64(v, 4)) & masks[2];
		w = (w + _mm_srli_epi64(w, 4)) & masks[2];
		x = (x + _mm_srli_epi64(x, 4)) & masks[2];
		y = (y + _mm_srli_epi64(y, 4)) & masks[2];

		v = _mm_packs_epi16(_mm_sad_epu8(v, z), _mm_sad_epu8(w, z));
		x = _mm_packs_epi16(_mm_sad_epu8(x, z), _mm_sad_epu8(y, z));

		v = _mm_madd_epi16 (_mm_packs_epi16(v, x), *_weights.data());
		v += _mm_srli_si128(v, 4);
		v += _mm_srli_si128(v, 8);

		return _mm_cvtsi128_si32(v);
	}
};
*/
class evaluator
:
	public std::unary_function<node_t, score_t>
{
private:
	static constexpr board_t center = D4 | E4 | D5 | E5;
	static constexpr board_t center2 = C3 | D3 | E3 | F3 | C4 | F4 | C5 | F5 | C6 | D6 | E6 | F6;
	static constexpr std::array<score_t, 8> weightsA
	{{
		+3, -3,
		+5, -5,
		+5, -5,
		+50, -50
//		+25, -25
	}};
	static constexpr std::array<score_t, 8> weightsC
	{{
		+10, -10,
		+5, -5,
		+5, -5,
		+2, -2
	}};
	static constexpr std::array<score_t, 8> weightsP
	{{
		+50, -50,
		+20, -20,
		+10, -10,
//		+25, -25
		+15, -15
	}};
//	static constexpr std::array<score_t, 8> weightsP
//	{{
//		+100, -100,
//		+50,  -50,
//		+20, -20,
//		+0, -0
//	}};

	attack_t<king_piece_tag> _kingAttack;

	static const weighted_pop_count _evalA;
	static const weighted_pop_count _evalC;
	static const weighted_pop_count _evalP;

public:
	score_t operator()(const node_t& node) const
	{
		score_t score = node.score_material();

		// Activity
		const std::array<board_t, 8> boardsA
		{{
			// Mobility
			node.board_attack_white(),
			node.board_attack_black(),
			// Attack
			node.board_attack_white() & node.board_occupy_black(),
			node.board_attack_black() & node.board_occupy_white(),
			// Defend
			node.board_attack_white() & node.board_occupy_white(),
			node.board_attack_black() & node.board_occupy_black(),
			// King
			node.board_attack_white() & (_kingAttack(node, node.square_king_black()) | node.board_king_black()),
			node.board_attack_black() & (_kingAttack(node, node.square_king_white()) | node.board_king_white())
		}};

		// Center
		const std::array<board_t, 8> boardsC
		{{
			// Occupy center
			node.board_occupy_white() & center,
			node.board_occupy_black() & center,
			// Attack center
			node.board_attack_white() & center,
			node.board_attack_black() & center,
			// Occupy center2
			node.board_occupy_white() & center2,
			node.board_occupy_black() & center2,
			// Attack center2
			node.board_occupy_white() & center2,
			node.board_occupy_black() & center2
		}};

		// Pawn
		const std::array<board_t, 8> boardsP
		{{
			// 7. rank
			node.board_pawns_white() & board_of_rank<6>()(),
			node.board_pawns_black() & board_of_rank<1>()(),
			// 6. rank
			node.board_pawns_white() & board_of_rank<5>()(),
			node.board_pawns_black() & board_of_rank<2>()(),
			// 5. rank
			node.board_pawns_white() & board_of_rank<4>()(),
			node.board_pawns_black() & board_of_rank<3>()(),
			//
			node.board_pawns_white() & _kingAttack(node, node.square_king_white()),
			node.board_pawns_black() & _kingAttack(node, node.square_king_black())
//			node.boardAttackWhite() & _kingAttack.white(node),
//			node.boardAttackBlack() & _kingAttack.black(node)
		}};

		score += _evalA(boardsA);
		score += _evalC(boardsC);
		score += _evalP(boardsP);

//		score += (node.canCastleWhiteKing() || node.canCastleWhiteQueen()) * -50;
//		score += (node.canCastleBlackKing() || node.canCastleBlackQueen()) * +50;

		return score * node.color();
	}
};

constexpr std::array<score_t, 8> evaluator::weightsA;
constexpr std::array<score_t, 8> evaluator::weightsC;
constexpr std::array<score_t, 8> evaluator::weightsP;

const weighted_pop_count evaluator::_evalA(evaluator::weightsA);
const weighted_pop_count evaluator::_evalC(evaluator::weightsC);
const weighted_pop_count evaluator::_evalP(evaluator::weightsP);

class knight_evaluator
{
private:
//	attack_t<king_piece_tag> king_attack;
	attack_t<knight_piece_tag> king_attack;
	attack_t<knight_piece_tag> knight_attack;

public:
	score_t operator()(const node_t& node) const
	{
		score_t score = 0;

		const board_t black_king = king_attack(node, node.square_king_black());
		board_t white_knights = node.board_knights_white();
		while (white_knights)
		{
			const square_t white_knight = board_clear_square_forward(white_knights);
			score += static_cast<bool>(knight_attack(node, white_knight) & black_king);
		}

		const board_t white_king = king_attack(node, node.square_king_white());
		board_t black_knights = node.board_knights_black();
		while (black_knights)
		{
			const square_t black_knight = board_clear_square_forward(black_knights);
			score -= static_cast<bool>(knight_attack(node, black_knight) & white_king);
		}

		return score * 25;
	}
};

class pawn_evaluator
{
private:
public:
	score_t operator()(const node_t& node) const
	{
		constexpr std::array<score_t, 8> foo
		{{
			0, 0, -50, -100, -150, -200, -250, -300
		}};

		constexpr board_of_file<0> board_of_file;

		score_t score = 0;

		for (square_file_t file = 0; file < 8; ++file)
		{
			score += foo[board_count_squares_2(node.board_pawns_white() & board_of_file(file))];
			score -= foo[board_count_squares_2(node.board_pawns_black() & board_of_file(file))];
		}
/*
		score += foo[board_count_squares_2(node.board_pawns_white() & board_of_file<0>()())];
		score += foo[board_count_squares_2(node.board_pawns_white() & board_of_file<1>()())];
		score += foo[board_count_squares_2(node.board_pawns_white() & board_of_file<2>()())];
		score += foo[board_count_squares_2(node.board_pawns_white() & board_of_file<3>()())];
		score += foo[board_count_squares_2(node.board_pawns_white() & board_of_file<4>()())];
		score += foo[board_count_squares_2(node.board_pawns_white() & board_of_file<5>()())];
		score += foo[board_count_squares_2(node.board_pawns_white() & board_of_file<6>()())];
		score += foo[board_count_squares_2(node.board_pawns_white() & board_of_file<7>()())];

		score -= foo[board_count_squares_2(node.board_pawns_black() & board_of_file<0>()())];
		score -= foo[board_count_squares_2(node.board_pawns_black() & board_of_file<1>()())];
		score -= foo[board_count_squares_2(node.board_pawns_black() & board_of_file<2>()())];
		score -= foo[board_count_squares_2(node.board_pawns_black() & board_of_file<3>()())];
		score -= foo[board_count_squares_2(node.board_pawns_black() & board_of_file<4>()())];
		score -= foo[board_count_squares_2(node.board_pawns_black() & board_of_file<5>()())];
		score -= foo[board_count_squares_2(node.board_pawns_black() & board_of_file<6>()())];
		score -= foo[board_count_squares_2(node.board_pawns_black() & board_of_file<7>()())];
*/
		return score;
	}
};

class bishop_evaluator
{
private:
public:
	score_t operator()(const node_t& node) const
	{
		score_t score = 0;

//		const auto foo = board_count_squares_2(node.board_bishops_white(), node.board_bishops_black());
//		const score_t score = foo.first + foo.second;
//		score += foo.first;
//		score -= foo.second;

		score += board_count_squares_2(node.board_bishops_white()) >= 2;
		score -= board_count_squares_2(node.board_bishops_black()) >= 2;

		return score * 25;
	}
};

class rook_evaluator
{
private:
public:
	score_t operator()(const node_t& node) const
	{
		constexpr board_of_file<0> board_of_file;
		constexpr board_of_rank<0> board_of_rank;

		score_t score = 0;

		const board_t b = board_of_file(file_of(node.square_king_black())) | board_of_rank(rank_of(node.square_king_black()));
		score += board_count_squares_2(b & node.board_rooks_queens_white());

		const board_t w = board_of_file(file_of(node.square_king_white())) | board_of_rank(rank_of(node.square_king_white()));
		score -= board_count_squares_2(w & node.board_rooks_queens_black());

		return score * 10;
	}
};

class king_evaluator
{
private:
public:
	score_t operator()(const node_t& node) const
	{
		score_t score = 0;

		score += node.check(white);
		score -= node.check(black);

		return score * -25;// -50;
	}
};

score_t
node_evaluator::operator()(const node_t& node) const
{
	return evaluator()(node) + pawn_evaluator()(node) + knight_evaluator()(node) + bishop_evaluator()(node) + rook_evaluator()(node);// + king_evaluator()(node);
}

}
