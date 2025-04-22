#include "CommandLineArea.h"
#include "FontAndColors.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <memory>
#include <sstream>

CommandLineArea::CommandLineArea()
{
	starting_X = 250;
	starting_Y = 600;
	ending_X = 1000;
	ending_Y = 793;
}

CommandLineArea::~CommandLineArea()
{
	// Destructor implementation
}

void CommandLineArea::DisplayShellInput(SDL_Renderer* renderer, FontAndColors* color, int mode)
{
	std::string commandLineMessage = "";

	switch (mode)
	{
	case 0: commandLineMessage = "-- NORMAL --"; break;
	case 1: commandLineMessage = "-- INSERT --"; break;
	case 2: commandLineMessage = "-- VISUAL --"; break;
	//case 3: commandLineMessage = "[COMMAND]"; break;
	}

	SDL_Rect rect = { (int)starting_X, (int)starting_Y, (int)(ending_X - starting_X), 20 };
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &rect);

	SDL_Color textColor = { 0,0,0,255 };

	SDL_Surface* surface = TTF_RenderText_Blended(color->TTFont, (commandLineMessage + currentCommand).c_str(), textColor);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	if (surface)
	{
		SDL_Rect textRect = { (int)starting_X + 20, (int)starting_Y, (int)surface->w, (int)surface->h };

		SDL_RenderCopy(renderer, texture, nullptr, &textRect);
		SDL_FreeSurface(surface);
	}

	SDL_RenderPresent(renderer);
}

void CommandLineArea::DisplayShellOutput(SDL_Renderer* renderer, FontAndColors* color)
{
	SDL_Rect rect = { (int)starting_X, (int)starting_Y + 20, (int)(ending_X - starting_X), (int)(ending_Y - starting_Y - 20) };
	SDL_SetRenderDrawColor(renderer,
		color->GetColor(FontAndColors::Colors::BACKGROUND_AREA_COLOR).r,
		color->GetColor(FontAndColors::Colors::BACKGROUND_AREA_COLOR).g,
		color->GetColor(FontAndColors::Colors::BACKGROUND_AREA_COLOR).b,
		color->GetColor(FontAndColors::Colors::BACKGROUND_AREA_COLOR).a);
	SDL_RenderFillRect(renderer, &rect);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White border
	SDL_RenderDrawRect(renderer, &rect);

	std::string line = ExucuteAndDisplayCommand();// .substr(0, 50);

	if (line != "")
		std::cout << line[45] << ' ' << (int)line[45] << '\n';

	std::stringstream ss(line);
	std::string item;
	size_t YOffset = 25;

	while (std::getline(ss, item, (char)10))
	{
		SDL_Surface* surface = TTF_RenderText_Blended(color->filesAreaTTFont, item.c_str(), color->GetColor(FontAndColors::Colors::TEXT_COLOR));
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

		if (surface)
		{
			SDL_Rect textRect = { (int)starting_X + 10, (int)starting_Y + YOffset, (int)surface->w, (int)surface->h};

			SDL_RenderCopy(renderer, texture, nullptr, &textRect);
			SDL_FreeSurface(surface);
		}
		YOffset += 20;
	}

	SDL_RenderPresent(renderer);
}

void CommandLineArea::AppendToCommand(const char letter)
{
	currentCommand += letter;
}

void CommandLineArea::DeleteToCommand()
{
	if (currentCommand.length() > 0)
		currentCommand.pop_back();
}

std::string CommandLineArea::ExucuteAndDisplayCommand()
{
	std::string result = "";
	char buffer[128];

	if (currentCommand.length() > 1 && currentCommand[1] == '!')
	{
		#ifdef _WIN32
			std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(currentCommand.substr(2).c_str(), "r"), _pclose);
		#else
			std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(currentCommand.substr(2).c_str(), "r"), pclose);
		#endif

		if (pipe)
		{

			while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
				result += buffer;
			}
		}
	}
	currentCommand.clear();
	//std::cout << result << '\n';
	return result;
}