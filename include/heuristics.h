#pragma once
#include "../include/GameState.h"

namespace AI::Heuristic 
{

	#define INNER_CENTER_BONUS 3
	#define OUTER_CENTER_BONUS 1
	
	template<bool evalForWhite>
	double inline pieceScore(chess::GameState& game) 
	{
		
		const int allyOffset = evalForWhite ? chess::white_pieces_offset : 0;
		const int enemyOffset = evalForWhite ? 0 : chess::white_pieces_offset;
		//game.boardState.board[chess::pawn + allyOffset].debug("ALLY PAWNS");
		//game.boardState.board[chess::pawn + enemyOffset].debug("ENEMY PAWNS");

		const double allyPawnCount   = game.boardState.board[chess::pawn   + allyOffset].populationCount();
		const double allyBishopCount = game.boardState.board[chess::bishop + allyOffset].populationCount();
		const double allyKnightCount = game.boardState.board[chess::knight + allyOffset].populationCount();
		const double allyKingCount   = game.boardState.board[chess::king   + allyOffset].populationCount();
		const double allyQueenCount  = game.boardState.board[chess::queen  + allyOffset].populationCount();
		const double allyRookCount   = game.boardState.board[chess::rook   + allyOffset].populationCount();

		const double enemyPawnCount   = game.boardState.board[chess::pawn   + enemyOffset].populationCount();
		const double enemyBishopCount = game.boardState.board[chess::bishop + enemyOffset].populationCount();
		const double enemyKnightCount = game.boardState.board[chess::knight + enemyOffset].populationCount();
		const double enemyKingCount   = game.boardState.board[chess::king   + enemyOffset].populationCount();
		const double enemyQueenCount  = game.boardState.board[chess::queen  + enemyOffset].populationCount();
		const double enemyRookCount   = game.boardState.board[chess::rook   + enemyOffset].populationCount();

		const double pawnScore = (allyPawnCount - enemyPawnCount);
		const double bishopScore = (allyBishopCount - enemyBishopCount) * 3.3;
		const double knightScore = (allyKnightCount - enemyKnightCount) * 3.0;
		const double kingScore = (allyKingCount - enemyKingCount) * 3000.0;
		const double queenScore = (allyQueenCount - enemyQueenCount) * 9.0;
		const double rookScore = (allyRookCount - enemyRookCount) * 5.0;
		//std::cout << "EVALUATIION ------------------" << pawnScore + bishopScore + knightScore + kingScore + queenScore + rookScore << "----------\n";

		return pawnScore + bishopScore + knightScore + kingScore + queenScore + rookScore;
	}


	// TODO: FOR SOME REASON, THIS IS RETURNING THE VALUE FLIPPED, SOLVE THIS 
	template<bool evalForWhite>
	double inline centerScore(chess::GameState& game) {

		const int allyOffset = evalForWhite ? chess::pieces_offset + 1 : chess::pieces_offset;
		const int enemyOffset = evalForWhite ? chess::pieces_offset : chess::pieces_offset + 1;

		const auto innerCenterAlly = (game.boardState.board[allyOffset] & chess::BitBoard(chess::innerCenterSquares)).populationCount();
		const auto innerCenterEnemy = (game.boardState.board[enemyOffset] & chess::BitBoard(chess::innerCenterSquares)).populationCount();

		const auto outerCenterAlly = (game.boardState.board[allyOffset] & chess::BitBoard(chess::outerCenterSquares)).populationCount();
		const auto outerCenterEnemy = (game.boardState.board[enemyOffset] & chess::BitBoard(chess::outerCenterSquares)).populationCount();

		return (innerCenterAlly - innerCenterEnemy) * INNER_CENTER_BONUS + (outerCenterAlly - outerCenterEnemy) * OUTER_CENTER_BONUS;
	}

	template<bool evalForWhite>
	double inline eval(chess::GameState& game) {
		return -centerScore<evalForWhite>(game) + pieceScore<evalForWhite>(game);
	}
}