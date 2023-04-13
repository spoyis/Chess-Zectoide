#pragma once
#include "BitBoard.h"
namespace chess
{ // begin namespace chess
 static constexpr unsigned long invalid_move = 420;
class Move 
{ // begin class Move
public:
  unsigned long initialSquare;
  BitBoard possibleMoves;
}; // end class Move

} // end namespace chess