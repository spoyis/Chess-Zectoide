#pragma once
#include "BitBoard.h"
#include "HexConstants.h"
namespace chess
{ // begin namespace chess

constexpr int white_pieces_offset = 6;
constexpr int pieces_offset = 12;
constexpr int joined_board = 14;
constexpr int empty_board = 15;
constexpr int en_passant_board = 16;


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

		for (int i = 0; i < white_pieces_offset; i++)
		{
			board[pieces_offset] += board[i];
			board[pieces_offset + 1] += board[i + white_pieces_offset];
		}

		board[joined_board] = board[pieces_offset] + board[pieces_offset + 1];
		board[empty_board] = ~board[joined_board];
	}
	BitBoard board[17];
}; // end class BoardState

} // end namespace chess
