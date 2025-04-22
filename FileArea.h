#pragma once
#include "SDL.h"
#include "FontAndColors.h"
#include <vector>
#include <string>

class FileArea
{
public:
	FileArea();
	~FileArea();

	size_t starting_X, starting_Y, ending_X, ending_Y;
	std::vector<std::string> loadedFilesPath;
	std::string path = "C:\\Users\\matas\\Desktop\\ForEditor";

	void DisplayFileArea(SDL_Renderer* renderer, FontAndColors* color);
};
