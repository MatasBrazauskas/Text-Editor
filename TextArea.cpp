#include "TextArea.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <print>
#include <filesystem>

TextArea::TextArea()
{
	starting_X = 250;
	starting_Y = 0;
	ending_X = 1536;
	ending_Y = 600;

	cursorWidht = 2;

	cursorX = CXOffset;
	cursorY = CYOffset;

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
		lineNumber = std::to_string(abs((int)index - (int)filesHashMap[currentFileName].currentRow));

	for(size_t i = 0; i < (int)(filesHashMap[currentFileName].text.size() /  10) + 1 - lineNumber.length(); i++)
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

	if (currentFileName.empty() == true)
		return;

    int yPos = 0;
	
	CYOffset = 0;
	CXOffset = 0;

	if (showNumbers)
	{
		//CXOffset += (log10(fileText[currentFileName].text.size()) + 1) * 5;
		int x, y;
		TTF_SizeUTF8(colors->TTFont, std::string(1, 'a').c_str(), &x, &y);
		SDL_Rect rec = { (int)starting_X, (int)starting_Y, (int)(log10(filesHashMap[currentFileName].text.size()) + 1) * x, (int)(ending_Y - starting_Y - 2) };

		SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
		SDL_RenderFillRect(renderer, &rec);

		CYOffset = (int)(log10(filesHashMap[currentFileName].text.size()) + 1) * x;
		CXOffset = CYOffset;
	}

	for (size_t i = 0; i < filesHashMap.at(currentFileName).text.size(); i++){

		std::string line = LineNumbers(i) + filesHashMap.at(currentFileName).text.at(i);

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
	if (currentFileName.empty() == true)
		return;

	std::ifstream file(currentFileName);
	std::string line;


	filesHashMap.insert({ currentFileName, textFileInformation() });

	while (std::getline(file, line))
	{
		filesHashMap[currentFileName].text.push_back(line);
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

	std::for_each(filesHashMap.at(currentFileName).text.begin(), filesHashMap.at(currentFileName).text.end(), [&writer](const std::string i) { writer << i << '\n'; });
	writer.close();
}

void TextArea::InsertNearTheCursor(FontAndColors* color, const char letter)
{
	filesHashMap.at(currentFileName).text.at(filesHashMap.at(currentFileName).currentRow).insert(filesHashMap.at(currentFileName).text.at(filesHashMap.at(currentFileName).currentRow).begin() + filesHashMap.at(currentFileName).currentColumn, letter);
	//filesHashMap.at(currentFileName);// .text.at(filesHashMap.at(currentFileName).currentRow);
	filesHashMap.at(currentFileName).currentColumn++;
	int x, y;
	TTF_SizeText(color->TTFont, std::string(1, letter).c_str(), &x, &y);
	cursorX += x;
}

void TextArea::DeleteCurrentLetter(FontAndColors* color)
{
	if (filesHashMap[currentFileName].currentColumn > 0)
	{
		int x, y;
		filesHashMap[currentFileName].currentColumn--;

		TTF_SizeUTF8(color->TTFont, std::string(1, filesHashMap.at(currentFileName).text.at(filesHashMap[currentFileName].currentRow)[filesHashMap[currentFileName].currentColumn]).c_str(), &x, &y);
		filesHashMap.at(currentFileName).text.at(filesHashMap[currentFileName].currentRow).erase(filesHashMap[currentFileName].currentColumn, 1);
		cursorX -= x;
	}
	else if (filesHashMap[currentFileName].currentColumn == 0 && filesHashMap[currentFileName].currentRow > 0)
	{
		filesHashMap.at(currentFileName).text.at(filesHashMap[currentFileName].currentRow - 1) += filesHashMap.at(currentFileName).text.at(filesHashMap[currentFileName].currentRow);
		filesHashMap.at(currentFileName).text.erase(filesHashMap.at(currentFileName).text.begin() + filesHashMap[currentFileName].currentRow);

		filesHashMap[currentFileName].currentRow--;
		cursorY -= TEMPYOFFSETFORCURSOR;

		int x, y;
		filesHashMap[currentFileName].currentColumn = filesHashMap.at(currentFileName).text.at(filesHashMap[currentFileName].currentRow).length() - 1;
		TTF_SizeUTF8(color->TTFont, filesHashMap.at(currentFileName).text.at(filesHashMap[currentFileName].currentRow).substr(0, filesHashMap[currentFileName].currentColumn).c_str(), &x, &y);
		cursorX = CXOffset + x;
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
		char currentLetter = filesHashMap.at(currentFileName).text.empty() ? ' ' : filesHashMap.at(currentFileName).text.at(filesHashMap[currentFileName].currentRow)[filesHashMap[currentFileName].currentColumn];
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
	TTF_SizeUTF8(color->TTFont, std::string(1, filesHashMap.at(currentFileName).text.at(filesHashMap[currentFileName].currentRow)[filesHashMap[currentFileName].currentColumn]).c_str(), &x, &y);
	size_t currentLineLength = filesHashMap.at(currentFileName).text.at(filesHashMap[currentFileName].currentRow).length();

	if (x_offset != 0 && filesHashMap[currentFileName].currentColumn + x_offset >= 0 && filesHashMap[currentFileName].currentColumn + x_offset < currentLineLength)
	{
		if (x_offset == -1) {
			TTF_SizeUTF8(color->TTFont, std::string(1, filesHashMap.at(currentFileName).text.at(filesHashMap[currentFileName].currentRow)[filesHashMap[currentFileName].currentColumn - 1]).c_str(), &x, &y);
			cursorX -= x;
			filesHashMap[currentFileName].currentColumn--;
		}
		else if (x_offset == 1) {
			cursorX += x + CXOffset;
			filesHashMap[currentFileName].currentColumn++;
		}
	}
	else if (y_offset != 0 && filesHashMap[currentFileName].currentRow + y_offset < filesHashMap.at(currentFileName).text.size() && filesHashMap[currentFileName].currentRow + y_offset >= 0)
	{
		filesHashMap[currentFileName].currentRow += y_offset;
		cursorY += (y_offset * TEMPYOFFSETFORCURSOR);

		size_t length = filesHashMap.at(currentFileName).text.at(filesHashMap[currentFileName].currentRow).length();

		if (length <= filesHashMap[currentFileName].currentColumn)
		{
			size_t newStringLength = filesHashMap.at(currentFileName).text.at(filesHashMap[currentFileName].currentRow).length() - 1;
			TTF_SizeUTF8(color->TTFont, filesHashMap.at(currentFileName).text.at(filesHashMap[currentFileName].currentRow).substr(0, newStringLength).c_str(), &x, &y);
			cursorX = CXOffset + x;
			filesHashMap[currentFileName].currentColumn = newStringLength;
		}
	}
}

void TextArea::MoveCursorToEnd(FontAndColors* color)
{
	int x, y;
	size_t len = filesHashMap.at(currentFileName).text.at(filesHashMap[currentFileName].currentRow).length() - 1;
	TTF_SizeText(color->TTFont, filesHashMap.at(currentFileName).text.at(filesHashMap[currentFileName].currentRow).substr(0, len).c_str(), &x, &y);
	cursorX = CYOffset + x;
	filesHashMap[currentFileName].currentColumn = len;
}

void TextArea::CursorFromRight(FontAndColors* color)
{
	int x, y;
	TTF_SizeUTF8(color->TTFont, std::string(1, filesHashMap.at(currentFileName).text.at(filesHashMap[currentFileName].currentRow).at(filesHashMap[currentFileName].currentColumn)).c_str(), &x, &y);
	filesHashMap[currentFileName].currentColumn++;
	cursorX += x;
}

void TextArea::AppendAndMoveToLine()
{
	filesHashMap.at(currentFileName).text.insert(filesHashMap.at(currentFileName).text.begin() + filesHashMap[currentFileName].currentRow + 1, std::string());
	cursorX = CXOffset;
	filesHashMap[currentFileName].currentColumn = 0;

	cursorY += TEMPYOFFSETFORCURSOR;
	filesHashMap[currentFileName].currentRow++;
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
		filesHashMap.insert({ filesName, textFileInformation() });
		ReadCurrentFile();
	}
}