#pragma once
#include "BoardState.h"
#include "MoveList.h"

#define NO_EN_PASSANT 100

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
	void make(int pieceId, unsigned long startSquare, unsigned long finalSquare);
	bool makePawn(unsigned long startSquare, unsigned long finalSquare);
	bool turnQuery(bool color) { return color == whoseTurn; }
	int getBoardStateOffset() { return boardStateOffset; }
	auto getPromotionState() { return  isBeingPromoted; };
	void promote(int pieceId);
private:
	bool whoseTurn{WHITES_TURN};
	int boardStateOffset{white_pieces_offset};
	int promotedPieceIndex;
	unsigned long enPassantSquare{NO_EN_PASSANT};
	unsigned long futureEnPassantSquare;
	bool isBeingPromoted{};
	
}; // end class GameState


typedef bool (GameState::*MemFunctPtr)(unsigned long, unsigned long);


constexpr MemFunctPtr methodPointers[] = {
	nullptr,//king
	nullptr,//queen
	nullptr,//rook
	nullptr,//bishop
	nullptr,//knight 
	&GameState::makePawn//pawn
};

// macro below used in GameState methods
#define ENEMYBITBOARD     boardState.board[enemyPieceIndex[boardStateOffset]]

}// end namespace chess