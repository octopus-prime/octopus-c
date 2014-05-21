/*
 * attack.cpp
 *
 *  Created on: 21.06.2013
 *      Author: mike_gresens
 */

#include "attack.hpp"
#include "board_rotation.hpp"

namespace chess {

template <std::int8_t delta>
class slide
:
	public std::unary_function<square_t, board_t>
{
public:
	board_t operator()(square_t square) const;
};

template <>
class slide<+1>
{
public:
	board_t operator()(square_t square) const
	{
		board_t board = 0ULL;
		for (;;)
		{
			if (file_of(square) == 7)
				break;
			square += 1;
			board |= board_of(square);
		}
		return board;
	}
};

template <>
class slide<-1>
{
public:
	board_t operator()(square_t square) const
	{
		board_t board = 0ULL;
		for (;;)
		{
			if (file_of(square) == 0)
				break;
			square -= 1;
			board |= board_of(square);
		}
		return board;
	}
};

template <>
class slide<+8>
{
public:
	board_t operator()(square_t square) const
	{
		board_t board = 0ULL;
		for (;;)
		{
			if (rank_of(square) == 7)
				break;
			square += 8;
			board |= board_of(square);
		}
		return board;
	}
};

template <>
class slide<-8>
{
public:
	board_t operator()(square_t square) const
	{
		board_t board = 0ULL;
		for (;;)
		{
			if (rank_of(square) == 0)
				break;
			square -= 8;
			board |= board_of(square);
		}
		return board;
	}
};

template <>
class slide<+7>
{
public:
	board_t operator()(square_t square) const
	{
		board_t board = 0ULL;
		for (;;)
		{
			if (file_of(square) == 0 || rank_of(square) == 7)
				break;
			square += 7;
			board |= board_of(square);
		}
		return board;
	}
};

template <>
class slide<-7>
{
public:
	board_t operator()(square_t square) const
	{
		board_t board = 0ULL;
		for (;;)
		{
			if (file_of(square) == 7 || rank_of(square) == 0)
				break;
			square -= 7;
			board |= board_of(square);
		}
		return board;
	}
};

template <>
class slide<+9>
{
public:
	board_t operator()(square_t square) const
	{
		board_t board = 0ULL;
		for (;;)
		{
			if (file_of(square) == 7 || rank_of(square) == 7)
				break;
			square += 9;
			board |= board_of(square);
		}
		return board;
	}
};

template <>
class slide<-9>
{
public:
	board_t operator()(square_t square) const
	{
		board_t board = 0ULL;
		for (;;)
		{
			if (file_of(square) == 0 || rank_of(square) == 0)
				break;
			square -= 9;
			board |= board_of(square);
		}
		return board;
	}
};

template <std::int8_t delta>
class rook_creator
{
private:
	slide<-delta> _mstep;
	slide<+delta> _pstep;
	board_rotation<-delta> _unrotate;

public:
	std::array<std::array<board_t, 64>, 64> operator()(const std::array<std::uint8_t, 64>& shift) const
	{
		std::array<std::array<board_t, 64>, 64> attacks;
		for (square_t square = a1; square <= h8; ++square)
			for (board_t occupy = 0; occupy < 64; ++occupy)
			{
				const board_t b = _unrotate(((occupy << 1) | 129UL) << (shift[square] - 1));
				const board_t m = b & _mstep(square);
				const board_t d = m ? _mstep(square) & ~_mstep(board_find_square_backward(m)) : 0ULL;
				const board_t p = b & _pstep(square);
				const board_t u = p ? _pstep(square) & ~_pstep(board_find_square_forward(p)) : 0ULL;
				attacks[square][occupy] = d | u;
			}
		return attacks;
	}
};

template <std::int8_t delta>
class bishop_creator
{
private:
	slide<-delta> _mstep;
	slide<+delta> _pstep;
	board_rotation<-delta> _unrotate;

public:
	std::array<std::array<board_t, 64>, 64> operator()(const std::array<std::uint8_t, 64>& shift, const std::array<std::uint8_t, 64>& length) const
	{
		std::array<std::array<board_t, 64>, 64> attacks;
		for (square_t square = a1; square <= h8; ++square)
			for (board_t occupy = 0; occupy < 64; ++occupy)
			{
				const board_t s = 1ULL | board_of(length[square] - 1);
				const board_t b = _unrotate(((occupy << 1) | s) << (shift[square] - 1));
				const board_t m = b & _mstep(square);
				const board_t d = m ? _mstep(square) & ~_mstep(board_find_square_backward(m)) : 0ULL;
				const board_t p = b & _pstep(square);
				const board_t u = p ? _pstep(square) & ~_pstep(board_find_square_forward(p)) : 0ULL;
				attacks[square][occupy] = d | u;
			}
		return attacks;
	}
};

class stepping_attack_creator
:
	public std::unary_function<std::array<std::array<std::int8_t, 2>, 8>, std::array<board_t, 64>>
{
public:
	std::array<board_t, 64> operator()(const std::array<std::array<std::int8_t, 2>, 8>& deltas) const
	{
		std::array<board_t, 64> attacks;
		for (square_t fromSquare = a1; fromSquare <= h8; ++fromSquare)
		{
			const square_file_t fromFile = file_of(fromSquare);
			const square_rank_t fromRank = rank_of(fromSquare);
			board_t board = 0ULL;
			for (const std::array<std::int8_t, 2>& delta : deltas)
			{
				const square_file_t toFile = fromFile + delta[0];
				const square_rank_t toRank = fromRank + delta[1];
				if (toFile >= 0 && toFile < 8 && toRank >= 0 && toRank < 8)
				{
					const square_t toSquare = square_of(toFile, toRank);
					board |= board_of(toSquare);
				}
			}
			attacks[fromSquare] = board;
		}
		return attacks;
	}
};

static std::array<board_t, 64>
create_king_attacks()
{
	static constexpr std::array<std::array<std::int8_t, 2>, 8> deltas
	{{
		{{-1, -1}}, {{-1,  0}}, {{-1, +1}},
		{{ 0, -1}}, /*From*/    {{ 0, +1}},
		{{+1, -1}}, {{+1,  0}}, {{+1, +1}}
	}};

	return stepping_attack_creator()(deltas);
}

static std::array<std::array<board_t, 64>, 64>
create_rook_attacks_1()
{
	static constexpr std::array<std::uint8_t, 64> shift
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

	return rook_creator<1>()(shift);
}

static std::array<board_t, 64>
create_rook_attacks()
{
	std::array<board_t, 64> attacks;

	for (square_t s = 0; s < 64; ++s)
		attacks[s] = slide<+8>()(s) | slide<-8>()(s);

	return attacks;
}
/*
static std::array<std::array<board_t, 64>, 64>
create_rook_attacks_8()
{
	static constexpr std::array<std::uint8_t, 64> shift
	{{
		 1,  9, 17, 25, 33, 41, 49, 57,
		 1,  9, 17, 25, 33, 41, 49, 57,
		 1,  9, 17, 25, 33, 41, 49, 57,
		 1,  9, 17, 25, 33, 41, 49, 57,
		 1,  9, 17, 25, 33, 41, 49, 57,
		 1,  9, 17, 25, 33, 41, 49, 57,
		 1,  9, 17, 25, 33, 41, 49, 57,
		 1,  9, 17, 25, 33, 41, 49, 57
	}};

	return rook_creator<8>()(shift);
}
*/
/*
static std::array<std::array<board_t, 64>, 64>
create_bishop_attacks_7()
{
	constexpr std::array<std::uint8_t, 64> shift
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

	constexpr std::array<std::uint8_t, 64> length
	{{
		 1,  2,  3,  4,  5,  6,  7,  8,
		 2,  3,  4,  5,  6,  7,  8,  7,
		 3,  4,  5,  6,  7,  8,  7,  6,
		 4,  5,  6,  7,  8,  7,  6,  5,
		 5,  6,  7,  8,  7,  6,  5,  4,
		 6,  7,  8,  7,  6,  5,  4,  3,
		 7,  8,  7,  6,  5,  4,  3,  2,
		 8,  7,  6,  5,  4,  3,  2,  1
	}};

	return bishop_creator<7>()(shift, length);
}

static std::array<std::array<board_t, 64>, 64>
create_bishop_attacks_9()
{
	constexpr std::array<std::uint8_t, 64> shift
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

	constexpr std::array<std::uint8_t, 64> length
	{{
		 8,  7,  6,  5,  4,  3,  2,  1,
		 7,  8,  7,  6,  5,  4,  3,  2,
		 6,  7,  8,  7,  6,  5,  4,  3,
		 5,  6,  7,  8,  7,  6,  5,  4,
		 4,  5,  6,  7,  8,  7,  6,  5,
		 3,  4,  5,  6,  7,  8,  7,  6,
		 2,  3,  4,  5,  6,  7,  8,  7,
		 1,  2,  3,  4,  5,  6,  7,  8
	}};

	return bishop_creator<9>()(shift, length);
}
*/
static std::array<board_t, 64>
create_knight_attacks()
{
	static 	constexpr std::array<std::array<std::int8_t, 2>, 8> deltas
	{{
					{{-2, -1}},		{{-2, +1}},
		{{-1, -2}},								{{-1, +2}},
							//From
		{{+1, -2}},								{{+1, +2}},
					{{+2, -1}},		{{+2, +1}}
	}};

	return stepping_attack_creator()(deltas);
}

static std::array<__m128i, 64>
create_bishop_attacks()
{
	std::array<__m128i, 64> attacks;

	for (square_t s = 0; s < 64; ++s)
	{
		const board_t d = slide<+9>()(s) | slide<-7>()(s);
		const board_t a = slide<+7>()(s) | slide<-9>()(s);
		attacks[s] = _mm_set_epi64x(d, a);
	}

	return attacks;
}

const std::array<board_t, 64> attack_t<king_piece_tag>::attacks(create_king_attacks());
constexpr std::array<std::uint8_t, 64> attack_t<rook_queen_piece_tag>::shift1;
//constexpr std::array<std::uint8_t, 64> attack_t<rook_queen_piece_tag>::shift8;
const std::array<std::array<board_t, 64>, 64> attack_t<rook_queen_piece_tag>::_attacks1(create_rook_attacks_1());
//const std::array<std::array<board_t, 64>, 64> attack_t<rook_queen_piece_tag>::_attacks8(create_rook_attacks_8());
const std::array<board_t, 64> attack_t<rook_queen_piece_tag>::_attacks(create_rook_attacks());
const std::array<__m128i, 64> attack_t<bishop_queen_piece_tag>::_attacks(create_bishop_attacks());
const __m128i attack_t<bishop_queen_piece_tag>::_mask(_mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));
//constexpr std::array<std::uint8_t, 64> attack_t<bishop_queen_piece_tag>::shift7;
//constexpr std::array<std::uint8_t, 64> attack_t<bishop_queen_piece_tag>::shift9;
//constexpr std::array<std::uint8_t, 64> attack_t<bishop_queen_piece_tag>::mask7;
//constexpr std::array<std::uint8_t, 64> attack_t<bishop_queen_piece_tag>::mask9;
//const std::array<std::array<board_t, 64>, 64> attack_t<bishop_queen_piece_tag>::_attacks7(create_bishop_attacks_7());
//const std::array<std::array<board_t, 64>, 64> attack_t<bishop_queen_piece_tag>::_attacks9(create_bishop_attacks_9());
const std::array<board_t, 64> attack_t<knight_piece_tag>::attacks(create_knight_attacks());

}
