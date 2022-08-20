#pragma once
#include "Move.h"
namespace chess
{ // begin namespace chess

class MoveListIterator 
{ // begin class MoveListIterator
public:
  MoveListIterator(Move* ptr) : ptr{ ptr }{};
  
  void operator++()
  {
    ptr++;
  }
  
  void operator--()
  {
    ptr--;
  }

  bool operator<(MoveListIterator other)
  {
    return this->ptr < other.ptr;
  }

  bool operator==(MoveListIterator other) const
  {
    return this->ptr == other.ptr;
  }

  bool operator!=(MoveListIterator other) const
  {
    return this->ptr != other.ptr;
  }

  Move operator*()
  {
    return *ptr;
  }

private:
  Move* ptr;
}; // end class MoveListIterator

    //////////////////////////////////
//////////////////////////////////////////// oi, tudo bem?
    //////////////////////////////////

class MoveList
{ // begin class MoveList
public:
  MoveList() = default;
  MoveList(int _size) : _size{_size} { moves = new Move[_size]; };
  ~MoveList() { delete moves; }

  MoveList(MoveList&& other) noexcept
  {
    this->moves = other.moves;
    other.moves = nullptr;
    this->_size = other._size;
    this->occupied = other.occupied;
  }

  MoveList& operator=(MoveList&& other) noexcept
  {
    this->moves = other.moves;
    other.moves = nullptr;
    this->_size = other._size;
    this->occupied = other.occupied;

    return *this;
  }

  MoveListIterator begin() 
  {
    return MoveListIterator(moves);
  };

  MoveListIterator end()
  {
    return MoveListIterator(moves + occupied);
  };
  auto size() { return _size; }
  void addMove(Move move) 
  {
    // no verification to check if adding to unallocated space
    // this method will be called A LOT during search, the if statement would just slow down performance
    // just make sure it never happens, lol
    moves[occupied++] = move; 
  } 
private:
  Move* moves{};
  int _size;
  int occupied{};
}; // end class MoveList


} // end namespace chess