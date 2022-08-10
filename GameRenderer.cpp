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
		isAPieceBeingHeld = true;
		whichPieceIsBeingHeld = selectedPiece;
		heldPieceOriginalSquare = {y,x};
		piece[y][x] = '0';
	}
}

void GameRenderer::releasePiece()
{
	std::cout << "hmm\n";
	if (isAPieceBeingHeld)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		pollBoardPosition(&x, &y);
		if (x == -1 or y == -1) return; // PIECE OUT OF BOUNDS
		std::cout << "released at " << x << " " << y << '\n';
		isAPieceBeingHeld = false;
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
				if (game->boardState.board[k][chess::INDEX[i][j]])
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

	if (!playingAsWhite)
		*y = abs(*y - 7);
}
