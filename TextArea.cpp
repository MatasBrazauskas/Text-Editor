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
	cursorWidht = 3;
	cursor_Height = 24;

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

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);

    int yPos = 0;

    for (const std::string& line : fileText[currentFileName].text) {
        std::string displayText = line.empty() ? " " : line;

		if (displayText.find('\t') != std::string::npos)
			displayText.replace(displayText.find('\t'), 1, "    ");
		if (displayText.find('\n') != std::string::npos)
			displayText.replace(displayText.find('\n'), 1," ");

        SDL_Surface* surface = TTF_RenderText_Blended(font, displayText.c_str(), textColor);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

		if (surface)
		{
			SDL_Rect textRect = { (int)starting_X + 10, (int)starting_Y + 10 + yPos, (int)surface->w, (int)surface->h };

			SDL_RenderCopy(renderer, texture, nullptr, &textRect);
			SDL_FreeSurface(surface);
		}
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

void TextArea::WriteIntoCurrentFile()
{
	std::ofstream writer(currentFileName);

	std::for_each(fileText.at(currentFileName).text.begin(), fileText.at(currentFileName).text.end(), [&writer](const std::string i)
	{
			writer << i;// << '\n';
	});
}

void TextArea::InsertNearTheCursor(const char letter)
{
	//std::string text = fileText.at(currentFileName).text.at(cursorRow);
	if ((int)letter != 0) {
		fileText.at(currentFileName).text.at(cursorRow).insert(fileText.at(currentFileName).text.at(cursorRow).begin() + cursorColumn, letter);
		cursorColumn++;
	}

//	WriteIntoCurrentFile();
}

void TextArea::DeleteCurrentLetter()
{
	fileText[currentFileName].text[cursorRow].erase(cursorColumn, 1);
}

void TextArea::DisplayCursor(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	for (size_t i = 0; i < cursor_Height; i++)
	{
		for (size_t j = 0; j < cursorWidht; j++)
		{
			SDL_RenderDrawPoint(renderer, starting_X + cursorColumn + j, starting_Y + cursor_Height + i);
		}
	}
	SDL_RenderPresent(renderer);	
}