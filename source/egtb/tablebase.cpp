#include "tablebase.hpp"
#include "egtb.hpp"
#include <iostream>

namespace chess {

static inline void
init(std::uint32_t* rgCtr, unsigned int* rgSquares, const piece_t piece, board_t board)
{
	std::uint32_t counter = 0;
	while (board)
	{
		const square_t square = board_clear_square_forward(board);
		(rgSquares)[piece * C_PIECES + counter] = square;
		counter++;
	}
	(rgCtr)[piece] = counter;
}

tablebase_t::tablebase_t(const std::string& path, const std::size_t size)
:
	_mutex(),
	_cache(new char[size]),
	_pieces(IInitializeTb(const_cast<char*>(path.data())))
{
	if (!FTbSetCacheSize(_cache.get(), size))
		std::__throw_runtime_error("FTbSetCacheSize failed.");
}

bool
tablebase_t::operator()(const node_t& node, const std::uint_fast8_t height, score_t& score) const
{
	std::uint32_t rgiCounters[10];
	unsigned int rgsqWhite[C_PIECES * 5 + 1];
	unsigned int rgsqBlack[C_PIECES * 5 + 1];

	// initialize counters and piece arrays so the probe code
	// can compute the modified Godel number.
	init(rgiCounters, rgsqWhite, 0, node.board_pawns_white());
	init(rgiCounters, rgsqWhite, 1, node.board_knights_white());
	init(rgiCounters, rgsqWhite, 2, node.board_bishops_white());
	init(rgiCounters, rgsqWhite, 3, node.board_rooks_white());
	init(rgiCounters, rgsqWhite, 4, node.board_queens_white());
	init(rgiCounters + 5, rgsqBlack, 0, node.board_pawns_black());
	init(rgiCounters + 5, rgsqBlack, 1, node.board_knights_black());
	init(rgiCounters + 5, rgsqBlack, 2, node.board_bishops_black());
	init(rgiCounters + 5, rgsqBlack, 3, node.board_rooks_black());
	init(rgiCounters + 5, rgsqBlack, 4, node.board_queens_black());

	std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(_mutex));

	// tablebase registered ?
	int iTb = IDescFindFromCounters((int*)rgiCounters);
	if (!iTb)
		return false;

	// set up tablebase parameters
	std::uint32_t invert;
	int side;
	unsigned int* psqW;
	unsigned int* psqB;

	rgsqWhite[C_PIECES * 5] = node.square_king_white();
	rgsqBlack[C_PIECES * 5] = node.square_king_black();
	if (iTb > 0)
	{
		side = node.color() == black;
		invert = false;
		psqW = rgsqWhite;
		psqB = rgsqBlack;
	}
	else
	{
		side = node.color() == white;
		invert = true;
		psqW = rgsqBlack;
		psqB = rgsqWhite;
		iTb = -iTb;
	}

	// tablebase registered ?
	if (!FRegisteredFun(iTb, side))
		return false;

	// get tbValue
	const unsigned int sqEnP = node.square_en_passant() ? node.square_en_passant() : 127;
	const std::uint64_t index = PfnIndCalcFun(iTb, side) ((unsigned int *)psqW, (unsigned int *)psqB, sqEnP, invert);
	const score_t tbValue = L_TbtProbeTable(iTb, side, index);
	if (tbValue == L_bev_broken)
		return false;

	// convert tbValue to score
	if (tbValue > 0)
		score = /*Score::max*/ +30000 + 2 * (-L_bev_mi1 + tbValue - 1) - (score_t)height + 1;
	else if (tbValue < 0)
		score = /*Score::min*/ -30000 + 2 * (+L_bev_mi1 + tbValue) + (score_t)height;
	else
		score = 0;//Score::remis;

	return true;
}

}
