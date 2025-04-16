#pragma once

#include <vector>
#include <string>
#include <unordered_map>

class TextArea
{
public:
	TextArea();
	~TextArea();

private:
	struct textFileInformation
	{
		size_t currentIndex;
		std::vector<std::string> text;
	};

	size_t strarting_X, starting_Y, ending_X, ending_Y;
	std::string currentFileName;
	std::vector<std::string> activeFiles;
	std::unordered_map<std::string, textFileInformation> fileText;

	size_t cursorRow, cursorColumn;
};
