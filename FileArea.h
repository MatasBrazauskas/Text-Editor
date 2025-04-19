#pragma once
#include "SDL.h"
#include "FontAndColors.h"
#include <vector>

class FileArea
{
public:
	FileArea();
	~FileArea();

	size_t starting_X, starting_Y, ending_X, ending_Y;
	std::vector<char*> loadedFilesPath;

	void DisplayFileArea(SDL_Renderer* renderer, FontAndColors* color);
};
