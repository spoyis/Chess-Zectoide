#pragma once

#include <SDL.h>

#define white 215,220,230,0
#define black 40,40,40,0

#define pale 180,175,165,0
#define brown 145,140,125,0

#define wheat 240,217,181,0
#define light_taupe 181,136,99,0

static constexpr SDL_Color colors[] = {white, black, pale, brown, wheat, light_taupe};

class Board
{
public:
	Board(SDL_Renderer* renderer, int width)
	{
		this->renderer = renderer;
		// default information on how to render the board.
		squareColor[0] = { white };
		squareColor[1] = { black };
		lineLength = width;
		squareSize = width / 8;
		boardStart = 100;
	}
	void fillBoard();
	void changeColors(SDL_Color color1, SDL_Color color2);

private:
	int boardStart; // first line where the board is rendered
	int squareSize; // size of the side of each square
	int lineLength;
	SDL_Color squareColor[2]; // color of the squares
	SDL_Renderer* renderer;
};

