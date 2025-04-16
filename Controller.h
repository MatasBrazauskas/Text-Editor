#pragma once

#include "ErrorArea.h"
#include "FileArea.h"
#include "TextArea.h"
#include "FontAndColors.h"
#include "CommandLineArea.h";

class Controller
{
public:
	Controller();
	~Controller();

private:
	ErrorArea* errorArea = nullptr;
	FileArea* fileArea = nullptr;
	TextArea* textArea = nullptr;
	FontAndColors* fontAndColors = nullptr;
	CommandLineArea* commandLineArea = nullptr;

	enum class mode {NORMAL, INSET, VISUAL, COMMAND };
	enum class currentActive {TEXT_AREA, COMMAND_LINE_AREA};


};
