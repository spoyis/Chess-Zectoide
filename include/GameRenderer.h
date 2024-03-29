#pragma once
#include <map>
#include "Button.h"
#include "GameState.h"
#include "PieceTextures.h"
#include "zectoide.h"

#define HASNT_STARTED 0
#define PICKING_COLOR 1
#define PLAYING 2
#define ENDED 3
#define PROMOTING 4
#define DIDNT_CHOOSE 0
#define CHOSE_WHITE 1
#define CHOSE_BLACK 2

class GameRenderer
{
public:
	GameRenderer(SDL_Renderer* renderer, SDL_Window* window, int width, int height) : renderer{renderer} , window{window} , textures(renderer, 0.119), width{width}, height{height}
	{
		whitePiece = new Button(width / 1.25, height / 2.2, "images\\pawn_white.png", nullptr, renderer, true, 0.5);
		blackPiece = new Button(width / 4.1, height / 2.2, "images\\pawn_black.png", nullptr, renderer, true, 0.5);
		////handler->currentWidth / 2.7, handler->currentHeight / 3
		squareSize = width / 8;
		setupMap();
	}

	~GameRenderer()
	{
		delete whitePiece;
		delete blackPiece;
	}

	void render();
	int didThePlayerChooseAColor();
	void clickPiece(int x, int y);
	void releasePiece();
	void setupGame(int color);
	void pollPromotion();
	void waitZectoide();

	PieceTextures textures;
	int gameRenderState{ HASNT_STARTED };
	bool isAPieceBeingHeld{false};
	bool waitingForZectoide{ false };
	int whichPieceIsBeingHeld{false};
	std::pair<int, int> heldPieceOriginalSquare{0,0};

	auto getLastMoveBegin() { return lastMoveBeginSquare; };
	auto getLastMoveEnd() { return lastMoveEndSquare; };
private:
	std::map<char, int> whichIndex;
	int boardStart{100}; // first line where the board is rendered
	int squareSize; // size of the side of each square
	bool playingAsWhite{};
	int width, height;
	
	Button* whitePiece;
	Button* blackPiece;
	SDL_Renderer* renderer;
	SDL_Window* window;
	chess::GameState* game;
	char piece[8][8];
	void updatePieceMatrix();
	void setupMap();
	void saveLastMoveSquares(int begin, int end);
	void pollBoardPosition(int* x, int* y);
	chess::BitBoard getLegalMoves(int piece, int x, int y);
	chess::BitBoard legalMoves;
	AI::Zectoide* zectoide { nullptr };
	std::pair<int, int> lastMoveBeginSquare{ -1, -1 };
	std::pair<int, int> lastMoveEndSquare{ -1, -1 };
};

