#include "../include/SDLHandler.h"

/* THIS FILE CONTAINS: FUNCTIONALITIES FOR THE BUTTON INSTANCES USED IN THE GUI */

extern SDLHandler SDL; // declared in main as global variable

// Manages the menu window
void cogwheelFunction()
{
	SDL.settings->isBeingDisplayed = !SDL.settings->isBeingDisplayed; 
}

// left arrow on options menu, to pick the board color
void optionsLeft()
{
	SDL.settings->textureList->next();
}

// right arrow on options menu, to pick the board color
void optionsRight()
{
	SDL.settings->textureList->previous();
}

// applies the color change on the options menu
void applyColors()
{
	int n = SDL.settings->textureList->selectedIndex();
	SDL.changeColors(colors[(n * 2)], colors[(n * 2) + 1]); // constexpr "colors" declared in Board.h
}