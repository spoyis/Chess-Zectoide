#pragma once
#include <map>
#include "Button.h"
#include "GameState.h"
#include "PieceTextures.h"

#define HASNT_STARTED 0
#define PICKING_COLOR 1
#define PLAYING 2
#define ENDED 3
#define DIDNT_CHOOSE 0
#define CHOSE_WHITE 1
#define CHOSE_BLACK 2

class GameRenderer
{
public:
	GameRenderer(SDL_Renderer* renderer, SDL_Window* window, int width, int height) : renderer{renderer} , window{window} , textures(renderer, 0.119)
	{
		whitePiece = new Button(width / 1.25, height / 2.2, "images\\pawn_white.png", nullptr, renderer, true, 0.5);
		blackPiece = new Button(width / 4.1, height / 2.2, "images\\pawn_black.png", nullptr, renderer, true, 0.5);
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

	PieceTextures textures;
	int gameRenderState{ HASNT_STARTED };
	bool isAPieceBeingHeld{false};
	int whichPieceIsBeingHeld{false};
	std::pair<int, int> heldPieceOriginalSquare{0,0};
private:
	std::map<char, int> whichIndex;
	int boardStart{100}; // first line where the board is rendered
	int squareSize; // size of the side of each square
	bool playingAsWhite{};
	
	Button* whitePiece;
	Button* blackPiece;
	SDL_Renderer* renderer;
	SDL_Window* window;
	chess::GameState* game;
	char piece[8][8];
	void setupMap();
	void pollBoardPosition(int* x, int* y);
};

