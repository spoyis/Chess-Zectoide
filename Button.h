#pragma once
#include <iostream>
#include "SDL.h"
#include <SDL_image.h>

using FunctionPtr = void(*)();
#define DO_NOT_CENTER false

class Button
{
public:
	Button(int x, int y, const char* texturePath, FunctionPtr func, SDL_Renderer* renderer, bool centered = true, double scale = 1.0) : x{x}, y{y}, action{func}
	{
		texture1 = IMG_LoadTexture(renderer, texturePath); // texture for unselected state
		texture2 = IMG_LoadTexture(renderer, getSecondTexturePath(texturePath)); // texture for selected state

		// query texture size, saves me time typing the size every time.
		int textureWidth, textureHeight; 
		SDL_QueryTexture(texture1, nullptr, nullptr, &textureWidth, &textureHeight);

		// changes rendering coordinates to render from the center of the image
		if (centered)
		{
			x -= (int)((textureWidth/2) * scale);
			y -= (int)((textureHeight/2)* scale);
		}

		// builds the SLD_Rect structure for the texture
		textureRect = {x,y, (int)(textureWidth * scale), (int)(textureHeight * scale)};
		// isn't hovered by default
		selectedTexture = texture1;
		hovered = false;
	}

	~Button()
	{
		SDL_DestroyTexture(texture1);
		SDL_DestroyTexture(texture2);
	}

	void Render(SDL_Renderer* renderer); // gives button texture to renderer
	void checkHover(SDL_Window* window); // checks if the cursor is hovering over the button
	SDL_Rect textureRect;
	bool hovered;
	FunctionPtr action;

private:
	int x;
	int y;
	SDL_Texture* texture1;
	SDL_Texture* texture2;
	SDL_Texture* selectedTexture;
	const char* getSecondTexturePath(const char* path);
};

