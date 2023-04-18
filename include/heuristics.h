#pragma once
#include "../include/GameState.h"
#include <random>

namespace AI::Heuristic 
{

	#define INNER_CENTER_BONUS 2.1
	#define OUTER_CENTER_BONUS 1.1

	double inline randomEval() {
		std::minstd_rand rand(std::random_device{}());

		// Generate a random number between 0 and 1
		double randNum = static_cast<double>(rand()) / static_cast<double>(rand.max());

		// Scale the random number to the [0, 0.2] interval
		return randNum * 0.2;
	}
	
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


	template<bool evalForWhite>
	double inline centerScore(chess::GameState& game) {

		const int allyOffset = evalForWhite ? chess::pieces_offset + 1 : chess::pieces_offset;
		const int enemyOffset = evalForWhite ? chess::pieces_offset : chess::pieces_offset + 1;

		const double innerCenterAlly = (game.boardState.board[allyOffset] & chess::BitBoard(chess::innerCenterSquares)).populationCount();
		const double innerCenterEnemy = (game.boardState.board[enemyOffset] & chess::BitBoard(chess::innerCenterSquares)).populationCount();

		const double outerCenterAlly = (game.boardState.board[allyOffset] & chess::BitBoard(chess::outerCenterSquares)).populationCount();
		const double outerCenterEnemy = (game.boardState.board[enemyOffset] & chess::BitBoard(chess::outerCenterSquares)).populationCount();

		return (innerCenterAlly - innerCenterEnemy) * INNER_CENTER_BONUS + (outerCenterAlly - outerCenterEnemy) * OUTER_CENTER_BONUS;
	}

	template<bool evalForWhite>
	double inline eval(chess::GameState& game) {
		return centerScore<evalForWhite>(game) + pieceScore<evalForWhite>(game);// +randomEval();
	}
}