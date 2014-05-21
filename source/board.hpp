/*
 * board.hpp
 *
 *  Created on: 07.02.2013
 *      Author: mike_gresens
 */

#pragma once

#include "square.hpp"
#include <cstdint>
#include <map>
#include <immintrin.h>

namespace chess {

/**
 * Defines board type.
 */
typedef std::uint64_t board_t;

/**
 * Gets board of square.
 * @param square The square.
 * @return The board.
 */
constexpr board_t
board_of(const square_t square)
{
	return 1ULL << square;
}

//constexpr square_t
//board_find_square_forward(const board_t board)
//{
//	return __builtin_ctzl(board);
//}

//TODO: use __attribute__ ((target ("sse4.2"))) to specify different code for different targets
/**
 * Finds square forward.
 * @param board The board.
 * @return The square.
 */
inline square_t
board_find_square_forward(const board_t board)
{
	std::uint64_t index;
	__asm__
	(
		"bsfq %[Board], %[Index]\n\t"
		:[Index] "=r" (index)
		:[Board] "mr" (board)
	);
	return index;
}

inline square_t
board_clear_square_forward(board_t& board)
{
	const square_t square = board_find_square_forward(board);
	board ^= board_of(square);
	return square;
}

//constexpr square_t
//board_find_square_backward(const board_t board)
//{
//	return 63 - __builtin_clzl(board);
//}

/**
 * Finds square backward.
 * @param board The board.
 * @return The square.
 */
inline square_t
board_find_square_backward(const board_t board)
{
	std::uint64_t index;
	__asm__
	(
		"bsrq %[Board], %[Index]\n\t"
		:[Index] "=r" (index)
		:[Board] "mr" (board)
	);
	return index;
}

inline square_t
board_clear_square_backward(board_t& board)
{
	const square_t square = board_find_square_backward(board);
	board ^= board_of(square);
	return square;
}

constexpr bool
board_test_square(const board_t board, const square_t square)
{
	return board & board_of(square);
}

//constexpr board_t
//board_set_square(const board_t board, const square_t square)
//{
//	return board | board_of(square);
//}
//
//constexpr board_t
//board_clear_square(const board_t board, const square_t square)
//{
//	return board ^ board_of(square);
//}

//constexpr std::uint16_t
//board_count_squares(const board_t board)
//{
//	return __builtin_popcountl(board);
//}

/**
 * Counts squares.
 * @param board The board.
 * @return The squares.
 */
std::uint16_t
board_count_squares(board_t board);

__attribute__ ((target ("popcnt")))
inline std::uint16_t
board_count_squares(board_t board)
{
	std::uint64_t count;
	__asm__
	(
		"popcntq %[Board], %[Count]\n\t"
		:[Count] "=r" (count)
		:[Board] "mr" (board)
	);
	return count;
}

__attribute__ ((target ("default")))
inline std::uint16_t
board_count_squares(board_t board)
{
//	std::uint64_t count;
//	__asm__
//	(
//		"popcntq %[Board], %[Count]\n\t"
//		:[Count] "=r" (count)
//		:[Board] "mr" (board)
//	);
//	return count;

	constexpr board_t k1 = 0x5555555555555555ULL;
	constexpr board_t k2 = 0x3333333333333333ULL;
	constexpr board_t k4 = 0x0f0f0f0f0f0f0f0fULL;
	constexpr board_t kf = 0x0101010101010101ULL;

	board =  board       - ((board >> 1)  & k1);
	board = (board & k2) + ((board >> 2)  & k2);
	board = (board       +  (board >> 4)) & k4 ;
	board = (board * kf) >> 56;
	return board;
}
/*
inline std::pair<std::uint_fast16_t, std::uint_fast16_t>
board_count_squares_2(const board_t board1, const board_t board2)
{
	static const __m128i lookup = _mm_set_epi8(0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4);
	static const __m128i mask = _mm_set1_epi64x(15);

	__m128i board = _mm_set_epi64x(board1, board2);
	__m128i r1 = _mm_srli_epi16(board, 4);

	board = _mm_and_si128(board, mask);
	r1 = _mm_and_si128(r1, mask);

	board = _mm_shuffle_epi8(board, lookup);
	r1 = _mm_shuffle_epi8(r1, lookup);

	board += r1;

//	movdqa  %%xmm0, %%xmm1
//	psrlw       $4, %%xmm1
//
//	pand    %%xmm6, %%xmm0  ; xmm0 - lower nibbles
//	pand    %%xmm6, %%xmm1  ; xmm1 - higher nibbles
//
//	movdqa  %%xmm7, %%xmm2  ; since instruction pshufb modifes LUT
//	movdqa  %%xmm7, %%xmm3  ; it must be saved for further use
//
//	pshufb  %%xmm0, %%xmm2  ; xmm2 = vector of popcount for lower nibbles
//	pshufb  %%xmm1, %%xmm3  ; xmm3 = vector of popcount for higher nibbles
//
//	paddb   %%xmm3, %%xmm2  ; xmm2 += xmm3 -- vector of popcount for bytes}
	return std::make_pair<std::uint_fast16_t, std::uint_fast16_t>
	(
		reinterpret_cast<board_t*>(&board)[0],
		reinterpret_cast<board_t*>(&board)[1]
//		_mm_cvtsi128_si64(board),
//		_mm_cvtsi128_si64(board)
	);
}
*/
std::uint16_t
board_count_squares_2(board_t board);

__attribute__ ((target ("default")))
inline std::uint16_t
board_count_squares_2(board_t board)
{
	std::uint16_t count;
	for (count = 0; board; count++)
		board &= (board - 1ULL);
	return count;
}

__attribute__ ((target ("popcnt")))
inline std::uint16_t
board_count_squares_2(board_t board)
{
	return board_count_squares(board);
}

/**
 * Defines board values.
 */
enum board : board_t
{
	A1 = board_of(a1), B1 = board_of(b1), C1 = board_of(c1), D1 = board_of(d1), E1 = board_of(e1), F1 = board_of(f1), G1 = board_of(g1), H1 = board_of(h1),
	A2 = board_of(a2), B2 = board_of(b2), C2 = board_of(c2), D2 = board_of(d2), E2 = board_of(e2), F2 = board_of(f2), G2 = board_of(g2), H2 = board_of(h2),
	A3 = board_of(a3), B3 = board_of(b3), C3 = board_of(c3), D3 = board_of(d3), E3 = board_of(e3), F3 = board_of(f3), G3 = board_of(g3), H3 = board_of(h3),
	A4 = board_of(a4), B4 = board_of(b4), C4 = board_of(c4), D4 = board_of(d4), E4 = board_of(e4), F4 = board_of(f4), G4 = board_of(g4), H4 = board_of(h4),
	A5 = board_of(a5), B5 = board_of(b5), C5 = board_of(c5), D5 = board_of(d5), E5 = board_of(e5), F5 = board_of(f5), G5 = board_of(g5), H5 = board_of(h5),
	A6 = board_of(a6), B6 = board_of(b6), C6 = board_of(c6), D6 = board_of(d6), E6 = board_of(e6), F6 = board_of(f6), G6 = board_of(g6), H6 = board_of(h6),
	A7 = board_of(a7), B7 = board_of(b7), C7 = board_of(c7), D7 = board_of(d7), E7 = board_of(e7), F7 = board_of(f7), G7 = board_of(g7), H7 = board_of(h7),
	A8 = board_of(a8), B8 = board_of(b8), C8 = board_of(c8), D8 = board_of(d8), E8 = board_of(e8), F8 = board_of(f8), G8 = board_of(g8), H8 = board_of(h8)
};

/**
 * Gets rank board
 */
template <square_rank_t index>
class board_of_rank
{
public:
	constexpr board_t operator()() const
	{
		return (A1 | B1 | C1 | D1 | E1 | F1 | G1 | H1) << (index << 3);
	}

	constexpr board_t operator()(const square_rank_t index2) const
	{
		return (A1 | B1 | C1 | D1 | E1 | F1 | G1 | H1) << (index2 << 3);
	}
};

/**
 * Gets file board
 */
template <square_file_t index>
class board_of_file
{
public:
	constexpr board_t operator()() const
	{
		return (A1 | A2 | A3 | A4 | A5 | A6 | A7 | A8) << index;
	}

	constexpr board_t operator()(const square_file_t index2) const
	{
		return (A1 | A2 | A3 | A4 | A5 | A6 | A7 | A8) << index2;
	}
};

}
