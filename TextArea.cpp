#include "TextArea.h"
#include <fstream>
#include <algorithm>
#include <iostream>

TextArea::TextArea()
{
	starting_X = 250;
	starting_Y = 0;
	ending_X = 1536;
	ending_Y = 600;

	cursorRow = 0;
	cursorColumn = 0;

	currentFileName = "idk.txt";
	ReadCurrentFile();
}

TextArea::~TextArea()
{
	// Destructor implementation
}

void TextArea::DisplayFileArea(SDL_Renderer* renderer, SDL_Color backgroundColor, SDL_Color textColor, TTF_Font* font)
{
    SDL_Rect rect = { (int)starting_X, (int)starting_Y, (int)(ending_X - starting_X), (int)(ending_Y - starting_Y) };
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderFillRect(renderer, &rect);

    auto lambda = [&](const std::string& line, int yPos)
        {
            SDL_Surface* surface = TTF_RenderText_Solid(font, line.c_str(), textColor);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_Rect textRect = { (int)starting_X + 10, (int)starting_Y + 10 + yPos, (int)surface->w, (int)surface->h };
            SDL_RenderCopy(renderer, texture, nullptr, &textRect);

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        };

    int yPos = 0;

    for (const auto& line : fileText[currentFileName].text) {
        lambda(line, yPos);
        yPos += 30;
    }

    SDL_RenderPresent(renderer);
}

void TextArea::ReadCurrentFile()
{
	std::ifstream file(currentFileName);
	std::string line;

	while (std::getline(file, line))
	{
		fileText[currentFileName].text.push_back(line);
	}

	fileText[currentFileName].currentIndex = 0;
	file.close();
}