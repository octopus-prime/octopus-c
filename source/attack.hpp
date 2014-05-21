/*
 * attack.hpp
 *
 *  Created on: 21.06.2013
 *      Author: mike_gresens
 */

#pragma once

#include "node.hpp"
#include <immintrin.h>

namespace chess {

template <typename piece_tag>
class attack_t
:
	public std::binary_function<node_t, square_t, board_t>
{
public:
	board_t operator()(const node_t& node, const square_t square) const;
};

template <>
class attack_t<king_piece_tag>
{
private:
	static const std::array<board_t, 64> attacks;

public:
	constexpr board_t operator()(const node_t& node, const square_t square) const
	{
		return attacks[square];
	}
};

//template <>
//class attack<rook_queen_piece_tag>
//{
//public:
//	constexpr board_t operator()(const node_t& node, const square_t square) const
//	{
//		return 0;
//	}
//};

template <>
class attack_t<rook_queen_piece_tag>
{
private:
	static constexpr std::array<std::uint8_t, 64> shift1
	{{
		 1,  1,  1,  1,  1,  1,  1,  1,
		 9,  9,  9,  9,  9,  9,  9,  9,
		17, 17, 17, 17, 17, 17, 17, 17,
		25, 25, 25, 25, 25, 25, 25, 25,
		33, 33, 33, 33, 33, 33, 33, 33,
		41, 41, 41, 41, 41, 41, 41, 41,
		49, 49, 49, 49, 49, 49, 49, 49,
		57, 57, 57, 57, 57, 57, 57, 57
	}};

//	static constexpr std::array<std::uint8_t, 64> shift8
//	{{
//		 1,  9, 17, 25, 33, 41, 49, 57,
//		 1,  9, 17, 25, 33, 41, 49, 57,
//		 1,  9, 17, 25, 33, 41, 49, 57,
//		 1,  9, 17, 25, 33, 41, 49, 57,
//		 1,  9, 17, 25, 33, 41, 49, 57,
//		 1,  9, 17, 25, 33, 41, 49, 57,
//		 1,  9, 17, 25, 33, 41, 49, 57,
//		 1,  9, 17, 25, 33, 41, 49, 57
//	}};

	static const std::array<std::array<board_t, 64>, 64> _attacks1;
//	static const std::array<std::array<board_t, 64>, 64> _attacks8;

	static const std::array<board_t, 64> _attacks;

protected:
	board_t fileAttacks(const board_t occ, const square_t sq) const
	{
		const board_t mask = board_of(sq);
		board_t forward, reverse;
	   forward  = occ & _attacks[sq];
	   reverse  = __builtin_bswap64(forward);
	   forward -= mask;
	   reverse -= __builtin_bswap64(mask);
	   forward ^= __builtin_bswap64(reverse);
	   forward &= _attacks[sq];
	   return forward;
	}

	constexpr board_t attacks1(const node_t& node, const square_t square) const
	{
		return _attacks1[square][(node.board_occupy() >> shift1[square]) & 63];
	}

//	constexpr board_t attacks8(const node_t& node, const square_t square) const
//	{
//		return _attacks8[square][(node.boardRotated90() >> shift8[square]) & 63];
//	}

public:
	//constexpr
	board_t operator()(const node_t& node, const square_t square) const
	{
//		return attacks1(node, square) | attacks8(node, square);
		return attacks1(node, square) | fileAttacks(node.board_occupy(), square);
	}
};

class bishop_queen_piece_tag2;

template <>
class attack_t<bishop_queen_piece_tag>
{
private:
	static const std::array<__m128i, 64> _attacks;
	static const __m128i _mask;

protected:
//public:
	board_t attacks(const board_t occupy, const square_t square) const
	{
		const __m128i m = _attacks[square];
		__m128i b = _mm_set1_epi64x(board_of(square));
		__m128i o = _mm_set1_epi64x(occupy);
		o = _mm_and_si128       (o, m); // o (antidiag : diagonal)
		__m128i r = _mm_shuffle_epi8    (o, _mask); // o'(antidiag : diagonal)
		o = _mm_sub_epi64       (o, b); // o - bishop
		b = _mm_shuffle_epi8    (b, _mask); // bishop', one may also use singleBitsXMM [sq^56]
		r = _mm_sub_epi64       (r, b); // o'- bishop'
		r = _mm_shuffle_epi8    (r, _mask); // re-reverse
		o = _mm_xor_si128       (o, r); // attacks
		o = _mm_and_si128       (o, m); // antidiag : diagonal
		r = _mm_unpackhi_epi64  (o, o); // antidiag : antidiag
		o = _mm_add_epi64       (o, r); // diagonal + antidiag
		return _mm_cvtsi128_si64(o);    // convert xmm to general purpose 64 bit
	}

public:
	board_t operator()(const node_t& node, const square_t square) const
	{
		return attacks(node.board_occupy(), square);
	}
};

/*
template <>
class attack_t<bishop_queen_piece_tag>
{
private:
	static constexpr std::array<std::uint8_t, 64> shift7
	{{
		 1,  2,  4,  7, 11, 16, 22, 29,
		 2,  4,  7, 11, 16, 22, 29, 37,
		 4,  7, 11, 16, 22, 29, 37, 44,
		 7, 11, 16, 22, 29, 37, 44, 50,
		11, 16, 22, 29, 37, 44, 50, 55,
		16, 22, 29, 37, 44, 50, 55, 59,
		22, 29, 37, 44, 50, 55, 59, 62,
		29, 37, 44, 50, 55, 59, 62, 64
	}};

	static constexpr std::array<std::uint8_t, 64> shift9
	{{
		29, 22, 16, 11,  7,  4,  2,  1,
		37, 29, 22, 16, 11,  7,  4,  2,
		44, 37, 29, 22, 16, 11,  7,  4,
		50, 44, 37, 29, 22, 16, 11,  7,
		55, 50, 44, 37, 29, 22, 16, 11,
		59, 55, 50, 44, 37, 29, 22, 16,
		62, 59, 55, 50, 44, 37, 29, 22,
		64, 62, 59, 55, 50, 44, 37, 29
	}};

	static constexpr std::array<std::uint8_t, 64> mask7
	{{
		 0,  0,  1,  3,  7, 15, 31, 63,
		 0,  1,  3,  7, 15, 31, 63, 31,
		 1,  3,  7, 15, 31, 63, 31, 15,
		 3,  7, 15, 31, 63, 31, 15,  7,
		 7, 15, 31, 63, 31, 15,  7,  3,
		15, 31, 63, 31, 15,  7,  3,  1,
		31, 63, 31, 15,  7,  3,  1,  0,
		63, 31, 15,  7,  3,  1,  0,  0
	}};

	static constexpr std::array<std::uint8_t, 64> mask9
	{{
		63, 31, 15,  7,  3,  1,  0,  0,
		31, 63, 31, 15,  7,  3,  1,  0,
		15, 31, 63, 31, 15,  7,  3,  1,
		 7, 15, 31, 63, 31, 15,  7,  3,
		 3,  7, 15, 31, 63, 31, 15,  7,
		 1,  3,  7, 15, 31, 63, 31, 15,
		 0,  1,  3,  7, 15, 31, 63, 31,
		 0,  0,  1,  3,  7, 15, 31, 63
	}};

	static const std::array<std::array<board_t, 64>, 64> _attacks7;
	static const std::array<std::array<board_t, 64>, 64> _attacks9;

protected:
	constexpr board_t attacks7(const node_t& node, const square_t square) const
	{
		return _attacks7[square][(node.boardRotated45L() >> shift7[square]) & mask7[square]];
	}

	constexpr board_t attacks9(const node_t& node, const square_t square) const
	{
		return _attacks9[square][(node.boardRotated45R() >> shift9[square]) & mask9[square]];
	}

public:
	constexpr board_t operator()(const node_t& node, const square_t square) const
	{
		return attacks7(node, square) | attacks9(node, square);
	}
};
*/
template <>
class attack_t<knight_piece_tag>
{
private:
	static const std::array<board_t, 64> attacks;

public:
	constexpr board_t operator()(const node_t& node, const square_t square) const
	{
		return attacks[square];
	}
};

template <>
class attack_t<pawn_piece_tag>
{
public:
	constexpr board_t operator()(const node_t& node, const square_t square) const
	{
		return 0;
	}
};

}
