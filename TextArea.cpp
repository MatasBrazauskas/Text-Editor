#include "TextArea.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <cmath>

TextArea::TextArea()
{
	starting_X = 250;
	starting_Y = 0;
	ending_X = 1536;
	ending_Y = 600;

	cursorRow = 0;
	cursorColumn = 0;

	cursorWidht = 2;

	cursorX = CXOffset;
	cursorY = CYOffset;

	currentFileName = "idk.txt";
	ReadCurrentFile();
}

TextArea::~TextArea()
{
	// Destructor implementation
}

std::string TextArea::LineNumbers(size_t index)
{
	if(showNumbers == false)
		return "";

	std::string lineNumber = std::to_string(index + 1);
	if (relativeLineNumbers)
		lineNumber = std::to_string(abs((int)index - (int)cursorRow));

	for(size_t i = 0; i < (int)(fileText[currentFileName].text.size() /  10) + 1 - lineNumber.length(); i++)
	{
		lineNumber.insert(0, 1, ' ');
	}

	return lineNumber;
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
	
	CYOffset = 0;
	CXOffset = 0;

	if (showNumbers)
	{
		//CXOffset += (log10(fileText[currentFileName].text.size()) + 1) * 5;
		int x, y;
		TTF_SizeUTF8(colors->TTFont, std::string(1, 'a').c_str(), &x, &y);
		SDL_Rect rec = { (int)starting_X, (int)starting_Y, (int)(log10(fileText[currentFileName].text.size()) + 1) * x, (int)(ending_Y - starting_Y - 2) };

		SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
		SDL_RenderFillRect(renderer, &rec);

		CYOffset = (int)(log10(fileText[currentFileName].text.size()) + 1) * x;
		CXOffset = CYOffset;
	}

	for (size_t i = 0; i < fileText[currentFileName].text.size(); i++){
		std::string line = LineNumbers(i) + fileText[currentFileName].text[i];

		if (line.empty())
			line = " ";
		if (line.find((char)9) != std::string::npos)
			line.replace(line.find('\t'), 1, "    ");
		if (line.find('\n') != std::string::npos)
			line.replace(line.find('\n'), 1," ");

        SDL_Surface* surface = TTF_RenderText_Blended(colors->TTFont, line.c_str(), colors->GetColor(FontAndColors::Colors::TEXT_COLOR));
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

		if (surface)
		{
			SDL_Rect textRect = { (int)starting_X /* + CXOffset*/, (int)starting_Y + CYOffset + yPos, (int)surface->w, (int)surface->h};

			SDL_RenderCopy(renderer, texture, nullptr, &textRect);
			SDL_FreeSurface(surface);
		}
        yPos += TEMPYOFFSETFORCURSOR;
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

	std::for_each(fileText.at(currentFileName).text.begin(), fileText.at(currentFileName).text.end(), [&writer](const std::string i) { writer << i << '\n'; });
	writer.close();
}

void TextArea::InsertNearTheCursor(FontAndColors* color, const char letter)
{
	fileText.at(currentFileName).text.at(cursorRow).insert(fileText.at(currentFileName).text.at(cursorRow).begin() + cursorColumn, letter);
	cursorColumn++;
	int x, y;
	TTF_SizeText(color->TTFont, std::string(1, letter).c_str(), &x, &y);
	cursorX += x;
}

void TextArea::DeleteCurrentLetter(FontAndColors* color)
{
	if (cursorColumn > 0)
	{
		int x, y;
		cursorColumn--;

		TTF_SizeUTF8(color->TTFont, std::string(1, fileText.at(currentFileName).text.at(cursorRow)[cursorColumn]).c_str(), &x, &y);
		fileText.at(currentFileName).text.at(cursorRow).erase(cursorColumn, 1);
		cursorX -= x;
	}
	else if (cursorColumn == 0 && cursorRow > 0)
	{
		fileText.at(currentFileName).text.at(cursorRow - 1) += fileText.at(currentFileName).text.at(cursorRow);
		fileText.at(currentFileName).text.erase(fileText.at(currentFileName).text.begin() + cursorRow);

		cursorRow--;
		cursorY -= TEMPYOFFSETFORCURSOR;

		int x, y;
		cursorColumn = fileText.at(currentFileName).text.at(cursorRow).length() - 1;
		TTF_SizeUTF8(color->TTFont, fileText.at(currentFileName).text.at(cursorRow).substr(0, cursorColumn).c_str(), &x, &y);
		cursorX = CXOffset + x;
	}
}

void TextArea::DisplayCursor(SDL_Renderer *renderer, FontAndColors* colors, int displayMode)
{
	
	switch (displayMode)
	{
	case 1: case 3:
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		for (size_t i = 0; i < colors->CFontHeight; i++)
		{
			for (size_t j = 0; j < cursorWidht; j++)
			{
				SDL_RenderDrawPoint(renderer, starting_X + CXOffset + cursorX + j, starting_Y + CYOffset + cursorY + i);
			}
		}
		break;
	case 0: case 2:
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		int x, y;
		char currentLetter = fileText.at(currentFileName).text.at(cursorRow)[cursorColumn];
		if (currentLetter == '\0')
		{
			currentLetter = ' ';
		}
		TTF_SizeUTF8(colors->TTFont, std::string(1, currentLetter).c_str(), &x, &y);

		for (size_t i = 0; i < colors->CFontHeight; i++)
		{
			for (size_t j = 0; j < x; j++)
			{
				SDL_RenderDrawPoint(renderer, starting_X + CXOffset + cursorX + j, starting_Y + CYOffset + cursorY + i);
			}
		}

		SDL_Surface* surface = TTF_RenderText_Blended(colors->TTFont, std::string(1, currentLetter).c_str(), colors->GetColor(FontAndColors::Colors::OPPOSITE_TEXT_COLOR));
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

		if (surface)
		{
			SDL_Rect textRect = { (int)starting_X + CXOffset + cursorX, (int)starting_Y + CYOffset + cursorY, (int)surface->w, (int)surface->h};

			SDL_RenderCopy(renderer, texture, nullptr, &textRect);
			SDL_FreeSurface(surface);
		}

		break;
	}


	SDL_RenderPresent(renderer);
}

void TextArea::MoveCursor(FontAndColors* color, const int x_offset, const int y_offset)
{
	int x, y;
	TTF_SizeUTF8(color->TTFont, std::string(1, fileText.at(currentFileName).text.at(cursorRow)[cursorColumn]).c_str(), &x, &y);
	size_t currentLineLength = fileText.at(currentFileName).text.at(cursorRow).length();

	if (x_offset != 0 && cursorColumn + x_offset >= 0 && cursorColumn + x_offset < currentLineLength)
	{
		if (x_offset == -1) {
			TTF_SizeUTF8(color->TTFont, std::string(1, fileText.at(currentFileName).text.at(cursorRow)[cursorColumn - 1]).c_str(), &x, &y);
			cursorX -= x;
			cursorColumn--;
		}
		else if (x_offset == 1) {
			cursorX += x + CXOffset;
			cursorColumn++;
		}
	}
	else if (y_offset != 0 && cursorRow + y_offset < fileText.at(currentFileName).text.size() && cursorRow + y_offset >= 0)
	{
		cursorRow += y_offset;
		cursorY += (y_offset * TEMPYOFFSETFORCURSOR);

		size_t length = fileText.at(currentFileName).text.at(cursorRow).length();

		if (length <= cursorColumn)
		{
			size_t newStringLength = fileText.at(currentFileName).text.at(cursorRow).length() - 1;
			TTF_SizeUTF8(color->TTFont, fileText.at(currentFileName).text.at(cursorRow).substr(0, newStringLength).c_str(), &x, &y);
			cursorX = CXOffset + x;
			cursorColumn = newStringLength;
		}
	}
}

void TextArea::MoveCursorToEnd(FontAndColors* color)
{
	int x, y;
	size_t len = fileText.at(currentFileName).text.at(cursorRow).length() - 1;
	TTF_SizeText(color->TTFont, fileText.at(currentFileName).text.at(cursorRow).substr(0, len).c_str(), &x, &y);
	cursorX = CYOffset + x;
	cursorColumn = len;
}

void TextArea::CursorFromRight(FontAndColors* color)
{
	int x, y;
	TTF_SizeUTF8(color->TTFont, std::string(1, fileText.at(currentFileName).text.at(cursorRow).at(cursorColumn)).c_str(), &x, &y);
	cursorColumn++;
	cursorX += x;
}
void TextArea::AppendAndMoveToLine()
{
	fileText.at(currentFileName).text.insert(fileText.at(currentFileName).text.begin() + cursorRow + 1, std::string());
	cursorX = CXOffset;
	cursorColumn = 0;

	cursorY += TEMPYOFFSETFORCURSOR;
	cursorRow++;
}