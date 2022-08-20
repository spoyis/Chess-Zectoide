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
    #define ENEMYBITBOARD      boardState.board[enemyPieceIndex[boardStateOffset]]
    
    while(Pawns.board)
    {
      BitBoard PossibleMoves;
      BitBoard nextMove;
      auto piece = Pawns.popBit();
      nextMove += piece;
      bool isOnStartingRank = nextMove.board & PAWNSTARTINGRANK ? true : false;

      nextMove.shift(PAWNDIRECTION); // shifts bitboard one space north/south
      PossibleMoves += (nextMove & boardState.board[empty_board]);


      PossibleMoves.debug("POSSIBLE MOVES ONE"); // debug
      BitBoard(boardState.board[pieces_offset + 1]).debug("WHITE PIECES");
      for (int i = 0; i < 8; i++)
      {
          std::string string = "";
          string += (char)i + '0';
          string += " file";
          BitBoard(file[i]).debug(string.c_str());
      }
      BitBoard(Not_H_File).debug("NOT_H_FILE");
      
      //calculate attack squares
      BitBoard AttackMoves((nextMove.shift_and_clone(WEST) & Not_H_File) + (nextMove.shift_and_clone(EAST) & Not_A_File));
      AttackMoves.debug("ATTACK MOVES");
      AttackMoves &= (boardState.board[en_passant_board] + ENEMYBITBOARD); // attacked squares + enpassant
      PossibleMoves += AttackMoves;

      PossibleMoves.debug("POSSIBLE MOVES TWO"); // debug
      

      if (isOnStartingRank) // if the pawns original square is on the starting rank, it can move two spaces
      { 
        nextMove.shift(PAWNDIRECTION); // shifts bitboard one space north/south
        PossibleMoves += (nextMove ^ boardState.board[empty_board]);
        PossibleMoves.debug("POSSIBLE MOVES THREE"); // debug
        // i hate pawns
      }
      
      moves.addMove(Move{piece, PossibleMoves});
    }

    return moves;
  }

}