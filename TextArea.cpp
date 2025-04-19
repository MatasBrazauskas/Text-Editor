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

	cursorX = CXOffset;
	cursorY = CYOffset;

	currentFileName = "idk.txt";
	ReadCurrentFile();
}

TextArea::~TextArea()
{
	// Destructor implementation
}

void TextArea::DisplayTextArea(SDL_Renderer* renderer, FontAndColors* colors)
{
	
    SDL_Rect rect = { (int)starting_X, (int)starting_Y, (int)(ending_X - starting_X), (int)(ending_Y - starting_Y) };
	SDL_Color backgroundColor = colors->GetColor(FontAndColors::Colors::BACKGROUND_AREA_COLOR);
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

        SDL_Surface* surface = TTF_RenderText_Blended(colors->TTFont, displayText.c_str(), colors->GetColor(FontAndColors::Colors::TEXT_COLOR));
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

		if (surface)
		{
			SDL_Rect textRect = { (int)starting_X + CXOffset, (int)starting_Y + CYOffset + yPos, (int)surface->w, (int)surface->h };

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

void TextArea::InsertNearTheCursor(FontAndColors* color, const char letter)
{
	if (31 < (int)letter && (int)letter < 128) {
		fileText.at(currentFileName).text.at(cursorRow).insert(fileText.at(currentFileName).text.at(cursorRow).begin() + cursorColumn, letter);
		cursorColumn++;
		int x, temp;
		TTF_SizeText(color->TTFont, std::string(1, letter).c_str(), &x, &temp);
		cursorX += x;
	}
}

void TextArea::DeleteCurrentLetter(FontAndColors* color)
{
	if (cursorColumn > 0)
	{
		int x, temp;
		cursorColumn--;
		TTF_SizeUTF8(color->TTFont, std::string(1, fileText.at(currentFileName).text.at(cursorRow)[cursorColumn]).c_str(), &x, &temp);
		fileText.at(currentFileName).text.at(cursorRow).erase(cursorColumn, 1);
		cursorX -= x;
	}
}

void TextArea::DisplayCursor(SDL_Renderer *renderer, FontAndColors* colors, int displayMode)
{
	switch (displayMode)
	{
	case 1:
	case 3:
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		for (size_t i = 0; i < colors->CFontHeight; i++)
		{
			for (size_t j = 0; j < cursorWidht; j++)
			{
				//SDL_RenderDrawPoint(renderer, starting_X + ((cursorColumn) * colors->CFontHeight/2) + j, starting_Y + ((cursorRow + 1) * colors->CFontHeight/2) + i);
				SDL_RenderDrawPoint(renderer, starting_X + cursorX + j, starting_Y + cursorY + i);
			}
		}
		break;
	/*case 0:
	case 2:
		for (size_t i = 0; i < cursor_Height; i++)
		{
			for (size_t j = 0; j < cursorWidht * 10; j++)
			{
				SDL_RenderDrawPoint(renderer, starting_X + cursorColumn + j, starting_Y + cursorRow + i);
			}
		}
		break;*/
	}

	SDL_RenderPresent(renderer);
}