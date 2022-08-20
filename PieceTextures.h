#pragma once
#include "SDL.h"
#include "SDL_image.h"

#define LEGAL_MOVE_INDICATOR 12
class PieceTextures
{
public:
	PieceTextures(SDL_Renderer* renderer, double scale) : renderer{renderer}
	{
		loadPieces("_white.png", 11, 0);
		loadPieces("_black.png", 11, 6);
		piece[LEGAL_MOVE_INDICATOR] = IMG_LoadTexture(renderer, "images\\legal_move_indicator.png");
		int textureWidth, textureHeight;
		SDL_QueryTexture(piece[0], nullptr, nullptr, &textureWidth, &textureHeight);
		pieceRect = {0,0, (int)(textureWidth * scale), (int)(textureHeight * scale)};
	}

	~PieceTextures()
	{
		for (int i = 0; i < 13; i++)
			SDL_DestroyTexture(piece[i]);
	}

	void renderPiece(int pieceId, int x, int y);
	
private:
	SDL_Renderer* renderer;
	SDL_Texture* piece[13];
	SDL_Rect pieceRect;
	void loadPieces(const char* suffix, int suffixSize, int start);
};

