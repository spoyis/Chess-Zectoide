#pragma once

#include "GameState.h"
#include "../include/heuristics.h"

namespace AI
{ // begin namespace zectoide
	static constexpr bool MINIMIZNG_PLAYER = false;
	static constexpr bool MAXIMIZING_PLAYER = true;

	class Zectoide {

	public:

		Zectoide(chess::GameState originalState, bool maximizingWhite) : searchStates{}, searchVariables{maximizingWhite} 
		{
			this->originalState = chess::GameState(originalState);
		};
		void startSearch();
		bool stoppedSearching() { return finishedSearching; }
		
		struct BestMove {
			int pieceType{ NO_CHOSEN_PIECE };
			int originalSquare{ 0 };
			int finalSquare{ 0 };
			bool promotion{ false };
			int promotedTo{ NO_CHOSEN_PIECE };

			BestMove() = default;
		};

		BestMove getMove() { return searchVariables.bestMove; }

	private:
		static constexpr int NO_CHOSEN_PIECE = -1;
		
		struct SearchVariables {
			bool maximizingWhite;
			int maxDepth{ 1 };
			BestMove bestMove;
		};

		chess::GameState originalState;
		chess::GameState searchStates[100];
		void iterativeDeepening();
		SearchVariables searchVariables;
		bool finishedSearching{ false };
		
		template <bool maximizingPlayer, bool rootNode>
		double search(double alpha, double beta, int currentDepth);
	};

} // end namespace zectoide