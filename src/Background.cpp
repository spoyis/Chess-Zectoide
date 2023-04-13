#include "../include/Background.h"

void Background::render()
{
	if (scroll) update();
	SDL_RenderCopy(renderer, texture, NULL, &textureRect);
}

void Background::update()
{
	static int counter = 0;
	// checks if the texture can be offset before reaching its border
	if (textureWidth + textureRect.x - offset >= (screenWidth) and counter == tries)
	{
		textureRect.x -= offset;
		counter = 0;
	}
	else if (counter == tries)
		textureRect.x = 0;
	else
		counter++;
}

void Background::changeOrigin(int x, int y)
{
	textureRect.x = x;
	textureRect.y = y;
}
