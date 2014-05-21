/*
 * node.hpp
 *
 *  Created on: 08.02.2013
 *      Author: mike_gresens
 */

#pragma once

#include "square.hpp"
#include "board.hpp"
#include "hash.hpp"
#include "score.hpp"
#include "piece.hpp"
#include "castle.hpp"
#include "color.hpp"
#include "move.hpp"
#include "tag.hpp"
#include <array>
#include <string>

namespace chess {

class node_t
{
	class node_creator;

	class node_basic_executor;

	template <typename moves_tag>
	class node_executor;

private:
	const node_t*			_parent;
	board_t					_board_attack_white;
	board_t					_board_attack_black;
	board_t					_board_occupy_white;
	board_t					_board_occupy_black;
	board_t					_board_rooks_queens;
	board_t					_board_bishops_queens;
	board_t					_board_knights;
	board_t					_board_pawns;
//	board_t					_boardRotated90;
//	board_t					_boardRotated45L;
//	board_t					_boardRotated45R;
	hash_t					_hash_node;
//	hash_t					_hash_pawn;
	score_t					_score_material;
//	score_t					_scorePositional;
	color_t					_color;
	std::array<piece_t, 64>	_pieces;
	square_t				_square_king_white;
	square_t				_square_king_black;
	square_t				_square_en_passant;
	castle_t				_castle;
	std::uint8_t			_count_pieces_white;
	std::uint8_t			_count_pieces_black;
	std::uint8_t			_count_half_moves;
	std::uint8_t			_count_full_moves;

//protected:
public:
	constexpr node_t()
	:
		_parent(nullptr),
		_board_attack_white(),
		_board_attack_black(),
		_board_occupy_white(),
		_board_occupy_black(),
		_board_rooks_queens(),
		_board_bishops_queens(),
		_board_knights(),
		_board_pawns(),
//		_boardRotated90(),
//		_boardRotated45L(),
//		_boardRotated45R(),
		_hash_node(),
//		_hash_pawn(),
		_score_material(),
//		_scorePositional(),
		_color(),
		_pieces(),
		_square_king_white(),
		_square_king_black(),
		_square_en_passant(),
		_castle(),
		_count_pieces_white(),
		_count_pieces_black(),
		_count_half_moves(),
		_count_full_moves()
	{
//		__builtin_memset(this + sizeof(nullptr), 0, sizeof(node));
	}

public:
	constexpr node_t(const node_t& node)
	:
		_parent(&node),
		_board_attack_white(node._board_attack_white),
		_board_attack_black(node._board_attack_black),
		_board_occupy_white(node._board_occupy_white),
		_board_occupy_black(node._board_occupy_black),
		_board_rooks_queens(node._board_rooks_queens),
		_board_bishops_queens(node._board_bishops_queens),
		_board_knights(node._board_knights),
		_board_pawns(node._board_pawns),
//		_boardRotated90(node._boardRotated90),
//		_boardRotated45L(node._boardRotated45L),
//		_boardRotated45R(node._boardRotated45R),
		_hash_node(node._hash_node),
//		_hash_pawn(node._hash_pawn),
		_score_material(node._score_material),
//		_scorePositional(),
		_color(node._color),
		_pieces(node._pieces),
		_square_king_white(node._square_king_white),
		_square_king_black(node._square_king_black),
		_square_en_passant(node._square_en_passant),
		_castle(node._castle),
		_count_pieces_white(node._count_pieces_white),
		_count_pieces_black(node._count_pieces_black),
		_count_half_moves(node._count_half_moves),
		_count_full_moves(node._count_full_moves)
	{
//		__builtin_memset(this + sizeof(nullptr), 0, sizeof(node));
	}

public:
	static const std::string INITIAL_POSITION;

	explicit node_t(const std::string& fen);
	explicit node_t(const node_t& node, const move_t move, const active_moves_tag);
	explicit node_t(const node_t& node, const move_t move, const passive_moves_tag);

	constexpr const node_t* parent() const
	{
		return _parent;
	}

	constexpr board_t board_occupy_white() const
	{
		return _board_occupy_white;
	}

	constexpr board_t board_occupy_black() const
	{
		return _board_occupy_black;
	}

	constexpr board_t board_kings() const
	{
		return board_king_white() | board_king_black();
	}

	constexpr board_t board_king_white() const
	{
		return board_of(square_king_white());
	}

	constexpr board_t board_king_black() const
	{
		return board_of(square_king_black());
	}

	constexpr board_t board_queens() const
	{
		return _board_rooks_queens & _board_bishops_queens;
	}

	constexpr board_t board_queens_white() const
	{
		return board_queens() & board_occupy_white();
	}

	constexpr board_t board_queens_black() const
	{
		return board_queens() & board_occupy_black();
	}

	constexpr board_t board_rooks_queens() const
	{
		return _board_rooks_queens;
	}

	constexpr board_t board_rooks_queens_white() const
	{
		return board_rooks_queens() & board_occupy_white();
	}

	constexpr board_t board_rooks_queens_black() const
	{
		return board_rooks_queens() & board_occupy_black();
	}

	constexpr board_t board_rooks() const
	{
		return board_rooks_queens() & ~board_bishops_queens();
	}

	constexpr board_t board_rooks_white() const
	{
		return board_rooks() & board_occupy_white();
	}

	constexpr board_t board_rooks_black() const
	{
		return board_rooks() & board_occupy_black();
	}

	constexpr board_t board_bishops_queens() const
	{
		return _board_bishops_queens;
	}

	constexpr board_t board_bishops_queens_white() const
	{
		return board_bishops_queens() & board_occupy_white();
	}

	constexpr board_t board_bishops_queens_black() const
	{
		return board_bishops_queens() & board_occupy_black();
	}

	constexpr board_t board_bishops() const
	{
		return board_bishops_queens() & ~board_rooks_queens();
	}

	constexpr board_t board_bishops_white() const
	{
		return board_bishops() & board_occupy_white();
	}

	constexpr board_t board_bishops_black() const
	{
		return board_bishops() & board_occupy_black();
	}

	constexpr board_t board_knights() const
	{
		return _board_knights;
	}

	constexpr board_t board_knights_white() const
	{
		return board_knights() & board_occupy_white();
	}

	constexpr board_t board_knights_black() const
	{
		return board_knights() & board_occupy_black();
	}

	constexpr board_t board_pawns() const
	{
		return _board_pawns;
	}

	constexpr board_t board_pawns_white() const
	{
		return board_pawns() & board_occupy_white();
	}

	constexpr board_t board_pawns_black() const
	{
		return board_pawns() & board_occupy_black();
	}

	constexpr board_t boardNBR() const
	{
		return board_knights() | board_bishops_queens() | board_rooks_queens();
	}

	constexpr board_t boardNBRWhite() const
	{
		return boardNBR() & board_occupy_white();
	}

	constexpr board_t boardNBRBlack() const
	{
		return boardNBR() & board_occupy_black();
	}

	constexpr board_t board_occupy() const
	{
		return board_occupy_white() | board_occupy_black();
	}

//	constexpr board_t boardRotated90() const
//	{
//		return _boardRotated90;
//	}

//	constexpr board_t boardRotated45L() const
//	{
//		return _boardRotated45L;
//	}
//
//	constexpr board_t boardRotated45R() const
//	{
//		return _boardRotated45R;
//	}

	constexpr board_t board_attack_white() const
	{
		return _board_attack_white;
	}

	constexpr board_t board_attack_black() const
	{
		return _board_attack_black;
	}

	constexpr board_t board_attack() const
	{
		return board_attack_white() | board_attack_black();
	}
/*
	inline board_t operator()(const piece_t piece) const
	{
		static const std::array<std::function<board_t ()>, 13> _functions
		{{
			[](){return 0ULL;},

			std::bind(&Node::boardKingWhite, this),
			std::bind(&Node::boardQueensWhite, this),
			std::bind(&Node::boardRooksWhite, this),
			std::bind(&Node::boardBishopsWhite, this),
			std::bind(&Node::boardKnightsWhite, this),
			std::bind(&Node::boardPawnsWhite, this),

			std::bind(&Node::boardKingBlack, this),
			std::bind(&Node::boardQueensBlack, this),
			std::bind(&Node::boardRooksBlack, this),
			std::bind(&Node::boardBishopsBlack, this),
			std::bind(&Node::boardKnightsBlack, this),
			std::bind(&Node::boardPawnsBlack, this)
		}};
		return _functions[piece]();
	}
*/
	constexpr square_t square_king_white() const
	{
		return _square_king_white;
	}

	constexpr square_t square_king_black() const
	{
		return _square_king_black;
	}

	constexpr square_t square_en_passant() const
	{
		return _square_en_passant;
	}

	constexpr score_t score_material() const
	{
		return _score_material;
	}

	constexpr piece_t operator[](const square_t square) const
	{
		return _pieces[square];
	}

	constexpr color_t color() const
	{
		return _color;
	}

	constexpr hash_t hash_node() const
	{
		return _hash_node;
	}

//	constexpr hash_t hash_pawn() const
//	{
//		return _hash_pawn;
//	}

	constexpr bool check(const color_t color) const
	{
		return color == white ? board_king_white() & board_attack_black() : board_king_black() & board_attack_white();
	}

	constexpr bool can_castle_white_king() const
	{
//		constexpr board_t occupied = F1 | G1;
//		constexpr board_t attacked = E1 | F1 | G1;
		return (_castle & white_castle_king) && !(board_occupy() & (F1 | G1)) && !(board_attack_black() & (E1 | F1 | G1));
	}

	constexpr bool can_castle_white_queen() const
	{
//		constexpr board_t occupied = B1 | C1 | D1;
//		constexpr board_t attacked = C1 | D1 | E1;
		return (_castle & white_castle_queen) && !(board_occupy() & (B1 | C1 | D1)) && !(board_attack_black() & (C1 | D1 | E1));
	}

	constexpr bool can_castle_black_king() const
	{
//		constexpr board_t occupied = F8 | G8;
//		constexpr board_t attacked = E8 | F8 | G8;
		return (_castle & black_castle_king) && !(board_occupy() & (F8 | G8)) && !(board_attack_white() & (E8 | F8 | G8));
	}

	constexpr bool can_castle_black_queen() const
	{
//		constexpr board_t occupied = B8 | C8 | D8;
//		constexpr board_t attacked = C8 | D8 | E8;
		return (_castle & black_castle_queen) && !(board_occupy() & (B8 | C8 | D8)) && !(board_attack_white() & (C8 | D8 | E8));
	}

	constexpr std::uint8_t count_pieces_white() const
	{
		return _count_pieces_white;
	}

	constexpr std::uint8_t count_pieces_black() const
	{
		return _count_pieces_black;
	}

	constexpr std::uint8_t count_pieces() const
	{
		return count_pieces_white() + count_pieces_black();
	}

	constexpr std::uint8_t count_half_moves() const
	{
		return _count_half_moves;
	}

	constexpr std::uint8_t count_full_moves() const
	{
		return _count_full_moves;
	}

	inline square_t flip(const square_t en_passant)
	{
//		const en_passant_hash en_passant_hash;
//		const color_hash color_hash;
		const square_t result = _square_en_passant;
		// Set enPassant
		_hash_node ^= en_passant_hash()(result);
		_square_en_passant = en_passant;
		_hash_node ^= en_passant_hash()(en_passant);
		// Set color
		_hash_node ^= color_hash()();
		_color *= -1;
		// Set counter
	//	_countMovesFull += (_color == Color::w);
		return result;
	}

};

}
