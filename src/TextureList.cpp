#include "../include/TextureList.h"
#include <iostream>


void TextureList::loadTextures(const char* path, int n)
{
	list = new SDL_Texture*[n];

	for (int i = 0; i < n; i++)
	{
		const char* suffix = "_x.png";
		const int suffixSize = 7;

		int pathSize = strlen(path);
		char* output = new char[pathSize + suffixSize];
		int j;
		for (j = 0; j < pathSize; j++)
			output[j] = path[j];

		for (int k = 0; k < suffixSize; k++)
			output[j++] = suffix[k] != 'x' ? suffix[k] : '1' + i;

		list[i] = IMG_LoadTexture(renderer, output);
		std::cout << "loaded " << output << '\n';
		delete[] output;
	}
}

void TextureList::next()
{
	selected = (selected + 1) % n;
}

void TextureList::previous()
{
	selected = selected - 1;
	if (selected < 0) selected = n - 1;
}

int TextureList::selectedIndex()
{
	return selected;
}

void TextureList::render()
{
	SDL_RenderCopy(renderer, list[selected], NULL, &textureRect);
}

