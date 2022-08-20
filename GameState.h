#pragma once
#include "BoardState.h"
#include "MoveList.h"

namespace chess
{ // begin namespace chess

constexpr bool WHITES_TURN = 1;
constexpr bool BLACKS_TURN = 0;

class GameState
{ // begin class GameState
public:
	BoardState boardState;
	MoveList generatePawnMoves();
	MoveList generateRookMoves();
	MoveList generateBishopMoves();
	bool turnQuery(bool color) { return color == whoseTurn; }
private:
	bool whoseTurn{WHITES_TURN};
	int boardStateOffset{white_pieces_offset};
}; // end class GameState

}// end namespace chess