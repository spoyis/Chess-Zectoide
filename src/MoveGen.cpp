#include "../include/GameState.h"

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

  MoveList GameState::generateRookMoves()
  {
      BitBoard Rooks = boardState.board[rook + boardStateOffset];
      MoveList moves(Rooks.populationCount());

      while (Rooks.board)
      {
          BitBoard PossibleMoves;

          auto piece = Rooks.popBit();
          castRay<&BitBoard::north>(PossibleMoves, piece);
          castRay<&BitBoard::south>(PossibleMoves, piece);
          castRay<&BitBoard::west>(PossibleMoves, piece);
          castRay<&BitBoard::east>(PossibleMoves, piece);
          moves.addMove(Move{ piece, PossibleMoves });
      }

      return moves;
  }

  MoveList GameState::generateBishopMoves()
  {
      BitBoard Bishops = boardState.board[bishop + boardStateOffset];
      MoveList moves(Bishops.populationCount());

      while (Bishops.board)
      {
          BitBoard PossibleMoves;

          auto piece = Bishops.popBit();
          
          castRay<&BitBoard::northwest>(PossibleMoves, piece);
          castRay<&BitBoard::northeast>(PossibleMoves, piece);
          castRay<&BitBoard::southeast>(PossibleMoves, piece);
          castRay<&BitBoard::southwest>(PossibleMoves, piece);
          moves.addMove(Move{ piece, PossibleMoves });
      }

      return moves;
  }

  MoveList GameState::generateQueenMoves()
  {
      BitBoard Queens = boardState.board[queen + boardStateOffset];
      MoveList moves(Queens.populationCount());

      while (Queens.board)
      {
          BitBoard PossibleMoves;

          auto piece = Queens.popBit();
          //todo: swap for northeast, northwest, southeast, southwest.
          castRay<&BitBoard::north>(PossibleMoves, piece);
          castRay<&BitBoard::south>(PossibleMoves, piece);
          castRay<&BitBoard::west>(PossibleMoves, piece);
          castRay<&BitBoard::east>(PossibleMoves, piece);
          castRay<&BitBoard::northwest>(PossibleMoves, piece);
          castRay<&BitBoard::northeast>(PossibleMoves, piece);
          castRay<&BitBoard::southeast>(PossibleMoves, piece);
          castRay<&BitBoard::southwest>(PossibleMoves, piece);
          moves.addMove(Move{ piece, PossibleMoves });
      }

      return moves;
  }
   
  MoveList GameState::generateKnightMoves()
  {
      BitBoard Knights = boardState.board[knight + boardStateOffset];
      MoveList moves(Knights.populationCount());

      while (Knights.board)
      {
          auto piece = Knights.popBit();
          BitBoard PossibleMoves{ BitBoard(KnightMoves::get[piece]) & (ENEMYBITBOARD + boardState.board[empty_board])};
          moves.addMove(Move{ piece, PossibleMoves });
      }

      return moves;
  }

  template<void (BitBoard::* direction)()>
  inline void chess::GameState::castRay(BitBoard& output, int startingSquare)
  {
      BitBoard square{ 1ULL << startingSquare };
      BitBoard ray;

      (square.*direction)();
      output += square & ENEMYBITBOARD; // edge case: enemy piece is on the first ray direction square.
      //square.debug("STARTING SQUARE");

      // loop unrolled for performance reasons --> for(int i = 0; i < 7; i++)
      // steps:
      // call direction method for the 'square' object (shifts bitboard towards direction,could be south/north/east/etc)
      // check if intersected with empty bitboard
      // add to output
      //i = 0
      square &= boardState.board[empty_board];
      ray += square;
      //i = 1
      (square.*direction)();
      square &= boardState.board[empty_board];
      ray += square;
      //i = 1
      (square.*direction)();
      square &= boardState.board[empty_board];
      ray += square;
      //i = 2
      (square.*direction)();
      square &= boardState.board[empty_board];
      ray += square;
      //i = 3
      (square.*direction)();
      square &= boardState.board[empty_board];
      ray += square;
      //i = 4
      (square.*direction)();
      square &= boardState.board[empty_board];
      ray += square;
      //i = 5
      (square.*direction)();
      square &= boardState.board[empty_board];
      ray += square;
      //i = 6
      (square.*direction)();
      square &= boardState.board[empty_board];
      ray += square;

      //ray.debug("FINAL RAY");
      output += ray;
      (ray.*direction)();
      ray &= ENEMYBITBOARD;
      //ray.debug("CAPTURE RAY");
      output += ray;
  }
}