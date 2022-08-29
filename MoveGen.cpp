#include "GameState.h"

namespace chess
{
  #define NORTH 8
  #define SOUTH -8
  #define EAST 1
  #define WEST -1

  MoveList chess::GameState::generatePawnMoves()
  {
    BitBoard Pawns = boardState.board[pawn + boardStateOffset];
    MoveList moves(Pawns.populationCount());
   
    #define PAWNDIRECTION     pawnMoveDirection[boardStateOffset]
    #define PAWNSTARTINGRANK  startingPawnRank[boardStateOffset]
    
    while(Pawns.board)
    {
      BitBoard PossibleMoves;
      BitBoard nextMove;
      auto piece = Pawns.popBit();
      nextMove += piece;
      bool isOnStartingRank = nextMove.board & PAWNSTARTINGRANK ? true : false;

      nextMove.shift(PAWNDIRECTION); // shifts bitboard one space north/south
      PossibleMoves += (nextMove & boardState.board[empty_board]);

      //calculate attack squares
      BitBoard AttackMoves((nextMove.shift_and_clone(WEST) & Not_H_File) + (nextMove.shift_and_clone(EAST) & Not_A_File));
      AttackMoves &= (boardState.board[en_passant_board] + ENEMYBITBOARD); // attacked squares + enpassant
      PossibleMoves += AttackMoves;

      if (isOnStartingRank) // if the pawns original square is on the starting rank, it can move two spaces
      { 
        nextMove.shift(PAWNDIRECTION); // shifts bitboard one space north/south
        PossibleMoves += (nextMove & boardState.board[empty_board]);
        // i hate pawns
      }
      std::cout << "STARTINGSQUARE: " << piece << '\n';
      PossibleMoves.debug("POSSIBLE MOVES");
      moves.addMove(Move{piece, PossibleMoves});
    }

    return moves;
  }

}