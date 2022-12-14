#pragma once
#include <iostream>

namespace chess
{
	static const enum coords {
		a1, b1, c1, d1, e1, f1, g1, h1,
		a2, b2, c2, d2, e2, f2, g2, h2,
		a3, b3, c3, d3, e3, f3, g3, h3,
		a4, b4, c4, d4, e4, f4, g4, h4,
		a5, b5, c5, d5, e5, f5, g5, h5,
		a6, b6, c6, d6, e6, f6, g6, h6,
		a7, b7, c7, d7, e7, f7, g7, h7,
		a8, b8, c8, d8, e8, f8, g8, h8
	};

	static constexpr uint64_t generateConstant(int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8)
	{
		uint64_t output = 0ULL;
		output |= (1ULL << v1) | (1ULL << v2) | (1ULL << v3) | (1ULL << v4) | (1ULL << v5) | (1ULL << v6) | (1ULL << v7) | (1ULL << v8);
		return output;
	}

	static constexpr uint64_t file[]{
		generateConstant(a1,a2,a3,a4,a5,a6,a7,a8), // a file
		generateConstant(b1,b2,b3,b4,b5,b6,b7,b8), // b file
		generateConstant(c1,c2,c3,c4,c5,c6,c7,c8), // c file 
		generateConstant(d1,d2,d3,d4,d5,d6,d7,d8), // d file
		generateConstant(e1,e2,e3,e4,e5,e6,e7,e8), // e file 
		generateConstant(f1,f2,f3,f4,f5,f6,f7,f8), // f file
		generateConstant(g1,g2,g3,g4,g5,g6,g7,g8), // g file
		generateConstant(h1,h2,h3,h4,h5,h6,h7,h8), // h file
	};

	static constexpr uint64_t rank[]{
		0x00000000000000FF, // 1st rank
		0x000000000000FF00, // 2nd rank
		0x0000000000FF0000, // 3rd rank
		0x00000000FF000000, // 4th rank
		0x000000FF00000000, // 5th rank
		0x0000FF0000000000, // 6th rank
		0x00FF000000000000, // 7th rank
		0xFF00000000000000, // 8th rank
	};

	// the empty bitmasks are due to the offset constant used in code
	// said offset is defined in BoardState.h ---> "white_pieces_offset"
	// this should be faster than having a conditional to fetch the bitmask.
	static constexpr uint64_t startingPawnRank[]{
		rank[6], // black pawns
		0,      // never used
		0,     // never used
		0,    // never used
		0,   // never used
		0,  // never used
		rank[1]  // white pawns
	};

	static constexpr uint64_t pawnMoveDirection[]{
		-8,       // black pawns go south
		0,      // never used
		0,     // never used
		0,    // never used
		0,   // never used
		0,  // never used
		8 // white pawns go north
	};

	static constexpr uint64_t pawnPromotionRank[]
	{
		rank[0],
		0,      // never used
		0,     // never used
		0,    // never used
		0,   // never used
		0,
		rank[7]
	};

	static constexpr int enemyPieceIndex[]{
		13,                 // pieces_offset +1  --> white pieces
		0,                 // never used
		0,                // never used
		0,               // never used
		0,              // never used
		0,             // never used
		12            // pieces_offset --> black pieces
	};
													 
	static constexpr uint64_t Not_A_File = file[0] ^ 0xFFFFFFFFFFFFFFFF;
	static constexpr uint64_t Not_H_File = file[7] ^ 0xFFFFFFFFFFFFFFFF;
}