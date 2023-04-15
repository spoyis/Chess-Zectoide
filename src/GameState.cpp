#include "../include/GameState.h"

namespace chess
{// begin namespace chess

void GameState::make(int pieceId, unsigned long startSquare, unsigned long finalSquare)
{
	futureEnPassantSquare = NO_EN_PASSANT;

	// update your own piece bitboard
	// for your own color.
	boardState.board[pieceId + boardStateOffset] += finalSquare;
	boardState.board[pieces_offset + whoseTurn] += finalSquare;

	boardState.board[pieceId + boardStateOffset] -= startSquare;
	boardState.board[pieces_offset + whoseTurn] -= startSquare;

	// THIS LOOKS FUCKING CRAZY
	// BUT IT'S HOW YOU SEND A MESSAGE TO AN OBJECT USING POINTERS.
	// ALSO, WHICHEVER METHOD CALLED WILL RETURN A BOOLEAN INDICATING WHETHER OR NOT THE MOVE INCLUDES A CAPTURE
	// YOU'RE WELCOME, FUTURE SELF. 
	if ((this->*methodPointers[pieceId])(startSquare, finalSquare)) 
	{
		// remove correct piece on the specific bitboard
		BitBoard excludeFinalSquare{ 0xFFFFFFFFFFFFFFFF ^ (1ULL << finalSquare)};
		BitBoard* piece = &boardState.board[white_pieces_offset - boardStateOffset];
		excludeFinalSquare.debug("LOLOLOLOLOLOL");
		(piece++)->operator&=(excludeFinalSquare.board);     // king
		(piece++)->operator&=(excludeFinalSquare.board);    // queen 
		(piece++)->operator&=(excludeFinalSquare.board);   // rook 
		(piece++)->operator&=(excludeFinalSquare.board);  // bishop
		(piece++)->operator&=(excludeFinalSquare.board); // knight 
		(piece++)->operator&=(excludeFinalSquare.board);// pawn

		// do the same for general colored pieces bitboard
		// excludes finalsquare from opponents general pieces bitboard
		ENEMYBITBOARD -= finalSquare;
	}


	enPassantSquare = futureEnPassantSquare;
	boardStateOffset ^= white_pieces_offset;
	whoseTurn = !whoseTurn;
	boardState.board[en_passant_board] = futureEnPassantSquare != NO_EN_PASSANT ? (1ULL << enPassantSquare) : 0ULL;
	boardState.board[joined_board] = boardState.board[pieces_offset] + boardState.board[pieces_offset + 1];
	boardState.board[empty_board] =  ~boardState.board[joined_board];
}

bool GameState::makePawn(unsigned long startSquare, unsigned long finalSquare)
{
	//check if you're promoting
	if (BitBoard{ pawnPromotionRank[boardStateOffset] } [finalSquare] ) {
		// put gamestate into piece promotion state
		// player/engine has yet to choose a piece to promote
		isBeingPromoted = true;
		promotedPieceIndex = finalSquare;
	}

	if (abs((int)startSquare - (int)finalSquare) >= 10)
	{
		// moved two squares up
		// next move could be en passant
		futureEnPassantSquare = finalSquare - pawnMoveDirection[boardStateOffset];
	}

	//check if attack move
	if (ENEMYBITBOARD[finalSquare])
	{
		
		return true;
	}
	else if (finalSquare == enPassantSquare)
	{
		finalSquare -= pawnMoveDirection[boardStateOffset];
		
		boardState.board[pawn + white_pieces_offset - boardStateOffset] -= finalSquare;
		ENEMYBITBOARD -= finalSquare;
	}

	return false;
}

void GameState::promote(int pieceId)
{
	boardState.board[pawn + white_pieces_offset - boardStateOffset] -= promotedPieceIndex;
	boardState.board[pieceId + white_pieces_offset - boardStateOffset] += promotedPieceIndex;
	isBeingPromoted = false;
}

bool chess::GameState::makeOrthodiagonal(unsigned long startSquare, unsigned long finalSquare)
{
	if (ENEMYBITBOARD[finalSquare])
		return true;
	
	return false;
}

bool GameState::makeKing(unsigned long startSquare, unsigned long finalSquare)
{
	longCastle[whoseTurn] = false;
	shortCastle[whoseTurn] = false;


	if (startSquare - finalSquare == 2) // long castle
	{
		const auto oldRookSquare = finalSquare - 2;
		const auto newRookSquare = finalSquare + 1;
		// move rook
		boardState.board[rook + boardStateOffset] += newRookSquare;
		boardState.board[pieces_offset + whoseTurn] += newRookSquare;

		boardState.board[rook + boardStateOffset] -= oldRookSquare;
		boardState.board[pieces_offset + whoseTurn] -= oldRookSquare;
		// move rook
		return false;
	}

	if (finalSquare - startSquare == 2) // short castle
	{
		const auto oldRookSquare = finalSquare + 1;
		const auto newRookSquare = finalSquare - 1;
		// move rook
		boardState.board[rook + boardStateOffset] += newRookSquare;
		boardState.board[pieces_offset + whoseTurn] += newRookSquare;

		boardState.board[rook + boardStateOffset] -= oldRookSquare;
		boardState.board[pieces_offset + whoseTurn] -= oldRookSquare;

		return false;
	}

	if (ENEMYBITBOARD[finalSquare])
		return true;

	return false;
}

}// end namespace chess