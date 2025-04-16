#include "ErrorArea.h"

ErrorArea::ErrorArea()
{
	starting_X = 1000;
	starting_Y = 600;
	ending_X = 1536;
	ending_Y = 793;
}

ErrorArea::~ErrorArea()
{
	// Destructor implementation
}

void ErrorArea::DisplayFileArea(SDL_Renderer* renderer, SDL_Color color)
{
	SDL_Rect rect = { (int)starting_X, (int)starting_Y, (int)(ending_X - starting_X), (int)(ending_Y - starting_Y) };
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &rect);
	SDL_RenderPresent(renderer);
}