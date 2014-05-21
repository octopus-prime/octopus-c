/*
 * node_creator.cpp
 *
 *  Created on: 19.06.2013
 *      Author: mike_gresens
 */

#include "node.hpp"
#include "node_attacker.hpp"
#include "board_rotation.hpp"
#include <functional>
#include <boost/tokenizer.hpp>

namespace chess {

const std::string node_t::INITIAL_POSITION = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;

class position_fen_tag;
class player_fen_tag;
class castle_fen_tag;
class en_passant_fen_tag;
class half_moves_fen_tag;
class full_moves_fen_tag;

class node_t::node_creator
:
	public std::binary_function<node_t, std::string, void>
{
	template <typename fen_tag>
	class creator;

public:
	void operator()(node_t& node, const std::string& fen) const;
};

template <typename fen_tag>
class node_t::node_creator::creator
:
	public std::binary_function<node_t, std::string, void>
{
public:
	void operator()(node_t& node, const std::string& token) const;
};

template <>
class node_t::node_creator::creator<position_fen_tag>
{
private:
	piece_square_hash _hash;
	score_of_piece _score_of;

protected:
	void setRow(node_t& node, const std::string& row, const square_rank_t rank) const
	{
		square_file_t file = 0;
		for (std::string::const_iterator ch = row.begin(); ch != row.end(); ++ch, ++file)
		{
			const square_t square = square_of(file, rank);
			const board_t mask = board_of(square);
			switch (*ch)
			{
			case '8':
			case '7':
			case '6':
			case '5':
			case '4':
			case '3':
			case '2':
			case '1':
				file += (*ch - '1');
				break;
			case 'K':
				node._board_occupy_white	|= mask;
				node._square_king_white		 = square;
				node._count_pieces_white	++;
				node._pieces[square]		 = K;
				node._hash_node				^= _hash(K, square);
	//			node._scorePositional		+= Evaluator::pst[Piece::K][square];
				break;
			case 'k':
				node._board_occupy_black	|= mask;
				node._square_king_black		 = square;
				node._count_pieces_black	++;
				node._pieces[square]		 = k;
				node._hash_node				^= _hash(k, square);
	//			node._scorePositional		-= Evaluator::pst[Piece::k][square];
				break;
			case 'Q':
				node._board_occupy_white	|= mask;
				node._board_rooks_queens	|= mask;
				node._board_bishops_queens	|= mask;
				node._count_pieces_white	++;
				node._score_material		+= _score_of(Q);
	//			node._scorePositional		+= Evaluator::pst[Piece::Q][square];
				node._pieces[square]		 = Q;
				node._hash_node				^= _hash(Q, square);
				break;
			case 'q':
				node._board_occupy_black	|= mask;
				node._board_rooks_queens	|= mask;
				node._board_bishops_queens	|= mask;
				node._count_pieces_black	++;
				node._score_material		-= _score_of(q);
	//			node._scorePositional		-= Evaluator::pst[Piece::q][square];
				node._pieces[square]		 = q;
				node._hash_node				^= _hash(q, square);
				break;
			case 'R':
				node._board_occupy_white	|= mask;
				node._board_rooks_queens	|= mask;
				node._count_pieces_white	++;
				node._score_material		+= _score_of(R);
	//			node._scorePositional		+= Evaluator::pst[Piece::R][square];
				node._pieces[square]		 = R;
				node._hash_node				^= _hash(R, square);
				break;
			case 'r':
				node._board_occupy_black	|= mask;
				node._board_rooks_queens	|= mask;
				node._count_pieces_black	++;
				node._score_material		-= _score_of(r);
	//			node._scorePositional		-= Evaluator::pst[Piece::r][square];
				node._pieces[square]		 = r;
				node._hash_node				^= _hash(r, square);
				break;
			case 'B':
				node._board_occupy_white	|= mask;
				node._board_bishops_queens	|= mask;
				node._count_pieces_white	++;
				node._score_material		+= _score_of(B);
	//			node._scorePositional		+= Evaluator::pst[Piece::B][square];
				node._pieces[square]		 = B;
				node._hash_node				^= _hash(B, square);
				break;
			case 'b':
				node._board_occupy_black	|= mask;
				node._board_bishops_queens	|= mask;
				node._count_pieces_black	++;
				node._score_material		-= _score_of(b);
	//			node._scorePositional		-= Evaluator::pst[Piece::b][square];
				node._pieces[square]		 = b;
				node._hash_node				^= _hash(b, square);
				break;
			case 'N':
				node._board_occupy_white	|= mask;
				node._board_knights			|= mask;
				node._count_pieces_white	++;
				node._score_material		+= _score_of(N);
	//			node._scorePositional		+= Evaluator::pst[Piece::N][square];
				node._pieces[square]		 = N;
				node._hash_node				^= _hash(N, square);
				break;
			case 'n':
				node._board_occupy_black	|= mask;
				node._board_knights			|= mask;
				node._count_pieces_black	++;
				node._score_material		-= _score_of(n);
	//			node._scorePositional		-= Evaluator::pst[Piece::n][square];
				node._pieces[square]		 = n;
				node._hash_node				^= _hash(n, square);
				break;
			case 'P':
				node._board_occupy_white	|= mask;
				node._board_pawns			|= mask;
				node._count_pieces_white	++;
				node._score_material		+= _score_of(P);
	//			node._scorePositional		+= Evaluator::pst[Piece::P][square];
				node._pieces[square]		 = P;
				node._hash_node				^= _hash(P, square);
//				node._hash_pawn				^= _hash(P, square);
				break;
			case 'p':
				node._board_occupy_black	|= mask;
				node._board_pawns			|= mask;
				node._count_pieces_black	++;
				node._score_material		-= _score_of(p);
	//			node._scorePositional		-= Evaluator::pst[Piece::p][square];
				node._pieces[square]		 = p;
				node._hash_node				^= _hash(p, square);
//				node._hash_pawn				^= _hash(p, square);
				break;
			default:
				throw std::runtime_error("Wrong character in FEN position.");
			}
		}
		if (file != 8)
			throw std::runtime_error("Wrong file length in FEN position.");
	}

public:
	void operator()(node_t& node, const std::string& position) const
	{
		const boost::char_separator<char> separator("/");
		const Tokenizer tokenizer(position, separator);
		square_rank_t rank = 7;
		for (Tokenizer::const_iterator row = tokenizer.begin(); row != tokenizer.end(); ++row, --rank)
			setRow(node, *row, rank);
		if (rank != square_rank_t(-1))
			throw std::runtime_error("Wrong rank length in FEN position.");
	}
};

template <>
class node_t::node_creator::creator<player_fen_tag>
{
private:
	color_hash _hash;

public:
	void operator()(node_t& node, const std::string& player) const
	{
		if (player.size() != 1)
			throw std::runtime_error("Wrong length in FEN player.");
		switch (player[0])
		{
		case 'w':
			node._color = +1;
			break;
		case 'b':
			node._color = -1;
			node._hash_node ^= _hash();
			break;
		default:
			throw std::runtime_error("Wrong character in FEN player.");
		}
	}
};

template <>
class node_t::node_creator::creator<castle_fen_tag>
{
private:
	castle_hash _hash;

public:
	void operator()(node_t& node, const std::string& castles) const
	{
		if (castles == "-")
			return;
		if (castles.size() > 4)
			throw std::runtime_error("Wrong length in FEN castles.");
		for (std::string::const_iterator ch = castles.begin(); ch != castles.end(); ++ch)
			switch (*ch)
			{
			case 'K':
				node._castle |= white_castle_king;
				break;
			case 'Q':
				node._castle |= white_castle_queen;
				break;
			case 'k':
				node._castle |= black_castle_king;
				break;
			case 'q':
				node._castle |= black_castle_queen;
				break;
			default:
				throw std::runtime_error("Wrong character in FEN castles.");
			}
		node._hash_node ^= _hash(node._castle);
	}
};

template <>
class node_t::node_creator::creator<en_passant_fen_tag>
{
private:
	en_passant_hash _hash;

public:
	void operator()(node_t& node, const std::string& en_passant) const
	{
		if (en_passant == "-")
			return;
		if (en_passant.size() != 2)
			throw std::runtime_error("Wrong length in FEN en-passant.");
		const unsigned file = en_passant[0] - 'a';
		const unsigned rank = en_passant[1] - '1';
		if (file > 7 || rank < 2 || rank > 5)
			throw std::runtime_error("Wrong square in FEN en-passant.");
		const square_t square = square_of(file, rank);
		node._square_en_passant = square;
		node._hash_node ^= _hash(square);
	}
};

template <>
class node_t::node_creator::creator<half_moves_fen_tag>
{
public:
	void operator()(node_t& node, const std::string& half_moves) const
	{
		try
		{
			node._count_half_moves = std::stoi(half_moves);
		}
		catch (const std::exception&)
		{
			throw std::runtime_error("Wrong half moves in FEN.");
		}
	}
};

template <>
class node_t::node_creator::creator<full_moves_fen_tag>
{
public:
	void operator()(node_t& node, const std::string& full_moves) const
	{
		try
		{
			node._count_full_moves = std::stoi(full_moves);
			if (node._count_full_moves == 0)
				std::__throw_out_of_range("Full moves must not be 0.");
		}
		catch (const std::exception&)
		{
			throw std::runtime_error("Wrong full moves in FEN.");
		}
	}
};

void
node_t::node_creator::operator()(node_t& node, const std::string& fen) const
{
	const boost::char_separator<char> separator(" ");
	const Tokenizer tokenizer(fen, separator);
	Tokenizer::const_iterator group = tokenizer.begin();

	creator<position_fen_tag>()(node, *group++);
	creator<player_fen_tag>()(node, *group++);
	creator<castle_fen_tag>()(node, *group++);
	creator<en_passant_fen_tag>()(node, *group++);
	creator<half_moves_fen_tag>()(node, *group++);
	creator<full_moves_fen_tag>()(node, *group++);

	// Rotated boards
//	node._boardRotated90  = board_rotation<+8>()(node.board_occupy());
//	node._boardRotated45L = board_rotation<+7>()(node.board_occupy());
//	node._boardRotated45R = board_rotation<+9>()(node.board_occupy());

	// Attack boards
	const auto attacks = node_attacker()(node);
	node._board_attack_white = reinterpret_cast<const board_t* const>(&attacks)[0];
	node._board_attack_black = reinterpret_cast<const board_t* const>(&attacks)[1];
//	node._boardAttackWhite = generator::AttackGenerator().white(node);
//	node._boardAttackBlack = generator::AttackGenerator().black(node);
}

node_t::node_t(const std::string& fen)
:
	node_t()
{
	node_creator()(*this, fen);
}

}
