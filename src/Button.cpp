#include "Button.h"
#include <iostream>

void Button::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, selectedTexture, NULL, &textureRect);
}

void Button::checkHover(SDL_Window* window)
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	SDL_Rect mousePos{x, y, 1, 1};
	//std::cout << "mousePos == " << mousePos.x << mousePos.y << '\n';
	if (SDL_HasIntersection(&textureRect, &mousePos))
	{
		hovered = true;
		selectedTexture = texture2;
	}
	else
	{
		hovered = false;
		selectedTexture = texture1;
	}
}

const char* Button::getSecondTexturePath(const char* path)
{
	const char* suffix = "_selected.png";
	const int suffixSize = 14;

	int pathSize = strlen(path);
	int strSize = pathSize + suffixSize + 1;
	char* output = new char[strSize];

	int i;
	for (i = 0; i < pathSize - 4; i++)
		output[i] = path[i];

	for (int j = 0; i < strSize; i++, j++)
		output[i] = suffix[j];

	output[strSize - 1] = '\0';

	return output;
}
