#pragma once

#include <vector>

class FileArea
{
public:
	FileArea();
	~FileArea();
private:

	size_t strarting_X, starting_Y, ending_X, ending_Y;
	std::vector<char*> loadedFilesPath;
};
