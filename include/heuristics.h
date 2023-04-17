#pragma once
#include "../include/GameState.h"

namespace AI::Heuristic 
{
	template<bool evalForWhite>
	double pieceScore(chess::GameState& game) 
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
		const double kingScore = (allyKingCount - enemyKingCount) * 30.0;
		const double queenScore = (allyQueenCount - enemyQueenCount) * 9.0;
		const double rookScore = (allyRookCount - enemyRookCount) * 5.0;
		//std::cout << "EVALUATIION ------------------" << pawnScore + bishopScore + knightScore + kingScore + queenScore + rookScore << "----------\n";

		return pawnScore + bishopScore + knightScore + kingScore + queenScore + rookScore;
	}

}