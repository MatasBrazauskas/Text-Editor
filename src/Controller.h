#pragma once

#include "ErrorArea.h"
#include "FileArea.h"
#include "TextArea.h"
#include "CommandLineArea.h"
#include "ThreadPool.h"

class Controller
{
public:
	Controller(SDL_Renderer* renderer);
	~Controller() noexcept;

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
	ThreadPool* pool = nullptr;
	FontAndColors* fontAndColors = nullptr;
	CommandLineArea* commandLineArea = nullptr;
};
