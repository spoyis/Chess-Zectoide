#pragma once

#include "GameState.h"

namespace AI
{ // begin namespace zectoide
	static constexpr bool MINIMIZNG_PLAYER = false;

	class Zectoide {

	public:
		Zectoide(chess::GameState originalState, bool maximizingWhite) : searchStates{}, searchVariables{maximizingWhite} 
		{
			this->originalState = chess::GameState(originalState);
		};
		void startSearch();

	private:
		static constexpr int NO_CHOSEN_PIECE = -1;
		struct BestMove {
			int pieceType{ NO_CHOSEN_PIECE };
			int originalSquare{0};
			int finalSquare{0};
			bool promotion{false};
			int promotedTo{ NO_CHOSEN_PIECE };

			BestMove() = default;
		};

		struct SearchVariables {
			bool maximizingWhite;
			int currentDepth{ 1 };
			int maxDepth{ 1 };
			BestMove bestMove;
		};

		chess::GameState originalState;
		chess::GameState searchStates[100];
		void iterativeDeepening();
		SearchVariables searchVariables;
		
		// depth 0 methods
		void search();

		// depth > 0 methods
		template <bool maximizingPlayer>
		double search(double alpha, double beta);

		template <bool maximizingPlayer>
		inline double recurse(double& alpha, double& beta, double& eval, int pieceID, unsigned long originalSquare, unsigned long finalSquare);

		template <bool maximizingPlayer>
		inline void recursePawn(double& alpha, double& beta, double& eval, int pieceID, unsigned long originalSquare, unsigned long finalSquare);

		template <bool maximizingPlayer>
		inline double promote(double& alpha, double& beta, double& eval, int pieceID, unsigned long originalSquare, unsigned long finalSquare, int promotionIndex);
	};

} // end namespace zectoide