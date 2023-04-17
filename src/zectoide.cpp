#include "../include/zectoide.h"
#include <thread>

namespace AI {

	static constexpr int promotionPieces[4]  { chess::queen, chess::rook, chess::knight, chess::bishop };

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

		while (duration < maxSearchTime) 
		{
			search();

			duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
			searchVariables.maxDepth++;
			std::cout << "DEPTH: " << searchVariables.maxDepth - 1 << '\n';
			std::cout << "BEST MOVE: " << searchVariables.bestMove.pieceType << " FROM " << searchVariables.bestMove.originalSquare << " TO " << searchVariables.bestMove.finalSquare << '\n';
			if (searchVariables.maxDepth == 100) break;
		}

		finishedSearching = true;
	}

	void Zectoide::search() 
	{
		searchVariables.currentDepth = 0;
		double eval = -DBL_MAX;

		// instantiate alpha/beta
		double alpha = -DBL_MAX;
		double beta = DBL_MAX;

		// fetch moves
		auto moves = originalState.getAllMoves();
		searchStates[0] = chess::GameState(originalState);

		// check if bestmove is set
		if (searchVariables.bestMove.pieceType != NO_CHOSEN_PIECE) {
			const auto& bestMove  = searchVariables.bestMove;
			int index = 0;
			for (auto piece : moves[bestMove.pieceType]) {
				if (piece.initialSquare == bestMove.originalSquare) {
					piece.possibleMoves -= bestMove.finalSquare;

					// recurse on best move
					searchStates[1] = originalState;
					searchStates[1].make(bestMove.pieceType ,bestMove.originalSquare, bestMove.finalSquare);

					if (bestMove.promotion)
						searchStates[1].promote(bestMove.promotedTo);

					auto result = search<MINIMIZNG_PLAYER>(alpha, beta);
					eval = alpha = result;
					break;
				}
			}
		}

		for (int pieceType = 0; pieceType != chess::pawn; pieceType++) {
			for (auto piece : moves[pieceType]){
				while (piece.possibleMoves.board){
					auto finalSquare = piece.possibleMoves.popBit();
					auto result = recurse<MINIMIZNG_PLAYER>(alpha, beta, eval, pieceType, piece.initialSquare, finalSquare);
					--searchVariables.currentDepth;

					if (result > eval)
					{
						searchVariables.bestMove.pieceType = pieceType;
						searchVariables.bestMove.originalSquare = piece.initialSquare;
						searchVariables.bestMove.finalSquare = finalSquare;
						searchVariables.bestMove.promotion = false;
						eval = result;
					}
				
					alpha = std::max(alpha, result);
				}
			}
		}
		
		for (auto piece : moves[chess::pawn]) {
			while (piece.possibleMoves.board) {
				auto finalSquare = piece.possibleMoves.popBit();
				recursePawn<MINIMIZNG_PLAYER>(alpha, beta, eval, chess::pawn, piece.initialSquare, finalSquare);
				int loopAmount = 1;

				if (searchStates[1].getPromotionState()) loopAmount = 4;

				for (long i = 1; i <= loopAmount; i++) {
					double result;

					if (loopAmount == 4)
						result = promote<MINIMIZNG_PLAYER>(alpha, beta, eval, chess::pawn, piece.initialSquare, finalSquare, i - 1);
					else
						result = search<MINIMIZNG_PLAYER>(alpha, beta);

					--searchVariables.currentDepth;
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


					alpha = std::max(alpha, result);
				}
			}
		}
	}

	template<bool maximizingPlayer>
	double Zectoide::search(double alpha, double beta) 
	{
		double eval = maximizingPlayer ? -DBL_MAX : DBL_MAX;

		auto boardLegality = searchStates[searchVariables.currentDepth].wasTheLastMoveLegal();
		if (!boardLegality) {
			//std::cout << "ILLEGAL BOARD ACHIEVED\n";
			return -eval;
		}

		if (searchVariables.currentDepth == searchVariables.maxDepth) {
			return searchVariables.maximizingWhite
				? Heuristic::eval<true >(searchStates[searchVariables.currentDepth])
				: Heuristic::eval<false>(searchStates[searchVariables.currentDepth]);
		}

		// fetch moves
		auto moves = searchStates[searchVariables.currentDepth].getAllMoves();

		// search as normal 
		for (int pieceType = 0; pieceType != chess::pawn; pieceType++) {
			for (auto piece : moves[pieceType]) {
				while (piece.possibleMoves.board) {
					auto result = recurse<!maximizingPlayer>(alpha, beta, eval, pieceType, piece.initialSquare, piece.possibleMoves.popBit());
					searchVariables.currentDepth--;

					if (maximizingPlayer)
					{
						if (result > eval) eval = result;
						if (eval > alpha) alpha = eval;

						if (alpha >= beta) {
							return alpha;
						}
					}
					else
					{
						if (result < eval) eval = result;
						if (eval < beta) beta = eval;

						if (alpha >= beta)
							return beta;

					}
				}
			}
		}

		for (auto piece : moves[chess::pawn]) {
			while (piece.possibleMoves.board) {
				auto finalSquare = piece.possibleMoves.popBit();
				recursePawn<!maximizingPlayer>(alpha, beta, eval, chess::pawn, piece.initialSquare, finalSquare);
				int loopAmount = 1;

				if (searchStates[1].getPromotionState()) loopAmount = 4;

				for (long i = 1; i <= loopAmount; i++) {
					double result;

					if (loopAmount == 4)
						result = promote<!maximizingPlayer>(alpha, beta, eval, chess::pawn, piece.initialSquare, finalSquare, i - 1);
					else
						result = search<!maximizingPlayer>(alpha, beta);

					--searchVariables.currentDepth;

					if (maximizingPlayer)
					{
						if (result > eval) eval = result;
						if (eval > alpha) alpha = eval;

						if (alpha >= beta)
							return alpha;
					}
					else
					{
						if (result < eval) eval = result;
						if (eval < beta) beta = eval;

						if (alpha >= beta)
							return beta;
					}
				}
			}
		}

		return eval;
	}

	template<bool maximizingPlayer>
	inline double Zectoide::recurse(double& alpha, double& beta, double& eval, int pieceID, unsigned long originalSquare, unsigned long finalSquare)
	{
		const auto currentDepth = ++searchVariables.currentDepth;
		searchStates[currentDepth] = searchStates[currentDepth - 1];
		searchStates[currentDepth].make(pieceID, originalSquare, finalSquare);

		return search<maximizingPlayer>(alpha, beta);
	}

	template<bool maximizingPlayer>
	inline void Zectoide::recursePawn(double& alpha, double& beta, double& eval, int pieceID, unsigned long originalSquare, unsigned long finalSquare)
	{
		const auto currentDepth = ++searchVariables.currentDepth;
		searchStates[currentDepth] = searchStates[currentDepth - 1];
		searchStates[currentDepth].make(pieceID, originalSquare, finalSquare);

	}

	template<bool maximizingPlayer>
	inline double Zectoide::promote(double& alpha, double& beta, double& eval, int pieceID, unsigned long originalSquare, unsigned long finalSquare, int promotionIndex)
	{
		const auto currentDepth = searchVariables.currentDepth;
		searchStates[currentDepth].promote(promotionPieces[promotionIndex]);
		
		return search<maximizingPlayer>(alpha, beta);
	}
}
