#pragma once
#include "../include/GameState.h"

namespace AI::Heuristic 
{
	template<bool evalForWhite>
	double pieceScore(chess::GameState& game) 
	{
		const double pawnScore   = 1.0  * game.boardState.board[chess::pawn   + evalForWhite ? chess::white_pieces_offset : 0].populationCount();
		const double bishopScore = 3.3  * game.boardState.board[chess::bishop + evalForWhite ? chess::white_pieces_offset : 0].populationCount();
		const double knightScore = 3.0  * game.boardState.board[chess::knight + evalForWhite ? chess::white_pieces_offset : 0].populationCount();
		const double kingScore   = 30.0 * game.boardState.board[chess::king   + evalForWhite ? chess::white_pieces_offset : 0].populationCount();
		const double queenScore  = 9.0  * game.boardState.board[chess::queen  + evalForWhite ? chess::white_pieces_offset : 0].populationCount();
		const double rookScore   = 5.0  * game.boardState.board[chess::rook   + evalForWhite ? chess::white_pieces_offset : 0].populationCount();

		return pawnScore + bishopScore + knightScore + kingScore + queenScore + rookScore;
	}

}