#include "../include/GameState.h"

namespace chess
{
  #define NORTH 8
  #define SOUTH -8
  #define EAST 1
  #define WEST -1

  MoveList chess::GameState::generatePawnMoves()
  {
    BitBoard Pawns = ALLYPAWNS;
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

      moves.addMove(Move{piece, PossibleMoves});
    }

    return moves;
  }

  MoveList GameState::generateRookMoves()
  {
      BitBoard Rooks = ALLYROOKS;
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
      BitBoard Bishops = ALLYBISHOPS;
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
      BitBoard Knights = ALLYKNIGHTS;
      MoveList moves(Knights.populationCount());

      while (Knights.board)
      {
          auto piece = Knights.popBit();
          BitBoard PossibleMoves{ BitBoard(KnightMoves::get[piece]) & (ENEMYBITBOARD + boardState.board[empty_board])};
          moves.addMove(Move{ piece, PossibleMoves });
      }

      return moves;
  }

  MoveList GameState::generateKingMoves()
  {
      BitBoard King = ALLYKING;
      MoveList moves(1);

      auto kingPos = King.popBit();
      BitBoard PossibleMoves{ BitBoard(KingMoves::get[kingPos]) & (ENEMYBITBOARD + boardState.board[empty_board]) };

      BitBoard longRay = { longCastleRay[boardStateOffset] };
      if (longCastle[whoseTurn] and (longRay & boardState.board[empty_board]) == longRay) {
          auto pos = kingPos;
          bool attacked = isThisSquareUnderAttack(pos);
          attacked |= isThisSquareUnderAttack(--pos);
          attacked |= isThisSquareUnderAttack(--pos);
          if (!attacked)
              PossibleMoves += pos;
      }

      BitBoard shortRay = { shortCastleRay[boardStateOffset] };
      
      if (shortCastle[whoseTurn] and (shortRay & boardState.board[empty_board]) == shortRay) {
          auto pos = kingPos;
          bool attacked = isThisSquareUnderAttack(pos);
          attacked |= isThisSquareUnderAttack(++pos);
          attacked |= isThisSquareUnderAttack(++pos);
          if (!attacked)
              PossibleMoves += pos;
      }

      moves.addMove(Move{ kingPos, PossibleMoves });
      
      return moves;
  }

  std::vector<MoveList> GameState::getAllMoves()
  {
      std::vector<MoveList> output(6);

      output[king]   = generateKingMoves();
      output[queen]  = generateQueenMoves();
      output[rook]   = generateRookMoves();
      output[bishop] = generateBishopMoves();
      output[knight] = generateKnightMoves();
      output[pawn]   = generatePawnMoves();

      return output;
  }

  bool GameState::isThisSquareUnderAttack(unsigned long square)
  {
      BitBoard diagonals;
      BitBoard orthogonals;
      BitBoard squareBitboard(square);

      castRay<&BitBoard::north>(orthogonals, square);
      castRay<&BitBoard::south>(orthogonals, square);
      castRay<&BitBoard::west>(orthogonals, square);
      castRay<&BitBoard::east>(orthogonals, square);

      castRay<&BitBoard::northwest>(diagonals, square);
      castRay<&BitBoard::northeast>(diagonals, square);
      castRay<&BitBoard::southeast>(diagonals, square);
      castRay<&BitBoard::southwest>(diagonals, square);

      BitBoard intersections;

      // save intersection with enemy pieces that move diagonally
      intersections += diagonals & (ENEMYBISHOPS + ENEMYQUEENS);
      // save intersection with enemy pieces that move orthogonally
      intersections += orthogonals & (ENEMYROOKS + ENEMYQUEENS);

      // save intersection with enemy king moves
      BitBoard King = ENEMYKING;
      if (King.board != 0) { // TODO: FIX THIS SHIT
                            // THIS IS HAPPENING BECAUSE THERES NO GAME OVER STATE IN GAMESTATE, THE SEARCH TRIES TO SEE IF A CAPTURED KING IS UNDER ATTACK, AND THAT DOESN'T MAKE ANY SENSE.
          auto king = King.popBit();
          intersections += squareBitboard & BitBoard(KingMoves::get[king]);
      }
      // save intersection with enemy knight moves
      BitBoard Knights = ENEMYKNIGHTS;
      BitBoard knightMoves;
      while (Knights.board)
      {
          auto enemyknight = Knights.popBit();
          knightMoves += BitBoard(KnightMoves::get[enemyknight]);
      }
      intersections += squareBitboard & knightMoves;

      return intersections.board;
  }

  template<void (BitBoard::* direction)()>
  inline void chess::GameState::castRay(BitBoard& output, unsigned long startingSquare)
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


  bool chess::GameState::wasTheLastMoveLegal()
  {
      auto enemyKing = ENEMYKING;

      // VERY DANGEROUS CODE, DO NOT CHANGE
      // TEMPORARILY CHANGE THE OFFSETS
      boardStateOffset ^= white_pieces_offset;
      auto output = !isThisSquareUnderAttack(enemyKing.popBit());
      boardStateOffset ^= white_pieces_offset;
      return output;
  }
 
  void GameState::filterSelfChecks(unsigned long kingPos, MoveList& moves)
  {
  }

}