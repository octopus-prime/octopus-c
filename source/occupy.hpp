/*
 * occupy.hpp
 *
 *  Created on: 21.06.2013
 *      Author: mike_gresens
 */

#pragma once

#include "node.hpp"

namespace chess {

template <typename piece_tag, typename color_tag>
class occupy
:
	public std::unary_function<node_t, board_t>
{
public:
	board_t operator()(const node_t& node) const;
};

template <>
class occupy<king_piece_tag, white_color_tag>
{
public:
	constexpr board_t operator()(const node_t& node) const
	{
		return node.board_king_white();
	}
};

template <>
class occupy<king_piece_tag, black_color_tag>
{
public:
	constexpr board_t operator()(const node_t& node) const
	{
		return node.board_king_black();
	}
};

template <>
class occupy<rook_queen_piece_tag, white_color_tag>
{
public:
	constexpr board_t operator()(const node_t& node) const
	{
		return node.board_rooks_queens_white();
	}
};

template <>
class occupy<rook_queen_piece_tag, black_color_tag>
{
public:
	constexpr board_t operator()(const node_t& node) const
	{
		return node.board_rooks_queens_black();
	}
};

template <>
class occupy<bishop_queen_piece_tag, white_color_tag>
{
public:
	constexpr board_t operator()(const node_t& node) const
	{
		return node.board_bishops_queens_white();
	}
};

template <>
class occupy<bishop_queen_piece_tag, black_color_tag>
{
public:
	constexpr board_t operator()(const node_t& node) const
	{
		return node.board_bishops_queens_black();
	}
};

template <>
class occupy<knight_piece_tag, white_color_tag>
{
public:
	constexpr board_t operator()(const node_t& node) const
	{
		return node.board_knights_white();
	}
};

template <>
class occupy<knight_piece_tag, black_color_tag>
{
public:
	constexpr board_t operator()(const node_t& node) const
	{
		return node.board_knights_black();
	}
};

template <>
class occupy<pawn_piece_tag, white_color_tag>
{
public:
	constexpr board_t operator()(const node_t& node) const
	{
		return node.board_pawns_white();
	}
};

template <>
class occupy<pawn_piece_tag, black_color_tag>
{
public:
	constexpr board_t operator()(const node_t& node) const
	{
		return node.board_pawns_black();
	}
};

}
