#include "SDLHandler.h"
#include <iostream>

int SDLHandler::pollEvent()
{
    update();
    while (SDL_PollEvent(&event))
    {
        
        switch (event.type)
        {
            case SDL_QUIT:
                return 0;
            case SDL_MOUSEBUTTONDOWN:
                if (settings->cogwheel->hovered)
                {
                    settings->cogwheel->action();
                    break;
                }
                if (settings->isBeingDisplayed)
                {
                    Button* hoveredButton = settings->isAButtonBeingHovered();
                    if (hoveredButton != nullptr) hoveredButton->action();
                    break;
                }
                pollGameRendererEvents(event);
                break;
            case SDL_MOUSEBUTTONUP:
              if(game->gameRenderState == PLAYING)
                game->releasePiece();
        }
    }

    return 1;
}

void SDLHandler::pollGameRendererEvents(SDL_Event& event)
{
    switch (game->gameRenderState)
    {
        case HASNT_STARTED:
            if (play->hovered) {
                game->gameRenderState = PICKING_COLOR;
                return;
            }
        case PICKING_COLOR:
        {
            int state = game->didThePlayerChooseAColor();
            if (state != DIDNT_CHOOSE) {
                game->gameRenderState = PLAYING;
                game->setupGame(state);
            }
            return;
        }
        case PLAYING:
        {
            int x, y;
            SDL_GetMouseState(&x, &y);
            if (game->isAPieceBeingHeld)
                game->textures.renderPiece(game->whichPieceIsBeingHeld, x, y);
            else
                game->clickPiece(x, y);
            return;
        }
        case PROMOTING:
        {
            game->pollPromotion();
        }
    }
}

void SDLHandler::update()
{
    SDL_RenderClear(renderer);

    background->render();
    board->fillBoard();
    if (game->gameRenderState == HASNT_STARTED)
    {
        play->checkHover(window);
        play->Render(renderer);
    }
    else if (game->gameRenderState == PROMOTING)
    {
        settings->textureList->render();
    }

    game->render();
    settings->render();
  
    SDL_RenderPresent(renderer);
}

void SDLHandler::changeColors(SDL_Color color1, SDL_Color color2)
{
    std::cout << "changing colors "<< '\n';
    board->changeColors(color1, color2);
}


void SDLHandler::OptionsMenu::render()
{
    cogwheel->checkHover(handler->window);
    cogwheel->Render(handler->renderer);
    if (isBeingDisplayed)
    {
        background->render();

        rightArrow->checkHover(handler->window);
        rightArrow->Render(handler->renderer);

        leftArrow->checkHover(handler->window);
        leftArrow->Render(handler->renderer);

        apply->checkHover(handler->window);
        apply->Render(handler->renderer);

        textureList->render();
    }
}

Button* SDLHandler::OptionsMenu::isAButtonBeingHovered()
{
    if (leftArrow->hovered) return leftArrow;
    if (rightArrow->hovered) return rightArrow;
    if (apply->hovered) return apply;
    
    return nullptr;
}
