#pragma once
#include "SDL.h"
#include <vector>
#include <string>

class CommandLineArea
{
public:
	CommandLineArea();
	~CommandLineArea();

	size_t starting_X, starting_Y, ending_X, ending_Y;
	std::vector<std::string> commandLineMessage;
	std::string currentCommand;

	void DisplayFileArea(SDL_Renderer* renderer, SDL_Color color);
};
