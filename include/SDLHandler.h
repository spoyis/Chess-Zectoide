#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "Board.h"
#include "Button.h"
#include "Background.h"
#include "ButtonFunctions.h"
#include "TextureList.h"
#include "GameRenderer.h"

constexpr auto defaultHeight = 1012;
constexpr auto defaultWidth = 760;

class SDLHandler
{
public:
	SDLHandler() : event()
	{
		SDL_Init(SDL_INIT_VIDEO);
		IMG_Init(IMG_INIT_PNG);
		window = SDL_CreateWindow("TESTE", 20, 20, defaultWidth, defaultHeight, SDL_WINDOW_OPENGL);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		
		board = new Board(renderer, defaultWidth);
		play = new Button(currentWidth / 2, currentHeight / 2, "images\\play.png", nullptr, renderer);
		background = new Background(renderer, "images\\background.png", currentWidth, SCROLLING);
		settings = new OptionsMenu(this);

		game = new GameRenderer(renderer, window, currentWidth, currentHeight);
	}

	~SDLHandler()
	{
		delete board;
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	int pollEvent();
	void pollGameRendererEvents(SDL_Event& event);
	void update();
	void changeColors(SDL_Color color1, SDL_Color color2);
	class OptionsMenu; // forward declaration
	OptionsMenu* settings;

private:
	int currentHeight{ defaultHeight };
	int currentWidth{ defaultWidth };
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
	Board* board;
	Background* background;
	Button* play;
	GameRenderer* game;
	
	class OptionsMenu // begin class OptionsMenu
	{
	public:
		OptionsMenu(SDLHandler* handler) : handler{ handler }
		{
			cogwheel = new Button(handler->currentWidth - 100, 0, "images\\cogwheel.png", &cogwheelFunction, handler->renderer, false, 0.2);
			
			leftArrow = new Button(handler->currentWidth / 4.6, handler->currentHeight / 2.65, "images\\left_arrow.png", &optionsLeft, handler->renderer, false, 0.08);
			rightArrow = new Button(handler->currentWidth / 1.55, handler->currentHeight / 2.65, "images\\right_arrow.png", &optionsRight, handler->renderer, false, 0.08);
			apply = new Button(handler->currentWidth / 2, handler->currentHeight / 1.7, "images\\checkmark.png", &applyColors, handler->renderer, true, 0.25);

			textureList = new TextureList(handler->currentWidth / 2.7, handler->currentHeight / 3 , 3, "images\\tile", handler->renderer);
			
			background = new Background(handler->renderer, "images\\menuBackground.png", handler->currentWidth, NO_SCROLLING, 0.5);
			background->changeOrigin(handler->currentWidth / 6, handler->currentHeight / 6);
		}

		~OptionsMenu()
		{
			delete cogwheel;
			delete background;
			delete leftArrow;
			delete rightArrow;
			delete textureList;
		}

		bool isBeingDisplayed{false};
		void render();
		Button* cogwheel;
		TextureList* textureList;
		Button* isAButtonBeingHovered();
	private:
		SDLHandler* handler;
		Background* background;
		Button* leftArrow;
		Button* rightArrow;
		Button* apply;
	}; // end class OptionsMenu
	
}; // end class SDLHandler


