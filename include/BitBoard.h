#pragma once
#include <iostream>
#include <intrin.h>
#include "HexConstants.h"
namespace chess
{ // begin namespace chess


class BitBoard
{ // begin class BitBoard
public:
	uint64_t board{0};

	BitBoard() = default;
	~BitBoard() = default;
	BitBoard(uint64_t board) : board{ board } {};
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

	// flips bit at boardId index in the mask
	BitBoard& operator -=(const int boardId)
	{
		board = board ^ (1ULL << boardId);
		return *this;
	}

	BitBoard& operator +=(const BitBoard right)
	{
		board |= right.board;
		return *this;
	}

	BitBoard& operator &=(const BitBoard right)
	{
		board &= right.board;
		return *this;
	}

	BitBoard operator <<(const int shiftAmount)
	{
		BitBoard output(this->board << shiftAmount);
		return output;
	}

	BitBoard operator+(const BitBoard right)
	{
		BitBoard output(this->board | right.board);
		return output;
	}

	BitBoard operator^(const BitBoard right)
	{
		BitBoard output(this->board ^ right.board);
		return output;
	}

	BitBoard operator&(const BitBoard right)
	{
		BitBoard output(this->board & right.board);
		return output;
	}

	BitBoard operator~()
	{
		BitBoard output(~this->board);
		return output;
	}
	// returns number of ones in the bitboard
	auto populationCount()
	{
		return __popcnt64(board); // intrinsic function
	}

	// scans index of least significant bit and removes it from the bitboard, then returns its index
	auto popBit()
	{
		unsigned long index;
		_BitScanForward64(&index, board); // intrinsic function
		board &= (board - 1);
		return index;
	}

	void shift(int shiftAmount)
	{
		if (shiftAmount > 0)
			board = board << shiftAmount;
		else
		{
			shiftAmount = -shiftAmount;
			board = board >> shiftAmount;
		}
	}

	BitBoard shift_and_clone(int shiftAmount)
	{
		BitBoard output(shiftAmount > 0? board << shiftAmount : board >> -shiftAmount);
		return output;
	}

	// shifts entire bitboard north
	void north()
	{
		board = board << 8;
	}
	// shifts entire bitboard south
	void south()
	{
		board = board >> 8;
	}
	// shifts entire bitboard west
	void west()
	{
		board = (board >> 1) & Not_H_File;
	}
	// shifts entire bitboard east
	void east()
	{
		board = (board << 1) & Not_A_File;
	}

	void northeast() 
	{
		board = (board << 9) & Not_A_File;
	}

	void northwest()
	{
		board = (board << 7) & Not_H_File;
	}

	void southeast() 
	{
		board = (board >> 7) & Not_A_File;
	}

	void southwest() 
	{
		board = (board >> 9) & Not_H_File;
	}

	BitBoard clone()
	{
		return BitBoard(board);
	}

	//                   INDEX[RANK][FILE]
	static constexpr int INDEX[8][8] = {
	{0, 1, 2 ,3 , 4, 5, 6, 7},      // a1, b1, c1...
	{8, 9, 10, 11 ,12 ,13, 14, 15},// a2, b2, c2...
	{16, 17, 18, 19, 20 ,21 ,22, 23},
	{24, 25, 26, 27, 28, 29, 30 ,31},
	{32, 33, 34, 35, 36 ,37 ,38, 39},
	{40, 41, 42, 43, 44, 45, 46, 47},
	{48, 49, 50, 51, 52, 53, 54, 55},
	{56, 57, 58, 59, 60, 61, 62, 63}
	};

	void debug(const char* string)
	{
		/*
			REMEMBER THE FOLLOWING WHEN USING THIS FUNCTION:

			a1 is printed on the bottom left,
			h8 is printed on the top right.
		*/
		std::cout << "--- BITBOARD: " << string << " DEBUG ----\n";
		for (int i = 7; i >= 0; i--)
		{
			for (int j = 0; j < 8; j++)
			{
				std::cout << this->operator[](INDEX[i][j]) << " ";
			}
			puts("\n");
		}
		std::cout << "-------------------------------------------\n";
	}
private:
	// NO PRIVATE MEMBERS!

}; // end class BitBoard


} // end namespace chess
