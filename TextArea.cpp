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
	offTheEdgeY = 30;
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
		lineNumber = std::to_string(abs((int)index - (int)filesHashMap[currentFileName].Row));
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

	if (activeFiles.empty() == false)
	{
		int x, y;
		SDL_Rect rec = { (int)starting_X, (int)starting_Y, (int)(ending_X - starting_X), offTheEdgeY };

		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_RenderFillRect(renderer, &rec);

		int xOffset = 0;
		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

		for (const std::string& i : activeFiles)
		{
			TTF_SizeUTF8(colors->filesAreaTTFont, i.c_str(), &x, &y);
			
			SDL_Rect rec = { (int)starting_X + xOffset + 10, (int)starting_Y + 5, x, y};
			SDL_RenderFillRect(renderer, &rec);

			SDL_Surface* surface = TTF_RenderText_Blended(colors->filesAreaTTFont, i.c_str(), colors->GetColor(FontAndColors::Colors::OPPOSITE_TEXT_COLOR));
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

			if (surface)
			{
				SDL_RenderCopy(renderer, texture, nullptr, &rec);
				SDL_FreeSurface(surface);
			}

			xOffset += x + 10;
		}
	}
	
	if (showNumbers || relativeLineNumbers)
	{
		SDL_Rect rec = { (int)starting_X, (int)starting_Y + offTheEdgeY, offTheEdgeX, (int)(ending_Y - starting_Y - 1 - offTheEdgeY)};

		SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
		SDL_RenderFillRect(renderer, &rec);
	}

	for (size_t i = 0; i < filesHashMap.at(currentFileName).text.size(); i++)
	{
		std::string line = LineNumbers(i);

		if ((showNumbers || relativeLineNumbers) && i == filesHashMap.at(currentFileName).Row)
		{
			line = std::to_string(i + 1) + ' ';
			line.insert(0, 4 - line.length(), ' ');
		}

		line += filesHashMap.at(currentFileName).text.at(i);

		if (line.find((char)9) != std::string::npos)
		{
			line.replace(line.find(char(9)), 1, std::string(4,(char)129));
		}

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

	filesHashMap.insert({ currentFileName, textFileInfo() });

	while (std::getline(file, line))
	{
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
	for(const auto& i : activeFiles)
		std::for_each(filesHashMap.at(currentFileName).text.begin(), filesHashMap.at(currentFileName).text.end(), [&writer](const std::string& i) 
			{ 
				writer << i << '\n'; 
			});
	writer.close();
}

void TextArea::InsertNearTheCursor(FontAndColors* color, std::string letter)
{
	const char specialSymbols[] = {'\'', '\"', '{', '(', '['};
	const char opp[] = {'\'', '\"', '}', ')', ']'};

	if (const char* it = std::find(specialSymbols, specialSymbols + 5, letter[0]); it != (specialSymbols + 6))
	{
		size_t index = std::distance(specialSymbols, it);
		letter.push_back(opp[index]);
	}
	if (letter[0] == '\t')
	{
		letter = std::string(4, (char)129) + '\n';
	}

	auto& it = filesHashMap.at(currentFileName);
	it.text.at(it.Row).insert(it.text.at(it.Row).begin() + it.Col, letter.begin(), letter.end() - 1);
	
	it.Col += (letter == std::string(4, (char)129) + '\n') ? 4 : 1;
}

void TextArea::DeleteCurrentLetter(FontAndColors* color)
{
	auto& currText = filesHashMap.at(currentFileName);
	if (filesHashMap.at(currentFileName).Col > 0)
	{
		if (size_t it = currText.text.at(currText.Row).rfind(std::string(4, (char)129)); it != -1 && it + 4 == currText.Col)
		{
			currText.text.at(currText.Row).erase(it, 4);
			currText.Col -= 4;
			return;
		}
		currText.Col--;
		currText.text.at(currText.Row).erase(currText.Col, 1);
	}
	else if (currText.Col == 0 && currText.Row > 0)
	{
		currText.text.at(currText.Row - 1) += currText.text.at(currText.Row);
		currText.text.erase(currText.text.begin() + currText.Row);

		currText.Row--;
		currText.Col = currText.text.at(currText.Row).length();
	}
}

void TextArea::DisplayCursor(SDL_Renderer *renderer, FontAndColors* colors, const int displayMode)
{
	if (currentFileName.empty() == true)
		return;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	const auto& curr = filesHashMap.at(currentFileName);
	const size_t length = static_cast<bool>(displayMode & 0b1) ? (int)Offsets::cursorWidth : charWidth;

	for (size_t i = 0; i < charHeight; i++)
	{
		for (size_t j = 0; j < length; j++)
		{
			SDL_RenderDrawPoint(renderer, starting_X + offTheEdgeX + (curr.Col * charWidth) + j, starting_Y + offTheEdgeY + (curr.Row * charHeight) + ((int)Offsets::pixelsBetweenLines * curr.Row) + i);
		}
	}

	if (static_cast<bool>(displayMode & 0b1) == false && curr.Col < curr.text.at(curr.Row).length())
	{
		char currentLetter = curr.text.empty() ? ' ' : curr.text.at(curr.Row).at(curr.Col);
		if (currentLetter != '\0' && currentLetter != ' ' && currentLetter != (char)129)
		{
			const char text[2] = { currentLetter, '\0' };
			SDL_Surface* surface = TTF_RenderText_Blended(colors->TTFont, text, colors->GetColor(FontAndColors::Colors::OPPOSITE_TEXT_COLOR));
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

			if (surface)
			{
				SDL_Rect textRect = { (int)starting_X + offTheEdgeX + (curr.Col * charWidth), starting_Y + offTheEdgeY + (curr.Row * charHeight) + ((int)Offsets::pixelsBetweenLines * curr.Row), (int)surface->w, (int)surface->h };

				SDL_RenderCopy(renderer, texture, nullptr, &textRect);
				SDL_FreeSurface(surface);
			}
		}
	}
	SDL_RenderPresent(renderer);
}

void TextArea::MoveCursor(FontAndColors* color, const int x_offset, const int y_offset)
{
	auto& currText = filesHashMap.at(currentFileName);
	const size_t currentLineLength = currText.text.at(currText.Row).length();

	if (x_offset != 0 && currText.Col + x_offset >= 0 && currText.Col + x_offset < currentLineLength)
	{
		filesHashMap.at(currentFileName).Col += x_offset;
	}
	else if (y_offset != 0 && currText.Row + y_offset < currText.text.size() && currText.Row + y_offset >= 0)
	{
		currText.Row += y_offset;
		const size_t length = currText.text.at(currText.Row).length();

		if (length == 0)
		{
			currText.Col = 0;
		}
		else if (length < currText.Col)
		{
			currText.Col = length - 1;
		}
	}
}

void TextArea::MoveCursorToEnd(FontAndColors* color)
{
	const size_t len = filesHashMap.at(currentFileName).text.at(filesHashMap.at(currentFileName).Row).length() - 1;
	filesHashMap.at(currentFileName).Col = len;
}

void TextArea::CursorFromRight(FontAndColors* color)
{
	textFileInfo& currLines = filesHashMap.at(currentFileName);
	if (const size_t len = currLines.Col; len < currLines.text.at(currLines.Row).length())
	{
		currLines.Col++;
	}
}

void TextArea::AppendAndMoveToLine()
{
	filesHashMap.at(currentFileName).text.insert(filesHashMap.at(currentFileName).text.begin() + filesHashMap.at(currentFileName).Row + 1, std::string(""));
	filesHashMap.at(currentFileName).Col = 0;
	filesHashMap.at(currentFileName).Row++;
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
		filesHashMap.insert( { filesName, textFileInfo() });
		ReadCurrentFile();
	}
}

void TextArea::CloseFile()
{
	filesHashMap.erase(currentFileName);

	if (const std::vector<std::string>::iterator it = std::find(activeFiles.begin(), activeFiles.end(), currentFileName); it != activeFiles.end())
		activeFiles.erase(it);
	
	if (activeFiles.empty())
		offTheEdgeY = 10;

	currentFileName = activeFiles.empty() == false ? activeFiles.at(activeFiles.size() - 1) : "";
}

void TextArea::ChangeCurrentFile(int index)
{
	std::vector<std::string>::iterator it = std::find(activeFiles.begin(), activeFiles.end(), currentFileName);
	if (it - activeFiles.begin() + index < 0)
	{
		it = activeFiles.end() - 1;
	}
	else if (it - activeFiles.begin() + index >= activeFiles.size())
	{
		it = activeFiles.begin();
	}
	else
	{
		it += index;
	}
	currentFileName = *it;
}

void TextArea::JumpToBuffer(const std::string& index)
{
	if (std::all_of(index.begin(), index.end(), ::isdigit) == true)
	{
		const size_t jumpIndex = std::stoul(index);

		if (jumpIndex >= 1 && jumpIndex <= activeFiles.size())
		{
			currentFileName = activeFiles.at(jumpIndex - 1);
		}
	}
	else if(const std::vector<std::string>::iterator it = std::find(activeFiles.begin(), activeFiles.end(), index); it != activeFiles.end())
	{
		currentFileName = *it;
	}
}