#pragma once
#include <iostream>

namespace chess
{ // begin namespace chess

static constexpr int INDEX[8][8] = {
	{0, 1, 2 ,3 , 4, 5, 6, 7},
	{8, 9, 10, 11 ,12 ,13, 14, 15},
	{16, 17, 18, 19, 20 ,21 ,22, 23},
	{24, 25, 26, 27, 28, 29, 30 ,31},
	{32, 33, 34, 35, 36 ,37 ,38, 39},
	{40, 41, 42, 43, 44, 45, 46, 47},
	{48, 49, 50, 51, 52, 53, 54, 55},
	{56, 57, 58, 59, 60, 61, 62, 63}
};

class BitBoard
{ // begin class BitBoard
public:
	uint64_t board{0};

	BitBoard() = default;
	~BitBoard() = default;

	// getter
	const bool operator[](int boardId) const
	{
		return board & (1ULL << boardId);
	}

	// bitboard operations
	void operator =(uint64_t otherBoard)
	{
		board = otherBoard;
	}

	BitBoard& operator +=(const int boardId)
	{
		board = board | (1ULL << boardId);
		return *this;
	}

	BitBoard& operator +=(const BitBoard& right)
	{
		board |= right.board;
		return *this;
	}

	BitBoard& operator+(const BitBoard& right)
	{
		BitBoard output;
		output.board = this->board | right.board;
		return output;
	}


private:
	// NO PRIVATE MEMBERS!

}; // end class BitBoard


} // end namespace chess
