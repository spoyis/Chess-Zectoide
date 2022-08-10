#include "SDL.h"
#include "SDL_image.h"
#pragma once
// background class to make things prettier

#define SCROLLING true
#define NO_SCROLLING false
class Background
{
public:
	Background(SDL_Renderer* renderer, const char* imagePath, int screenWidth, bool scroll, double scale = 1.0) : renderer{renderer} , screenWidth{screenWidth}, scroll{scroll}
	{
		texture = IMG_LoadTexture(renderer, imagePath);

		int textureHeight;
		SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);
		offset = 1;
		tries = 20;
		textureWidth *= scale;
		textureHeight *= scale;
		textureRect = {0 , 0, textureWidth, textureHeight };
	}

	~Background()
	{
		SDL_DestroyTexture(texture);
	}

	void render();
	void changeOrigin(int x, int y);
	

private:
	SDL_Texture* texture;
	SDL_Renderer* renderer;
	SDL_Rect textureRect;
	int textureWidth;
	int screenWidth;
	int offset;
	int tries;
	bool scroll;
	void update();
};

