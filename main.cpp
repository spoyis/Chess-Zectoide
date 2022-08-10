#include "SDLHandler.h"

SDLHandler SDL;

int main(int, char**) 
{
	bool running = true;
	uint64_t start, end = 0, diff = 1000;
	while (running)
	{
		start = SDL_GetTicks();
		
		if (diff > (1000 / 165)) // 165 fps
		{
			running = SDL.pollEvent();
			end = SDL_GetTicks();
		}
		
		diff = end - start;
	}
	
	return 0;
}
