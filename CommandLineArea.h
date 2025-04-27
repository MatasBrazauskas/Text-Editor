#pragma once
#include "SDL.h"
#include "FontAndColors.h"
#include "TextArea.h"
#include <vector>
#include <string>
#include <optional>

class CommandLineArea
{
public:
	CommandLineArea();
	~CommandLineArea();

	size_t starting_X, starting_Y, ending_X, ending_Y;
	std::vector<std::string> commandLineMessage;

	std::string currentCommand = "";

	void DisplayShellInput(SDL_Renderer* renderer, FontAndColors* color, int mode);
	void DisplayShellOutput(SDL_Renderer* renderer, FontAndColors* color, TextArea* textArea, bool& closeWindow);

	void AppendToCommand(const char letter);
	void DeleteToCommand();
		
	std::optional<std::string> ExucuteAndDisplayCommand(TextArea* textArea, bool& closeWindows);
};
