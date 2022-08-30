#include "GameRenderer.h"

static constexpr char pieceChar[] = { 'k', 'q', 'r', 'b', 'n', 'p', 'K', 'Q', 'R', 'B', 'N', 'P' };
void GameRenderer::render()
{
	
	if (gameRenderState == HASNT_STARTED) return;
	if (gameRenderState == PICKING_COLOR)
	{
		whitePiece->checkHover(window);
		whitePiece->Render(renderer);
		blackPiece->checkHover(window);
		blackPiece->Render(renderer);
	}
	if (gameRenderState == PLAYING)
	{
		int y = playingAsWhite ? boardStart + squareSize * 7: boardStart;
		for (int i = 0; i < 8; i++)
		{
			int x = playingAsWhite ?  0: squareSize * 7;
			for (int j = 0; j < 8; j++)
			{
				int index = whichIndex[piece[i][j]];
				if (index != -1)
					textures.renderPiece(index, x, y);
				
				x = playingAsWhite? (x + squareSize) : (x - squareSize);
			}
			y = playingAsWhite ? (y - squareSize) : (y + squareSize);
		}

		if (isAPieceBeingHeld)
		{
			int x, y;
			// render legal move squares
			chess::BitBoard legalMovesCopy = legalMoves;

			while (legalMovesCopy.board)
			{
				auto move = legalMovesCopy.popBit();
				x = playingAsWhite ? 
					(move % 8) * squareSize :
					abs((int)(move % 8) - 7) * squareSize;
				y = playingAsWhite ?
					boardStart + abs((long)(move / 8UL) - 7L) * squareSize:
					boardStart + move / 8UL * squareSize;
				
				textures.renderPiece(LEGAL_MOVE_INDICATOR, x , y);
				//std::cout << "move " << move << "X= " << x << " Y=" << y << " " <<SDL_GetError() << '\n';
				//std::cout << "held piece: " << whichPieceIsBeingHeld << '\n';
			}
			
			SDL_GetMouseState(&x, &y);
			textures.renderPiece(whichPieceIsBeingHeld, x - squareSize/2, y - squareSize/2);
		}

		if (game->getPromotionState())
		{
			gameRenderState = PROMOTING;
		}

		return;
	}
	if (gameRenderState == PROMOTING)
	{
		auto x = width / 2.7, y = height / 3.0;

		// slightly different logic for choosing piece colors when promoting
		// since the gamestate object will already have switched the offset
		// this macro just makes the code look less ugly.
        #define PROMOTING_COLOR - game->getBoardStateOffset() + chess::white_pieces_offset

		textures.renderPiece(chess::queen  PROMOTING_COLOR, x, y);
		textures.renderPiece(chess::rook   PROMOTING_COLOR, x + squareSize, y);
		textures.renderPiece(chess::knight PROMOTING_COLOR, x, y + squareSize);
		textures.renderPiece(chess::bishop PROMOTING_COLOR, x + squareSize, y + squareSize);
	}
}

int GameRenderer::didThePlayerChooseAColor()
{
	if (whitePiece->hovered) return CHOSE_WHITE;
	if (blackPiece->hovered) return CHOSE_BLACK;

	return DIDNT_CHOOSE;
}

void GameRenderer::clickPiece(int x, int y)
{
	pollBoardPosition(&x, &y);
	if (x == -1 or y == -1) return; // PIECE OUT OF BOUNDS
	std::cout << "selected " << x << " " << y << "LOLO "<< piece[y][x]<< '\n';
	int selectedPiece = whichIndex[piece[y][x]];
	if (selectedPiece != -1)
	{
		legalMoves = getLegalMoves(selectedPiece, x, y);
		if (legalMoves.board)
		{
			isAPieceBeingHeld = true;
			whichPieceIsBeingHeld = selectedPiece;
			heldPieceOriginalSquare = { y,x };
			piece[y][x] = '0';
		}
	}
}

void GameRenderer::releasePiece()
{
	if (isAPieceBeingHeld)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		pollBoardPosition(&x, &y);
		if (x == -1 or y == -1) return; // PIECE OUT OF BOUNDS
		std::cout << "released at " << x << " " << y << '\n';
		isAPieceBeingHeld = false;
		bool legalMove{false};

		chess::BitBoard legalMovesCopy = legalMoves;
		while (legalMovesCopy.board)
		{
			int X, Y;
			auto move = legalMovesCopy.popBit();
			X = (move % 8);
			Y =  move / 8UL;

			std::cout << "actual coords" << X << " " << Y << "| DROPPED AT " << x << y << '\n';
			if (x == X and y == Y)
			{
				piece[y][x] = pieceChar[whichPieceIsBeingHeld];
				auto OriginalSquare = heldPieceOriginalSquare.first * 8 + heldPieceOriginalSquare.second;
				game->make(whichPieceIsBeingHeld - game->getBoardStateOffset(), OriginalSquare, move);
				legalMove = true;
				updatePieceMatrix();
				break;
			}
		}
		if(!legalMove)
			piece[heldPieceOriginalSquare.first][heldPieceOriginalSquare.second] = pieceChar[whichPieceIsBeingHeld];
	}
}

void GameRenderer::setupGame(int color)
{
	if(color == CHOSE_WHITE) playingAsWhite = true;
	if(color == CHOSE_BLACK) playingAsWhite = false;

	game = new chess::GameState();

	updatePieceMatrix();
}

void GameRenderer::updatePieceMatrix()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			piece[i][j] = '0';
			for (int k = 0; k < chess::pieces_offset; k++)
			{
				if (game->boardState.board[k][chess::BitBoard::INDEX[i][j]])
				{
					piece[i][j] = pieceChar[k];
					break;
				}
			}
			printf("%c", piece[i][j]);
		}
		puts("\n");
	}
}

void GameRenderer::setupMap()
{
	whichIndex['p'] = chess::pawn;
	whichIndex['k'] = chess::king;
	whichIndex['r'] = chess::rook;
	whichIndex['q'] = chess::queen;
	whichIndex['b'] = chess::bishop;
	whichIndex['n'] = chess::knight;

	whichIndex['P'] = chess::pawn + chess::white_pieces_offset;
	whichIndex['K'] = chess::king + chess::white_pieces_offset;
	whichIndex['R'] = chess::rook + chess::white_pieces_offset;
	whichIndex['Q'] = chess::queen + chess::white_pieces_offset;
	whichIndex['B'] = chess::bishop + chess::white_pieces_offset;
	whichIndex['N'] = chess::knight + chess::white_pieces_offset;

	whichIndex['0'] = -1;
}

void GameRenderer::pollBoardPosition(int* x, int* y)
{
	if (*y < boardStart or *y > boardStart + squareSize * 8)
	{
		*y = -1;
		return;
	}
	*y -= boardStart;

	*y /= squareSize;
	*x /= squareSize;

	// if playing as black, the pieces aren't displayed on the same logical squares as saved in memory.
	// in fact, they're mirrored.
	// this condition checks for that and mirrors the click data for proper gameplay.
	if (playingAsWhite) 
	{
		*y = abs(*y - 7);
	}
	else
		*x = abs(*x - 7);
}

chess::BitBoard GameRenderer::getLegalMoves(int piece, int x, int y)
{
	// TODO: UNCOMMENT THIS CODE WHEN ACTUALLY PLAYING VS ENGINE
	//if (!game->turnQuery(playingAsWhite)) 
		//return chess::BitBoard(); // not your turn lol, returns empty Bitboard

	if (piece > chess::white_pieces_offset)
		piece -= chess::white_pieces_offset;
	chess::MoveList moves;
	switch (piece)
	{
	case chess::pawn:
		moves = game->generatePawnMoves();
		break;
	case chess::rook:
		moves = game->generateRookMoves();
		break;
	default:
		break;
	}

	int index = x + (8 * y);
	std::cout << "THIS WAS EVALUATED AT " << x << " " << y << " == " << index << '\n';
	for (const auto& move : moves)
	{
		if (index == move.initialSquare) return move.possibleMoves;
	}

	return chess::BitBoard(); // returns empty BitBoard
}


void GameRenderer::pollPromotion()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	// top left of squares
	auto upperBoundX = width / 2.7, upperBoundY = height / 3.0; 

	// bottom right bounds
	auto lowerBoundX = upperBoundX + squareSize * 2; 
	auto lowerBoundY = upperBoundY + squareSize * 2; 

	if (x >= upperBoundX and x <= lowerBoundX and y >= upperBoundY and y <= lowerBoundY)
	{
		// visual promotion matrix as shown in the gui
		static constexpr int promotionMatrix[2][2]
		{
			{chess::queen, chess::rook},
			{chess::knight, chess::bishop}
		};
		
		// calculates index for promotionMatrix
		x = (x - upperBoundX) / squareSize;
		y = (y - upperBoundY) / squareSize;

		game->promote(promotionMatrix[y][x]);
		updatePieceMatrix();
		gameRenderState = PLAYING;
	}
	
	
}