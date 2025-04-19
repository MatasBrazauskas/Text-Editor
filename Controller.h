#pragma once

#include "ErrorArea.h"
#include "FileArea.h"
#include "TextArea.h"
//#include "FontAndColors.h"
#include "CommandLineArea.h"

class Controller
{
public:
	Controller(SDL_Renderer* renderer);
	~Controller();

	bool RunLoop();
	void DistributeCommands();

	enum class mode { NORMAL, INSERT, VISUAL, COMMAND };
	enum class currentActive { TEXT_AREA, COMMAND_LINE_AREA };

	bool runLoop = true;
	mode currentMode = mode::NORMAL;

	SDL_Renderer* renderer = nullptr;

	ErrorArea* errorArea = nullptr;
	FileArea* fileArea = nullptr;
	TextArea* textArea = nullptr;
	FontAndColors* fontAndColors = nullptr;
	CommandLineArea* commandLineArea = nullptr;
};
