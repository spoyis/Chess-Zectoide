#include "../include/Board.h"


#define getColor(SDL_Color) SDL_Color.r, SDL_Color.g, SDL_Color.b, SDL_Color.a
#define getMovedColor(SDL_Color) SDL_Color.r * 0.6, SDL_Color.g * 0.6, SDL_Color.b * 0.4, SDL_Color.a

void Board::fillBoard(std::pair<int,int> lastMoveBegin, std::pair<int, int> lastMoveEnd)
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
            bool movedSquare = (j == lastMoveBegin.first && i == lastMoveBegin.second) || (j == lastMoveEnd.first && i == lastMoveEnd.second);
            if ((j + i) % 2 == 0)
            {
                if (movedSquare)
                    SDL_SetRenderDrawColor(renderer, getMovedColor(squareColor[0]));
                else
                    SDL_SetRenderDrawColor(renderer, getColor(squareColor[0]));
            }
            else {
                if(movedSquare)
                    SDL_SetRenderDrawColor(renderer, getMovedColor(squareColor[1]));
                else
                    SDL_SetRenderDrawColor(renderer, getColor(squareColor[1]));
            }
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
