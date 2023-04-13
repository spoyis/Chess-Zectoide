#include "../include/Board.h"

#define getColor(SDL_Color) SDL_Color.r, SDL_Color.g, SDL_Color.b, SDL_Color.a

void Board::fillBoard()
{
    int start_x = 0;
    int start_y = boardStart;

    int size = squareSize;
    short int curColor = 0;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            SDL_Rect rect = {start_x, start_y, squareSize, squareSize};

            if ((j + i) % 2 == 0)
                SDL_SetRenderDrawColor(renderer, getColor(squareColor[0]));
            else
                SDL_SetRenderDrawColor(renderer, getColor(squareColor[1]));

            SDL_RenderFillRect(renderer, &rect);
            start_x = (start_x + squareSize) % lineLength;
            if (start_x == 0) start_y += squareSize;
        }
    }
}

void Board::changeColors(SDL_Color color1, SDL_Color color2)
{
    squareColor[0] = color1;
    squareColor[1] = color2;
}
