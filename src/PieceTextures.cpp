#include "../include/PieceTextures.h"
#include <iostream>

void PieceTextures::renderPiece(int pieceId, int x, int y)
{
	//std::cout << "aqui " << pieceId << " " << x << " " << y <<  "\n";
	pieceRect.x = x;
	pieceRect.y = y;
	SDL_RenderCopy(renderer, piece[pieceId], NULL, &pieceRect);
}

void PieceTextures::loadPieces(const char* suffix, int suffixSize, int start)
{
	static constexpr const char* path = "images\\";
	static constexpr int pathSize = 7; 
	static constexpr const char* pieceStr[] = { "king", "queen", "rook", "bishop" , "knight", "pawn" };
	static constexpr int pieceStrSize[] = { 4, 5, 4, 6, 6, 4 };

	for (int i = 0; i < 6; i++)
	{
		char* piecePath = new char[pathSize + pieceStrSize[i] + suffixSize];
		int j;
		for (j = 0; j < pathSize; j++)
			piecePath[j] = path[j];

		for (int k = 0; k < pieceStrSize[i]; k++)
			piecePath[j++] = pieceStr[i][k];

		for (int k = 0; k < suffixSize; k++)
			piecePath[j++] = suffix[k];

		piece[i + start] = IMG_LoadTexture(renderer, piecePath);
		delete[] piecePath;

		//std::cout << SDL_GetError() << '\n';
	}
}