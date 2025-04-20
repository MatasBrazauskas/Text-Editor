#include "FileArea.h"
#include <filesystem>

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

	int y = 0;
	for (const auto& filesPath : std::filesystem::directory_iterator(path))
	{
		std::string temp = filesPath.path().string();
		size_t index = temp.rfind("\\");
		if (index != std::string::npos)
		{
			temp = temp.substr(index);
		}

		temp[0] = ' ';
		if (filesPath.is_directory())
		{
			temp[0] = '>';
			temp += '/';
		}

		SDL_Surface* surface = TTF_RenderText_Blended(color->filesAreaTTFont, temp.c_str(), color->GetColor(FontAndColors::Colors::TEXT_COLOR));
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

		if (surface)
		{
			SDL_Rect textRect = { (int)starting_X + 10, (int)starting_Y + 10 + y, (int)surface->w, (int)surface->h };

			SDL_RenderCopy(renderer, texture, nullptr, &textRect);
			SDL_FreeSurface(surface);
		}
		y+=20;
	}

	SDL_RenderDrawRect(renderer, &rect);
}