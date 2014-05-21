/*
 * io.cpp
 *
 *  Created on: 26.01.2013
 *      Author: mike_gresens
 */

#include "io.hpp"
#include <array>

namespace chess {

std::ostream&
operator<<(std::ostream& stream, const std::pair<const node_t&, const move_t&>& pair)
{
	static constexpr std::array<const char* const, 13> piece
	{{
		"",
		"K", "Q", "R", "B", "N", "P",
		"k", "q", "r", "b", "n", "p"
	}};
	static constexpr std::array<const char* const, 5> promoted
	{{
		"", "N", "B", "R", "Q"
	}};
	static constexpr std::array<const char* const, 2> check
	{{
		"", "+"
	}};
	stream << piece[pair.first[pair.second.from()]];
	stream << char('a' + file_of(pair.second.from()));
	stream << char('1' + rank_of(pair.second.from()));
	stream << (pair.first[pair.second.to()] ? 'x' : '-');
	stream << char('a' + file_of(pair.second.to()));
	stream << char('1' + rank_of(pair.second.to()));
	stream << promoted[pair.second.promoted()];
	stream << check[pair.second.check()];
	return stream;
}

std::ostream&
operator<<(std::ostream& stream, const node_t& node)
{
	static constexpr std::array<char, 13> pieces
	{{
		' ',
		'K', 'Q', 'R', 'B', 'N', 'P',
		'k', 'q', 'r', 'b', 'n', 'p'
	}};
	stream << std::endl << " abcdefgh" << std::endl;
	for (square_rank_t rank = 8; rank > 0; --rank)
	{
		stream << short(rank);
		for (square_file_t file = 0; file < 8; ++file)
		{
			const square_t square = square_of(file, rank - 1);
			const piece_t piece = node[square];
			stream << pieces[piece];
		}
		stream << short(rank) << std::endl;
	}
	stream << " abcdefgh" << std::endl;
	stream << "Color = " << node.color() << std::endl;
	stream << "EnPassant = ";
	stream << char('a' + file_of(node.square_en_passant()));
	stream << char('1' + rank_of(node.square_en_passant()));
	stream << std::endl;
	return stream;
}

std::ostream&
operator<<(std::ostream& stream, const std::bitset<64>& board)
{
	static constexpr std::array<char, 2> pieces
	{{
		' ', '*'
	}};
	stream << std::endl << " abcdefgh" << std::endl;
	for (square_rank_t rank = 8; rank > 0; --rank)
	{
		stream << short(rank);
		for (square_file_t file = 0; file < 8; ++file)
		{
			const square_t square = square_of(file, rank - 1);
			const bool piece = board[square];
			stream << pieces[piece];
		}
		stream << short(rank) << std::endl;
	}
	stream << " abcdefgh" << std::endl;
	return stream;
}

}
