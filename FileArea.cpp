#include "FileArea.h"

FileArea::FileArea()
{
	starting_X = 0;
	starting_Y = 0;
	ending_X = 250;
	ending_Y = 793;
}

FileArea::~FileArea()
{
	// Destructor implementation
}

void FileArea::DisplayFileArea(SDL_Renderer* renderer, FontAndColors* color)
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