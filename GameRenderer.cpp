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
		int y = playingAsWhite ? boardStart : boardStart + squareSize * 7;
		for (int i = 0; i < 8; i++)
		{
			int x = playingAsWhite ? 0 : squareSize * 7;
			for (int j = 0; j < 8; j++)
			{
				int index = whichIndex[piece[i][j]];
				if (index != -1)
					textures.renderPiece(index, x, y);
				
				x = playingAsWhite? (x + squareSize) : (x - squareSize);
			}
			y = playingAsWhite ? (y + squareSize) : (y - squareSize);
		}

		if (isAPieceBeingHeld)
		{
			int x, y;
			// render legal move squares
			chess::BitBoard legalMovesCopy = legalMoves;
			std::cout << "SUS\n";
			while (legalMovesCopy.board)
			{
				auto move = legalMovesCopy.popBit();
				x = move / 8;
				y = move % 8;
				std::cout << "move " << move << " " << x << " " << y << '\n';
			}
			
			SDL_GetMouseState(&x, &y);
			textures.renderPiece(whichPieceIsBeingHeld, x - squareSize/2, y - squareSize/2);
		}
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
	std::cout << "selected " << x << " " << y << '\n';
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
		if (true) // TODO: SUBSTITUTE THIS FOR LEGAL MOVE CHECK
		{
			SDL_GetMouseState(&x, &y);
			pollBoardPosition(&x, &y);
			piece[y][x] = pieceChar[whichPieceIsBeingHeld];
		}
		else
			piece[heldPieceOriginalSquare.first][heldPieceOriginalSquare.second] = pieceChar[whichPieceIsBeingHeld];
	}
}

void GameRenderer::setupGame(int color)
{
	if(color == CHOSE_WHITE) playingAsWhite = true;
	if(color == CHOSE_BLACK) playingAsWhite = false;

	game = new chess::GameState();

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
	std::cout << "polling " << *x << " " << *y << '\n';
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
	if (!playingAsWhite) 
	{
		*x = abs(*x - 7);
		*y = abs(*y - 7);
	}
}

chess::BitBoard GameRenderer::getLegalMoves(int piece, int x, int y)
{
	if (!game->turnQuery(playingAsWhite)) 
		return chess::BitBoard(); // not your turn lol, returns empty Bitboard

	if (piece > chess::white_pieces_offset)
		piece -= chess::white_pieces_offset;
	chess::MoveList moves;
	switch (piece)
	{
	case chess::pawn:
		moves = game->generatePawnMoves();
		break;
	default:
		break;
	}

	y = abs(y - 7);
	int index = x + (8 * y);
	for (const auto& move : moves)
	{
		if (index == move.initialSquare) return move.possibleMoves;
	}

	return chess::BitBoard(); // returns empty BitBoard
}
