#pragma once
#include "SDL.H"
#include "SDL_image.h"


class TextureList
{
public:
	TextureList(int x, int y, int listSize, const char* path, SDL_Renderer* renderer )  : n{listSize}, renderer{renderer}
	{
		loadTextures(path, n);
		int textureWidth, textureHeight;
		SDL_QueryTexture(list[0], nullptr, nullptr, &textureWidth, &textureHeight);
	
		textureRect = { x,y, textureWidth, textureHeight };
	}

	~TextureList()
	{
		for (int i = 0; i < n; i++)
			SDL_DestroyTexture(list[i]);

		delete list;
	}

	void next();
	void previous();
	
	int selectedIndex();
	void render();
private:
	int n;
	int selected{0};
	SDL_Texture** list;
	SDL_Rect textureRect;
	SDL_Renderer* renderer;

	void loadTextures(const char* path, int n);
};

