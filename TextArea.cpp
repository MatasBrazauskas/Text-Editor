#include "TextArea.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <print>
#include <filesystem>

TextArea::TextArea(FontAndColors* color)
{
	starting_X = 250;
	starting_Y = 0;
	ending_X = 1536;
	ending_Y = 600;

	TTF_SizeUTF8(color->TTFont, std::string(1, 'a').c_str(), &charWidth, &charHeight);

	offTheEdgeX = 4 * charWidth;
	offTheEdgeY = 10;
	cursorX = cursorY = 0;
}

TextArea::~TextArea()
{
	// Destructor implementation
}

std::string TextArea::LineNumbers(size_t index)
{
	if((showNumbers || relativeLineNumbers) == false)
		return "    ";

	std::string lineNumber = std::to_string(index + 1);

	if (relativeLineNumbers)
	{
		lineNumber = std::to_string(abs((int)index - (int)filesHashMap[currentFileName].currentRow));
	}
	lineNumber.insert(0, 4 - lineNumber.length(), ' ');

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

	if (currentFileName.empty() == true)
		return;

    int yPos = 0;
	
	if (showNumbers || relativeLineNumbers)
	{
		SDL_Rect rec = { (int)starting_X, (int)starting_Y, offTheEdgeX, (int)(ending_Y - starting_Y - 1)};

		SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
		SDL_RenderFillRect(renderer, &rec);
	}

	for (size_t i = 0; i < filesHashMap.at(currentFileName).text.size(); i++)
	{
		std::string line = LineNumbers(i);

		if ((showNumbers || relativeLineNumbers) && i == filesHashMap.at(currentFileName).currentRow)
		{
			line = std::to_string(i + 1) + ' ';
			line.insert(0, 4 - line.length(), ' ');
		}

		line += filesHashMap.at(currentFileName).text.at(i);

		if (line.find((char)9) != std::string::npos)
			line.replace(line.find('\t'), 1, "    ");
		if (line.find('\n') != std::string::npos)
			line.replace(line.find('\n'), 1," ");

        SDL_Surface* surface = TTF_RenderText_Blended(colors->TTFont, line.c_str(), colors->GetColor(FontAndColors::Colors::TEXT_COLOR));
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

		if (surface)
		{
			SDL_Rect textRect = { (int)starting_X, (int)starting_Y + offTheEdgeY + yPos, (int)surface->w, (int)surface->h};

			SDL_RenderCopy(renderer, texture, nullptr, &textRect);
			SDL_FreeSurface(surface);
		}
		yPos += ((int)Offsets::pixelsBetweenLines + charHeight);
    }

    SDL_RenderPresent(renderer);
}

void TextArea::ReadCurrentFile()
{
	if (currentFileName.empty() == true)
		return;

	std::ifstream file(currentFileName);
	std::string line;


	filesHashMap.insert({ currentFileName, textFileInformation() });

	while (std::getline(file, line))
	{
		if (line.empty())
			line = " ";
		filesHashMap.at(currentFileName).text.push_back(line);
	}

	if (filesHashMap.at(currentFileName).text.size() == 0)
	{
		filesHashMap.at(currentFileName).text.push_back(" ");
	}

	file.close();
}

void TextArea::WriteIntoCurrentFile()
{
	std::ofstream writer(currentFileName);
	std::for_each(filesHashMap.at(currentFileName).text.begin(), filesHashMap.at(currentFileName).text.end(), [&writer](const std::string& i) { writer << i << '\n'; });
	writer.close();
}

void TextArea::InsertNearTheCursor(FontAndColors* color, const char letter)
{
	auto start = filesHashMap.at(currentFileName).text.at(filesHashMap.at(currentFileName).currentRow).begin();
	filesHashMap.at(currentFileName).text.at(filesHashMap.at(currentFileName).currentRow).insert(start + filesHashMap.at(currentFileName).currentColumn, letter);

	filesHashMap.at(currentFileName).currentColumn++;
	cursorX += charWidth;
}

void TextArea::DeleteCurrentLetter(FontAndColors* color)
{
	if (filesHashMap.at(currentFileName).currentColumn > 0)
	{
		filesHashMap.at(currentFileName).currentColumn--;

		filesHashMap.at(currentFileName).text.at(filesHashMap.at(currentFileName).currentRow).erase(filesHashMap.at(currentFileName).currentColumn, 1);
		cursorX -= charWidth;
	}
	else if (filesHashMap.at(currentFileName).currentColumn == 0 && filesHashMap.at(currentFileName).currentRow > 0)
	{
		filesHashMap.at(currentFileName).text.at(filesHashMap[currentFileName].currentRow - 1) += filesHashMap.at(currentFileName).text.at(filesHashMap[currentFileName].currentRow);
		filesHashMap.at(currentFileName).text.erase(filesHashMap.at(currentFileName).text.begin() + filesHashMap[currentFileName].currentRow);

		filesHashMap[currentFileName].currentRow--;
		cursorY -= ((int)Offsets::pixelsBetweenLines + charHeight);

		filesHashMap[currentFileName].currentColumn = filesHashMap.at(currentFileName).text.at(filesHashMap[currentFileName].currentRow).length() - 1;
		cursorX = charWidth * filesHashMap[currentFileName].currentColumn;
	}
}

void TextArea::DisplayCursor(SDL_Renderer *renderer, FontAndColors* colors, int displayMode)
{
	if (currentFileName.empty() == true)
		return;

	switch (displayMode)
	{
	case 1: case 3:
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		for (size_t i = 0; i < charHeight; i++)
		{
			for (size_t j = 0; j < (int)Offsets::cursorWidth; j++)
			{
				SDL_RenderDrawPoint(renderer, starting_X + offTheEdgeX + cursorX + j, starting_Y + offTheEdgeY + cursorY + i);
			}
		}
		break;
	case 0: case 2:
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		char currentLetter = filesHashMap.at(currentFileName).text.empty() ? ' ' : filesHashMap.at(currentFileName).text.at(filesHashMap[currentFileName].currentRow)[filesHashMap[currentFileName].currentColumn];
		if (currentLetter == '\0')
		{
			currentLetter = ' ';
		}

		for (size_t i = 0; i < charHeight; i++)
		{
			for (size_t j = 0; j < charWidth; j++)
			{
				SDL_RenderDrawPoint(renderer, starting_X + offTheEdgeX + cursorX + j, starting_Y + offTheEdgeY + cursorY + i);
			}
		}

		SDL_Surface* surface = TTF_RenderText_Blended(colors->TTFont, std::string(1, currentLetter).c_str(), colors->GetColor(FontAndColors::Colors::OPPOSITE_TEXT_COLOR));
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

		if (surface)
		{
			SDL_Rect textRect = { (int)starting_X + offTheEdgeX + cursorX, (int)starting_Y + offTheEdgeY + cursorY, (int)surface->w, (int)surface->h};

			SDL_RenderCopy(renderer, texture, nullptr, &textRect);
			SDL_FreeSurface(surface);
		}

		break;
	}


	SDL_RenderPresent(renderer);
}

void TextArea::MoveCursor(FontAndColors* color, const int x_offset, const int y_offset)
{
	size_t currentLineLength = filesHashMap.at(currentFileName).text.at(filesHashMap.at(currentFileName).currentRow).length();

	if (x_offset != 0 && filesHashMap.at(currentFileName).currentColumn + x_offset >= 0 && filesHashMap.at(currentFileName).currentColumn + x_offset < currentLineLength)
	{
		cursorX += (charWidth * x_offset);
		filesHashMap.at(currentFileName).currentColumn += x_offset;
	}
	else if (y_offset != 0 && filesHashMap.at(currentFileName).currentRow + y_offset < filesHashMap.at(currentFileName).text.size() && filesHashMap.at(currentFileName).currentRow + y_offset >= 0)
	{
		filesHashMap.at(currentFileName).currentRow += y_offset;
		cursorY += (y_offset * ((int)Offsets::pixelsBetweenLines + charHeight));

		size_t length = filesHashMap.at(currentFileName).text.at(filesHashMap.at(currentFileName).currentRow).length();

		if (length <= filesHashMap.at(currentFileName).currentColumn)
		{
			size_t newStringLength = filesHashMap.at(currentFileName).text.at(filesHashMap.at(currentFileName).currentRow).length() - 1;
			cursorX = newStringLength * charWidth;
			filesHashMap.at(currentFileName).currentColumn = newStringLength;
		}
	}
}

void TextArea::MoveCursorToEnd(FontAndColors* color)
{
	size_t len = filesHashMap.at(currentFileName).text.at(filesHashMap.at(currentFileName).currentRow).length() - 1;
	cursorX = len * charWidth;
	filesHashMap.at(currentFileName).currentColumn = len;
}

void TextArea::CursorFromRight(FontAndColors* color)
{
	if (filesHashMap.at(currentFileName).currentColumn + 1 != filesHashMap.at(currentFileName).text.at(filesHashMap.at(currentFileName).currentRow).size())
	{
		filesHashMap.at(currentFileName).currentColumn++;
		cursorX += charWidth;
	}
}

void TextArea::AppendAndMoveToLine()
{
	filesHashMap.at(currentFileName).text.insert(filesHashMap.at(currentFileName).text.begin() + filesHashMap.at(currentFileName).currentRow + 1, std::string(" "));
	filesHashMap.at(currentFileName).currentColumn = 0;
	filesHashMap.at(currentFileName).currentRow++;

	cursorX = 0;
	cursorY += ((int)Offsets::pixelsBetweenLines + charHeight);
}

void TextArea::LoadOtherFile(const std::string& filesName)
{
	if (std::filesystem::exists(filesName) == false)
	{
		return;
	}	

	currentFileName = filesName;
	if (std::find(activeFiles.begin(), activeFiles.end(), filesName) == activeFiles.end())
	{
		activeFiles.push_back(filesName);
		filesHashMap.insert( { filesName, textFileInformation() });
		ReadCurrentFile();
	}
}

void TextArea::CloseFile()
{
	filesHashMap.erase(currentFileName);

	if (std::vector<std::string>::iterator it = std::find(activeFiles.begin(), activeFiles.end(), currentFileName); it != activeFiles.end())
		activeFiles.erase(it);

	currentFileName = "";
}