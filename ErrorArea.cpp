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

void ErrorArea::DisplayFileArea(SDL_Renderer* renderer, FontAndColors* color)
{
	SDL_Rect rect = { (int)starting_X, (int)starting_Y, (int)(ending_X - starting_X), (int)(ending_Y - starting_Y) };
	SDL_SetRenderDrawColor(renderer, 
		color->GetColor(FontAndColors::Colors::BACKGROUND_AREA_COLOR).r, 
		color->GetColor(FontAndColors::Colors::BACKGROUND_AREA_COLOR).g, 
		color->GetColor(FontAndColors::Colors::BACKGROUND_AREA_COLOR).b, 
		color->GetColor(FontAndColors::Colors::BACKGROUND_AREA_COLOR).a);
	SDL_RenderFillRect(renderer, &rect);
	SDL_RenderPresent(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White border
	SDL_RenderDrawRect(renderer, &rect);
}