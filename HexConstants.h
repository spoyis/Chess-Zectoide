#pragma once
#include <iostream>

namespace chess
{
	static constexpr uint64_t file[]{
		0x0101010101010101, // a file
		0x0202020202020202, // b file
		0x0303030303030303, // c file 
		0x0404040404040404, // d file
		0x0505050505050505, // e file 
		0x0606060606060606, // f file
		0x0707070707070707, // g file
		0x0808080808080808, // h file
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


}