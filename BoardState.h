#pragma once
#include "BitBoard.h"
#include "HexConstants.h"
namespace chess
{ // begin namespace chess

constexpr int white_pieces_offset = 6;
constexpr int pieces_offset = 12;
constexpr int joined_board = 14;

static const enum coords {
	a1, b1, c1, d1, e1, f1, g1, h1,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a8, b8, c8, d8, e8, f8, g8, h8
};

static const enum {
	king, queen, rook, bishop, knight, pawn
};

class BoardState
{ // begin class BoardState
public:

	// loads default board
	BoardState()
	{
		// black pieces
		board[king] += e8;
		board[queen] += d8;
		board[rook] += a8;
		board[rook] += h8;
		board[knight] += b8;
		board[knight] += g8;
		board[bishop] += c8;
		board[bishop] += f8;
		board[pawn] = rank[6];

		// white pieces
		board[king + white_pieces_offset] += e1;
		board[queen + white_pieces_offset] += d1;
		board[rook + white_pieces_offset] += a1;
		board[rook + white_pieces_offset] += h1;
		board[knight + white_pieces_offset] += b1;
		board[knight + white_pieces_offset] += g1;
		board[bishop + white_pieces_offset] += c1;
		board[bishop + white_pieces_offset] += f1;
		board[pawn + white_pieces_offset] = rank[1];
	}
	BitBoard board[15];
}; // end class BoardState

} // end namespace chess
