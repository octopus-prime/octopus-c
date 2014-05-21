/*
 * node_executor.cpp
 *
 *  Created on: 19.06.2013
 *      Author: mike_gresens
 */

#include "node.hpp"
#include "node_attacker.hpp"
#include "board_rotation.hpp"
#include "io.hpp"
#include <functional>
#include <boost/format.hpp>

namespace chess {

class illegal_exception
:
	public std::exception
{
private:
	std::string _what;
	node_t _node;
	move_t _move;

public:
	illegal_exception(const std::string& what, const node_t& node, const move_t move) noexcept
	:
		std::exception(),
		_what(what),
		_node(node),
		_move(move)
	{
	}

	virtual ~illegal_exception() noexcept
	{
	}

	virtual const char* what() const noexcept
	{
		boost::format format("What = %s\nNode = %s\nMove = %s\n");
		format % _what;
		format % _node;
		format % std::make_pair(std::ref(_node), std::ref(_move));
		return format.str().data();
	}
};

class node_t::node_basic_executor
{
protected:
//	class rotate;
	class flip;
	class shift;

	template <piece_t>
	class move_executor;

	template <piece_t>
	class capture_executor;

	template <piece_t>
	class promote_executor;
};
/*
class node_t::node_basic_executor::rotate
{
private:
//	square_rotation<+8> _rotate90;
//	square_rotation<+7> _rotate45L;
//	square_rotation<+9> _rotate45R;

public:
	void operator()(node_t& successor, const square_t from, const square_t to) const
	{
//		successor._boardRotated90  ^= board_of(_rotate90(from));
//		successor._boardRotated90  |= board_of(_rotate90(to));
//		successor._boardRotated45L ^= board_of(_rotate45L(from));
//		successor._boardRotated45L |= board_of(_rotate45L(to));
//		successor._boardRotated45R ^= board_of(_rotate45R(from));
//		successor._boardRotated45R |= board_of(_rotate45R(to));
	}
};
*/
class node_t::node_basic_executor::shift
{
private:
	piece_square_hash _hash;
//	rotate _rotate;

public:
	void operator()(node_t& successor, const square_t from, const square_t to) const
	{
		const piece_t piece = successor[from];
		successor._hash_node ^= _hash(piece, from);
		successor._hash_node ^= _hash(piece, to);
		successor._pieces[to] = piece;
		successor._pieces[from] = no_piece;
//		_rotate(successor, from, to);
	}
};

class node_t::node_basic_executor::flip
{
private:
	en_passant_hash _hash;
	color_hash _hash2;

public:
	square_t operator()(node_t& successor, const square_t enPassant) const
	{
		const square_t result = successor._square_en_passant;
		// Set enPassant
		successor._hash_node ^= _hash(result);
		successor._square_en_passant = enPassant;
		successor._hash_node ^= _hash(enPassant);
		// Set color
		successor._hash_node ^= _hash2();
		successor._color *= -1;
		return result;
	}
};

template <piece_t>
class node_t::node_basic_executor::move_executor
{
public:
	square_t operator()(node_t& successor, const square_t from, const square_t to) const;
};

template <>
class node_t::node_basic_executor::move_executor<K>
{
private:
	shift _shift;
	castle_hash _hash;

protected:
	void do_castle(node_t& node, const signed delta) const
	{
		switch (delta)
		{
		case -2:
			node._board_occupy_white ^= H1 | F1;
			node._board_rooks_queens ^= H1 | F1;
			_shift(node, h1, f1);
	//		node._castle |= Castle::KK;
			break;
		case +2:
			node._board_occupy_white ^= A1 | D1;
			node._board_rooks_queens ^= A1 | D1;
			_shift(node, a1, d1);
	//		node._castle |= Castle::QQ;
			break;
		default:
			break;
		}
		node._hash_node ^= _hash(node._castle);
		node._castle	&= ~white_castle;
		node._hash_node ^= _hash(node._castle);
	}

public:
	square_t operator()(node_t& successor, const square_t squareFrom, const square_t squareTo) const
	{
		const board_t	boardFrom		= board_of(squareFrom);
		const board_t	boardTo			= board_of(squareTo);
		const board_t	mask			= boardFrom | boardTo;
		successor._board_occupy_white ^= mask;
		successor._square_king_white = squareTo;
		if (successor._castle & white_castle)
			do_castle(successor, squareFrom - squareTo);
		return 0;
	}
};

template <>
class node_t::node_basic_executor::move_executor<k>
{
private:
	shift _shift;
	castle_hash _hash;

protected:
	void do_castle(node_t& node, const signed delta) const
	{
		switch (delta)
		{
		case -2:
			node._board_occupy_black ^= H8 | F8;
			node._board_rooks_queens ^= H8 | F8;
			_shift(node, h8, f8);
	//		node._castle |= Castle::kk;
			break;
		case +2:
			node._board_occupy_black ^= A8 | D8;
			node._board_rooks_queens ^= A8 | D8;
			_shift(node, a8, d8);
	//		node._castle |= Castle::qq;
			break;
		default:
			break;
		}
		node._hash_node ^= _hash(node._castle);
		node._castle	&= ~black_castle;
		node._hash_node ^= _hash(node._castle);
	}

public:
	square_t operator()(node_t& successor, const square_t squareFrom, const square_t squareTo) const
	{
		const board_t	boardFrom		= board_of(squareFrom);
		const board_t	boardTo			= board_of(squareTo);
		const board_t	mask			= boardFrom | boardTo;
		successor._board_occupy_black ^= mask;
		successor._square_king_black = squareTo;
		if (successor._castle & black_castle)
			do_castle(successor, squareFrom - squareTo);
		return 0;
	}
};

template <>
class node_t::node_basic_executor::move_executor<Q>
{
public:
	square_t operator()(node_t& successor, const square_t squareFrom, const square_t squareTo) const
	{
		const board_t	boardFrom		= board_of(squareFrom);
		const board_t	boardTo			= board_of(squareTo);
		const board_t	mask			= boardFrom | boardTo;
		successor._board_occupy_white	^= mask;
		successor._board_rooks_queens	^= boardFrom;
		successor._board_rooks_queens	|= boardTo;
		successor._board_bishops_queens	^= boardFrom;
		successor._board_bishops_queens	|= boardTo;
		return 0;
	}
};

template <>
class node_t::node_basic_executor::move_executor<q>
{
public:
	square_t operator()(node_t& successor, const square_t squareFrom, const square_t squareTo) const
	{
		const board_t	boardFrom		= board_of(squareFrom);
		const board_t	boardTo			= board_of(squareTo);
		const board_t	mask			= boardFrom | boardTo;
		successor._board_occupy_black	^= mask;
		successor._board_rooks_queens	^= boardFrom;
		successor._board_rooks_queens	|= boardTo;
		successor._board_bishops_queens	^= boardFrom;
		successor._board_bishops_queens	|= boardTo;
		return 0;
	}
};

template <>
class node_t::node_basic_executor::move_executor<R>
{
private:
	castle_hash _hash;

protected:
	void do_castle(node_t& node, const square_t square) const
	{
		node._hash_node ^= _hash(node._castle);
		switch (square)
		{
		case h1:
			node._castle &= ~white_castle_king;
			break;
		case a1:
			node._castle &= ~white_castle_queen;
			break;
		default:
			break;
		}
		node._hash_node ^= _hash(node._castle);
	}

public:
	square_t operator()(node_t& successor, const square_t squareFrom, const square_t squareTo) const
	{
		const board_t	boardFrom		= board_of(squareFrom);
		const board_t	boardTo			= board_of(squareTo);
		const board_t	mask			= boardFrom | boardTo;
		successor._board_occupy_white	^= mask;
		successor._board_rooks_queens	^= boardFrom;
		successor._board_rooks_queens	|= boardTo;
		if (successor._castle & white_castle)
			do_castle(successor, squareFrom);
		return 0;
	}
};

template <>
class node_t::node_basic_executor::move_executor<r>
{
private:
	castle_hash _hash;

protected:
	void do_castle(node_t& node, const square_t square) const
	{
		node._hash_node ^= _hash(node._castle);
		switch (square)
		{
		case h8:
			node._castle &= ~black_castle_king;
			break;
		case a8:
			node._castle &= ~black_castle_queen;
			break;
		default:
			break;
		}
		node._hash_node ^= _hash(node._castle);
	}

public:
	square_t operator()(node_t& successor, const square_t squareFrom, const square_t squareTo) const
	{
		const board_t	boardFrom		= board_of(squareFrom);
		const board_t	boardTo			= board_of(squareTo);
		const board_t	mask			= boardFrom | boardTo;
		successor._board_occupy_black	^= mask;
		successor._board_rooks_queens	^= boardFrom;
		successor._board_rooks_queens	|= boardTo;
		if (successor._castle & black_castle)
			do_castle(successor, squareFrom);
		return 0;
	}
};

template <>
class node_t::node_basic_executor::move_executor<B>
{
public:
	square_t operator()(node_t& successor, const square_t squareFrom, const square_t squareTo) const
	{
		const board_t	boardFrom		= board_of(squareFrom);
		const board_t	boardTo			= board_of(squareTo);
		const board_t	mask			= boardFrom | boardTo;
		successor._board_occupy_white	^= mask;
		successor._board_bishops_queens	^= boardFrom;
		successor._board_bishops_queens	|= boardTo;
		return 0;
	}
};

template <>
class node_t::node_basic_executor::move_executor<b>
{
public:
	square_t operator()(node_t& successor, const square_t squareFrom, const square_t squareTo) const
	{
		const board_t	boardFrom		= board_of(squareFrom);
		const board_t	boardTo			= board_of(squareTo);
		const board_t	mask			= boardFrom | boardTo;
		successor._board_occupy_black	^= mask;
		successor._board_bishops_queens	^= boardFrom;
		successor._board_bishops_queens	|= boardTo;
		return 0;
	}
};

template <>
class node_t::node_basic_executor::move_executor<N>
{
public:
	square_t operator()(node_t& successor, const square_t squareFrom, const square_t squareTo) const
	{
		const board_t	boardFrom		= board_of(squareFrom);
		const board_t	boardTo			= board_of(squareTo);
		const board_t	mask			= boardFrom | boardTo;
		successor._board_occupy_white	^= mask;
		successor._board_knights		^= boardFrom;
		successor._board_knights		|= boardTo;
		return 0;
	}
};

template <>
class node_t::node_basic_executor::move_executor<n>
{
public:
	square_t operator()(node_t& successor, const square_t squareFrom, const square_t squareTo) const
	{
		const board_t	boardFrom		= board_of(squareFrom);
		const board_t	boardTo			= board_of(squareTo);
		const board_t	mask			= boardFrom | boardTo;
		successor._board_occupy_black	^= mask;
		successor._board_knights		^= boardFrom;
		successor._board_knights		|= boardTo;
		return 0;
	}
};

template <>
class node_t::node_basic_executor::move_executor<P>
{
private:
	piece_square_hash _hash;

public:
	square_t operator()(node_t& successor, const square_t squareFrom, const square_t squareTo) const
	{
		const board_t	boardFrom		= board_of(squareFrom);
		const board_t	boardTo			= board_of(squareTo);
		const board_t	mask			= boardFrom | boardTo;
//		successor._hash_pawn			^= _hash(P, squareFrom);
//		successor._hash_pawn			^= _hash(P, squareTo);
		successor._board_occupy_white	^= mask;
		successor._board_pawns			^= boardFrom;
		successor._board_pawns			|= boardTo;
		successor._count_half_moves		= 0;
		return (squareTo - squareFrom == 16) ? squareFrom + 8 : 0;
	}
};

template <>
class node_t::node_basic_executor::move_executor<p>
{
private:
	piece_square_hash _hash;

public:
	square_t operator()(node_t& successor, const square_t squareFrom, const square_t squareTo) const
	{
		const board_t	boardFrom		= board_of(squareFrom);
		const board_t	boardTo			= board_of(squareTo);
		const board_t	mask			= boardFrom | boardTo;
//		successor._hash_pawn			^= _hash(p, squareFrom);
//		successor._hash_pawn			^= _hash(p, squareTo);
		successor._board_occupy_black	^= mask;
		successor._board_pawns			^= boardFrom;
		successor._board_pawns			|= boardTo;
		successor._count_half_moves		= 0;
		return (squareFrom - squareTo == 16) ? squareFrom - 8 : 0;
	}
};

template <piece_t>
class node_t::node_basic_executor::capture_executor
{
public:
	void operator()(node_t& successor, const square_t to) const;
};

template <>
class node_t::node_basic_executor::capture_executor<Q>
{
private:
	score_of_piece _score_of;

public:
	void operator()(node_t& successor, const square_t to) const
	{
		successor._board_occupy_white	^= board_of(to);
		successor._board_rooks_queens	^= board_of(to);
		successor._board_bishops_queens	^= board_of(to);
		successor._score_material		-= _score_of(Q);
		successor._count_pieces_white	--;
	}
};

template <>
class node_t::node_basic_executor::capture_executor<q>
{
private:
	score_of_piece _score_of;

public:
	void operator()(node_t& successor, const square_t to) const
	{
		successor._board_occupy_black	^= board_of(to);
		successor._board_rooks_queens	^= board_of(to);
		successor._board_bishops_queens	^= board_of(to);
		successor._score_material		+= _score_of(q);
		successor._count_pieces_black	--;
	}
};

template <>
class node_t::node_basic_executor::capture_executor<R>
{
private:
	castle_hash _hash;
	score_of_piece _score_of;

protected:
	void do_castle(node_t& node, const square_t square) const
	{
		node._hash_node ^= _hash(node._castle);
		switch (square)
		{
		case h1:
			node._castle &= ~white_castle_king;
			break;
		case a1:
			node._castle &= ~white_castle_queen;
			break;
		default:
			break;
		}
		node._hash_node ^= _hash(node._castle);
	}

public:
	void operator()(node_t& successor, const square_t to) const
	{
		successor._board_occupy_white	^= board_of(to);
		successor._board_rooks_queens	^= board_of(to);
		successor._score_material		-= _score_of(R);
		successor._count_pieces_white	--;
		if (successor._castle & white_castle)
			do_castle(successor, to);
	}
};

template <>
class node_t::node_basic_executor::capture_executor<r>
{
private:
	castle_hash _hash;
	score_of_piece _score_of;

protected:
	void do_castle(node_t& node, const square_t square) const
	{
		node._hash_node ^= _hash(node._castle);
		switch (square)
		{
		case h8:
			node._castle &= ~black_castle_king;
			break;
		case a8:
			node._castle &= ~black_castle_queen;
			break;
		default:
			break;
		}
		node._hash_node ^= _hash(node._castle);
	}

public:
	void operator()(node_t& successor, const square_t to) const
	{
		successor._board_occupy_black	^= board_of(to);
		successor._board_rooks_queens	^= board_of(to);
		successor._score_material		+= _score_of(r);
		successor._count_pieces_black	--;
		if (successor._castle & black_castle)
			do_castle(successor, to);
	}
};

template <>
class node_t::node_basic_executor::capture_executor<B>
{
private:
	score_of_piece _score_of;

public:
	void operator()(node_t& successor, const square_t to) const
	{
		successor._board_occupy_white	^= board_of(to);
		successor._board_bishops_queens	^= board_of(to);
		successor._score_material		-= _score_of(B);
		successor._count_pieces_white	--;
	}
};

template <>
class node_t::node_basic_executor::capture_executor<b>
{
private:
	score_of_piece _score_of;

public:
	void operator()(node_t& successor, const square_t to) const
	{
		successor._board_occupy_black	^= board_of(to);
		successor._board_bishops_queens	^= board_of(to);
		successor._score_material		+= _score_of(b);
		successor._count_pieces_black	--;
	}
};

template <>
class node_t::node_basic_executor::capture_executor<N>
{
private:
	score_of_piece _score_of;

public:
	void operator()(node_t& successor, const square_t to) const
	{
		successor._board_occupy_white	^= board_of(to);
		successor._board_knights		^= board_of(to);
		successor._score_material		-= _score_of(N);
		successor._count_pieces_white	--;
	}
};

template <>
class node_t::node_basic_executor::capture_executor<n>
{
private:
	score_of_piece _score_of;

public:
	void operator()(node_t& successor, const square_t to) const
	{
		successor._board_occupy_black	^= board_of(to);
		successor._board_knights		^= board_of(to);
		successor._score_material		+= _score_of(n);
		successor._count_pieces_black	--;
	}
};

template <>
class node_t::node_basic_executor::capture_executor<P>
{
private:
	piece_square_hash _hash;
	score_of_piece _score_of;

public:
	void operator()(node_t& successor, const square_t to) const
	{
//		successor._hash_pawn			^= _hash(P, to);
		successor._board_occupy_white	^= board_of(to);
		successor._board_pawns			^= board_of(to);
		successor._score_material		-= _score_of(P);
		successor._count_pieces_white	--;
	}
};

template <>
class node_t::node_basic_executor::capture_executor<p>
{
private:
	piece_square_hash _hash;
	score_of_piece _score_of;

public:
	void operator()(node_t& successor, const square_t to) const
	{
//		successor._hash_pawn			^= _hash(p, to);
		successor._board_occupy_black	^= board_of(to);
		successor._board_pawns			^= board_of(to);
		successor._score_material		+= _score_of(p);
		successor._count_pieces_black	--;
	}
};

template <piece_t>
class node_t::node_basic_executor::promote_executor
{
public:
	void operator()(node_t& successor, const square_t to) const;
};

template <>
class node_t::node_basic_executor::promote_executor<Q>
{
private:
	score_of_piece _score_of;

public:
	void operator()(node_t& successor, const square_t to) const
	{
		successor._board_rooks_queens	|= board_of(to);
		successor._board_bishops_queens	|= board_of(to);
		successor._score_material		+= _score_of(Q) - _score_of(P);
	}
};

template <>
class node_t::node_basic_executor::promote_executor<q>
{
private:
	score_of_piece _score_of;

public:
	void operator()(node_t& successor, const square_t to) const
	{
		successor._board_rooks_queens	|= board_of(to);
		successor._board_bishops_queens	|= board_of(to);
		successor._score_material		-= _score_of(q) - _score_of(p);
	}
};

template <>
class node_t::node_basic_executor::promote_executor<R>
{
private:
	score_of_piece _score_of;

public:
	void operator()(node_t& successor, const square_t to) const
	{
		successor._board_rooks_queens	|= board_of(to);
		successor._score_material		+= _score_of(R) - _score_of(P);
	}
};

template <>
class node_t::node_basic_executor::promote_executor<r>
{
private:
	score_of_piece _score_of;

public:
	void operator()(node_t& successor, const square_t to) const
	{
		successor._board_rooks_queens	|= board_of(to);
		successor._score_material		-= _score_of(r) - _score_of(p);
	}
};

template <>
class node_t::node_basic_executor::promote_executor<B>
{
private:
	score_of_piece _score_of;

public:
	void operator()(node_t& successor, const square_t to) const
	{
		successor._board_bishops_queens	|= board_of(to);
		successor._score_material		+= _score_of(B) - _score_of(P);
	}
};

template <>
class node_t::node_basic_executor::promote_executor<b>
{
private:
	score_of_piece _score_of;

public:
	void operator()(node_t& successor, const square_t to) const
	{
		successor._board_bishops_queens	|= board_of(to);
		successor._score_material		-= _score_of(b) - _score_of(p);
	}
};

template <>
class node_t::node_basic_executor::promote_executor<N>
{
private:
	score_of_piece _score_of;

public:
	void operator()(node_t& successor, const square_t to) const
	{
		successor._board_knights		|= board_of(to);
		successor._score_material		+= _score_of(N) - _score_of(P);
	}
};

template <>
class node_t::node_basic_executor::promote_executor<n>
{
private:
	score_of_piece _score_of;

public:
	void operator()(node_t& successor, const square_t to) const
	{
		successor._board_knights		|= board_of(to);
		successor._score_material		-= _score_of(n) - _score_of(p);
	}
};

template <typename moves_tag>
class node_t::node_executor
{
public:
	void operator()(node_t& successor, const node_t& node, const move_t move) const;
};

template <>
class node_t::node_executor<active_moves_tag>
:
	node_t::node_basic_executor
{
private:
	piece_square_hash _hash;
	shift _shift;
	flip _flip;
//	rotate _rotate;
//	generator::AttackGenerator _attack;

public:
	void operator()(node_t& successor, const node_t& node, const move_t move) const
	{
		successor._count_half_moves ++;
		successor._count_full_moves += (node.color() == black);

		const std::uint_fast8_t	index_promoted	= move.promoted();
		const piece_t			piece_moved		= node[move.from()];
			  piece_t			piece_captured	= node[move.to()];

		if (successor.square_en_passant() && (successor.square_en_passant() == move.to()))
		{
			square_t index = move.to();
			switch (piece_moved)
			{
			case P:
				index -= 8;
				successor._board_occupy_black ^= board_of(index);
				successor._board_occupy_black |= board_of(move.to());
				piece_captured = p;
				successor._board_pawns ^= board_of(index);
				successor._board_pawns |= board_of(move.to());
				_shift(successor, index, move.to());
				break;
			case p:
				index += 8;
				successor._board_occupy_white ^= board_of(index);
				successor._board_occupy_white |= board_of(move.to());
				piece_captured = P;
				successor._board_pawns ^= board_of(index);
				successor._board_pawns |= board_of(move.to());
				_shift(successor, index, move.to());
				break;
			default:
				break;
			}
		}

		if (piece_captured)
		{
			successor._hash_node ^= _hash(piece_captured, move.to());

			switch (piece_captured)
			{
			case Q:
				capture_executor<Q>()(successor, move.to());
				break;
			case q:
				capture_executor<q>()(successor, move.to());
				break;
			case R:
				capture_executor<R>()(successor, move.to());
				break;
			case r:
				capture_executor<r>()(successor, move.to());
				break;
			case B:
				capture_executor<B>()(successor, move.to());
				break;
			case b:
				capture_executor<b>()(successor, move.to());
				break;
			case N:
				capture_executor<N>()(successor, move.to());
				break;
			case n:
				capture_executor<n>()(successor, move.to());
				break;
			case P:
				capture_executor<P>()(successor, move.to());
				break;
			case p:
				capture_executor<p>()(successor, move.to());
				break;
			default:
				throw illegal_exception("Illegal capture", node, move);
				break;
			}

			successor._count_half_moves = 0;
		}

		square_t enPassant = 0;
		switch (piece_moved)
		{
		case K:
			move_executor<K>()(successor, move.from(), move.to());
			break;
		case k:
			move_executor<k>()(successor, move.from(), move.to());
			break;
		case Q:
			move_executor<Q>()(successor, move.from(), move.to());
			break;
		case q:
			move_executor<q>()(successor, move.from(), move.to());
			break;
		case R:
			move_executor<R>()(successor, move.from(), move.to());
			break;
		case r:
			move_executor<r>()(successor, move.from(), move.to());
			break;
		case B:
			move_executor<B>()(successor, move.from(), move.to());
			break;
		case b:
			move_executor<b>()(successor, move.from(), move.to());
			break;
		case N:
			move_executor<N>()(successor, move.from(), move.to());
			break;
		case n:
			move_executor<n>()(successor, move.from(), move.to());
			break;
		case P:
			enPassant = move_executor<P>()(successor, move.from(), move.to());
			break;
		case p:
			enPassant = move_executor<p>()(successor, move.from(), move.to());
			break;
		default:
			throw illegal_exception("Illegal move", node, move);
			break;
		}

//		_rotate(successor, move.from(), move.to());
		successor._hash_node ^= _hash(piece_moved, move.from());

		if (index_promoted)
		{
			const piece_t piecePromoted		= piece_moved - index_promoted;
			successor._pieces[move.to()]	= piecePromoted;
			successor._board_pawns			^= board_of(move.to());
//			successor._hash_pawn			^= _hash(piece_moved, move.from());

			switch (piecePromoted)
			{
			case Q:
				promote_executor<Q>()(successor, move.to());
				break;
			case q:
				promote_executor<q>()(successor, move.to());
				break;
			case R:
				promote_executor<R>()(successor, move.to());
				break;
			case r:
				promote_executor<r>()(successor, move.to());
				break;
			case B:
				promote_executor<B>()(successor, move.to());
				break;
			case b:
				promote_executor<b>()(successor, move.to());
				break;
			case N:
				promote_executor<N>()(successor, move.to());
				break;
			case n:
				promote_executor<n>()(successor, move.to());
				break;
			default:
				throw illegal_exception("Illegal promote", node, move);
				break;
			}
		}
		else
			successor._pieces[move.to()] = piece_moved;

		successor._pieces[move.from()] = no_piece;
		successor._hash_node ^= _hash(successor[move.to()], move.to());
		_flip(successor, enPassant);

		const auto attacks = node_attacker()(successor);
		successor._board_attack_white = reinterpret_cast<const board_t* const>(&attacks)[0];
		successor._board_attack_black = reinterpret_cast<const board_t* const>(&attacks)[1];
//		successor._boardAttackWhite = _attack.white(successor);
//		successor._boardAttackBlack = _attack.black(successor);
	}
};

template <>
class node_t::node_executor<passive_moves_tag>
:
	node_t::node_basic_executor
{
private:
	shift _shift;
	flip _flip;
//	generator::AttackGenerator _attack;

public:
	void operator()(node_t& successor, const node_t& node, const move_t move) const
	{
		successor._count_half_moves ++;
		successor._count_full_moves += (node.color() == black);

		square_t enPassant = 0;
		switch (node[move.from()])
		{
		case K:
			move_executor<K>()(successor, move.from(), move.to());
			break;
		case k:
			move_executor<k>()(successor, move.from(), move.to());
			break;
		case Q:
			move_executor<Q>()(successor, move.from(), move.to());
			break;
		case q:
			move_executor<q>()(successor, move.from(), move.to());
			break;
		case R:
			move_executor<R>()(successor, move.from(), move.to());
			break;
		case r:
			move_executor<r>()(successor, move.from(), move.to());
			break;
		case B:
			move_executor<B>()(successor, move.from(), move.to());
			break;
		case b:
			move_executor<b>()(successor, move.from(), move.to());
			break;
		case N:
			move_executor<N>()(successor, move.from(), move.to());
			break;
		case n:
			move_executor<n>()(successor, move.from(), move.to());
			break;
		case P:
			enPassant = move_executor<P>()(successor, move.from(), move.to());
			break;
		case p:
			enPassant = move_executor<p>()(successor, move.from(), move.to());
			break;
		default:
			throw illegal_exception("Illegal move", node, move);
			break;
		}

		_shift(successor, move.from(), move.to());
		_flip(successor, enPassant);

		const auto attacks = node_attacker()(successor);
		successor._board_attack_white = reinterpret_cast<const board_t* const>(&attacks)[0];
		successor._board_attack_black = reinterpret_cast<const board_t* const>(&attacks)[1];
//		successor._boardAttackWhite = _attack.white(successor);
//		successor._boardAttackBlack = _attack.black(successor);
	}
};

node_t::node_t(const node_t& node, const move_t move, const active_moves_tag)
:
	node_t(node)
{
	node_executor<active_moves_tag>()(*this, node, move);
}

node_t::node_t(const node_t& node, const move_t move, const passive_moves_tag)
:
	node_t(node)
{
	node_executor<passive_moves_tag>()(*this, node, move);
}

}
