#pragma once
#include "BoardState.h"
#include "MoveList.h"
#include <vector>

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
	MoveList generateQueenMoves();
	MoveList generateBishopMoves();
	MoveList generateKnightMoves();
	MoveList generateKingMoves();
	std::vector<MoveList> getAllMoves();
	bool isThisSquareUnderAttack(unsigned long square);
	void filterSelfChecks(unsigned long kingPos, MoveList& moves);
	void updateCastlingRights(unsigned long startSquare, unsigned long finalSquare);
	void make(int pieceId, unsigned long startSquare, unsigned long finalSquare);
	bool makePawn(unsigned long startSquare, unsigned long finalSquare);
	bool makeOrthodiagonal(unsigned long startSquare, unsigned long finalSquare);
	bool makeKing(unsigned long startSquare, unsigned long finalSquare);
	bool turnQuery(bool color) { return color == whoseTurn; }
	int getBoardStateOffset() { return boardStateOffset; }
	auto getPromotionState() { return  isBeingPromoted; };
	bool wasTheLastMoveLegal();
	void promote(int pieceId);
private:
	bool whoseTurn{WHITES_TURN};
	int boardStateOffset{white_pieces_offset};
	int promotedPieceIndex;
	unsigned long enPassantSquare{NO_EN_PASSANT};
	unsigned long futureEnPassantSquare;
	bool isBeingPromoted{};
	bool longCastle[2] =  { true, true };
	bool shortCastle[2] = { true, true };

	template<void (BitBoard::* BitBoardFunctPtr)()>
	inline void castRay(BitBoard& output, unsigned long startingSquare);
}; // end class GameState

typedef bool (GameState::* MemFunctPtr)(unsigned long, unsigned long);

constexpr MemFunctPtr methodPointers[] = {
	&GameState::makeKing,//king
	&GameState::makeOrthodiagonal,//queen
	&GameState::makeOrthodiagonal,//rook
	&GameState::makeOrthodiagonal,//bishop
	&GameState::makeOrthodiagonal,//knight 
	&GameState::makePawn//pawn
};

// macros below used in GameState methods
#define ENEMYBITBOARD     boardState.board[enemyPieceIndex[boardStateOffset]]

#define ALLYPAWNS         boardState.board[pawn + boardStateOffset]
#define ALLYROOKS         boardState.board[rook + boardStateOffset]
#define ALLYBISHOPS       boardState.board[bishop + boardStateOffset]
#define ALLYKNIGHTS       boardState.board[knight + boardStateOffset]
#define ALLYKING          boardState.board[king + boardStateOffset]

#define ENEMYPAWNS        boardState.board[pawn   + (boardStateOffset ^ white_pieces_offset)]
#define ENEMYROOKS        boardState.board[rook   + (boardStateOffset ^ white_pieces_offset)]
#define ENEMYBISHOPS      boardState.board[bishop + (boardStateOffset ^ white_pieces_offset)]
#define ENEMYKNIGHTS      boardState.board[knight + (boardStateOffset ^ white_pieces_offset)]
#define ENEMYKING         boardState.board[king   + (boardStateOffset ^ white_pieces_offset)]
#define ENEMYQUEENS       boardState.board[queen  + (boardStateOffset ^ white_pieces_offset)]

}// end namespace chess