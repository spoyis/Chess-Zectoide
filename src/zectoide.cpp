#include "../include/zectoide.h"
#include <thread>

namespace AI {

	static constexpr int promotionPieces[4]{ chess::queen, chess::rook, chess::knight, chess::bishop };

	void Zectoide::startSearch()
	{
		searchVariables.maxDepth = 1;
		
		std::thread searchThread(&Zectoide::iterativeDeepening, this);
		searchThread.detach(); // detach the thread so it runs independently
	}

	void Zectoide::iterativeDeepening()
	{
		static constexpr double maxSearchTime = 0.5;

		std::clock_t start = std::clock();
		double duration = 0;
		searchStates[0] = originalState;

		while (duration < maxSearchTime)
		{
			const auto result = search<true, true>(-DBL_MAX, DBL_MAX, 0);

			duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
			searchVariables.maxDepth++;
			std::cout << "DEPTH: " << searchVariables.maxDepth - 1 << '\n';
			std::cout << "BEST MOVE: " << searchVariables.bestMove.pieceType << " FROM " << searchVariables.bestMove.originalSquare << " TO " << searchVariables.bestMove.finalSquare << '\n';
			std::cout << "WITH EVAL: " << result << '\n';
			if (searchVariables.maxDepth == 100) break;
		}

		finishedSearching = true;
	}


	template<bool maximizingPlayer, bool rootNode>
	double Zectoide::search(double alpha, double beta, int currentDepth)
	{
		double eval = maximizingPlayer ? -DBL_MAX : DBL_MAX;

		if (currentDepth == searchVariables.maxDepth) {
			return searchVariables.maximizingWhite
				? Heuristic::eval<true >(searchStates[currentDepth])
				: Heuristic::eval<false>(searchStates[currentDepth]);
		}

		// fetch moves
		auto moves = searchStates[currentDepth].getAllMoves();

		if (rootNode) {
			// check if bestmove is set
			if (searchVariables.bestMove.pieceType != NO_CHOSEN_PIECE) {
				const auto& bestMove = searchVariables.bestMove;
				int index = 0;
				for (auto piece : moves[bestMove.pieceType]) {
					if (piece.initialSquare == bestMove.originalSquare) {
						piece.possibleMoves -= bestMove.finalSquare;

						// recurse on best move
						searchStates[1] = originalState;
						searchStates[1].make(bestMove.pieceType, bestMove.originalSquare, bestMove.finalSquare);

						if (bestMove.promotion)
							searchStates[1].promote(bestMove.promotedTo);

						auto result = search<!maximizingPlayer, false>(alpha, beta, currentDepth + 1);
						eval = alpha = result;
						break;
					}
				}
			}
		}
		// search as normal 
		for (int pieceType = 0; pieceType != chess::pawn; pieceType++) {
			for (auto piece : moves[pieceType]) {
				while (piece.possibleMoves.board) {

					const auto finalSquare = piece.possibleMoves.popBit();
					searchStates[currentDepth + 1] = searchStates[currentDepth];
					searchStates[currentDepth + 1].make(pieceType, piece.initialSquare, finalSquare);

					if (!searchStates[currentDepth + 1].wasTheLastMoveLegal())
						continue;
					
					const auto result = search<!maximizingPlayer, false>(alpha, beta, currentDepth + 1);

					if (rootNode) {
						if (result > eval){
							searchVariables.bestMove.pieceType = pieceType;
							searchVariables.bestMove.originalSquare = piece.initialSquare;
							searchVariables.bestMove.finalSquare = finalSquare;
							searchVariables.bestMove.promotion = false;
							eval = result;
						}
					}
					else  // node is NOT root
					{
						if (maximizingPlayer) {
							if (result > eval) eval = result;
							if (eval > alpha) alpha = eval;

							if (eval >= beta)
								return eval;
						}
						else {
							if (result < eval) eval = result;
							if (eval < beta) beta = eval;

							if (eval <= alpha)
								return eval;
						}
					}
				}
			}
		}

		for (auto piece : moves[chess::pawn]) {
			while (piece.possibleMoves.board) {

				const auto finalSquare = piece.possibleMoves.popBit();

				searchStates[currentDepth + 1] = searchStates[currentDepth];
				searchStates[currentDepth + 1].make(chess::pawn, piece.initialSquare, finalSquare);

				if (!searchStates[currentDepth + 1].wasTheLastMoveLegal())
					continue;

				int loopAmount = 1;

				if (searchStates[1].getPromotionState()) loopAmount = 4;

				for (long i = 1; i <= loopAmount; i++) {
					double result;

					if (loopAmount == 4) {
						searchStates[currentDepth + 1].promote(promotionPieces[i]);
						return search<!maximizingPlayer, false>(alpha, beta, currentDepth + 1);
					}
					else
						result = search<!maximizingPlayer, false>(alpha, beta, currentDepth + 1);

					if (rootNode){

						if (result > eval)
						{
							searchVariables.bestMove.pieceType = chess::pawn;
							searchVariables.bestMove.originalSquare = piece.initialSquare;
							searchVariables.bestMove.finalSquare = finalSquare;

							if (loopAmount == 4) {
								searchVariables.bestMove.promotion = true;
								searchVariables.bestMove.promotedTo = loopAmount;
							}
							else
								searchVariables.bestMove.promotion = false;

							eval = result;
						}
					}
					else
					{
						if (maximizingPlayer) {
							if (result > eval) eval = result;
							if (eval > alpha) alpha = eval;

							if (eval >= beta)
								return eval;
						}
						else {
							if (result < eval) eval = result;
							if (eval < beta) beta = eval;

							if (eval <= alpha)
								return eval;
						}
					}
				}
			}
		}

		return eval;
	}
}