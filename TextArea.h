#pragma once
#include "SDL.h"
//#include "SDL_ttf.h"
#include "FontAndColors.h"
#include <vector>
#include <string>
#include <unordered_map>

class TextArea
{
public:
	TextArea();
	~TextArea();

	struct textFileInformation
	{
		size_t currentRow{};
		size_t currentColumn{};
		std::vector<std::string> text;
	};

	size_t starting_X, starting_Y, ending_X, ending_Y;
	std::string currentFileName = "";

	std::vector<std::string> activeFiles;
	std::unordered_map<std::string, textFileInformation> filesHashMap;

	size_t cursorWidht{}, cursor_Height{};
	size_t cursorX = 12, cursorY = 12;

	int CXOffset, CYOffset;
	int offTheEdgeX = 10, offTheEdgeY = 10;
	const int TEMPYOFFSETFORCURSOR = 24;

	bool showNumbers = false;
	bool relativeLineNumbers = false;

	std::string LineNumbers(size_t index);

	void DisplayTextArea(SDL_Renderer* renderer, FontAndColors* colors);

	void ReadCurrentFile();
	void WriteIntoCurrentFile();

	void InsertNearTheCursor(FontAndColors* color, const char letter);
	void DeleteCurrentLetter(FontAndColors* color);

	void DisplayCursor(SDL_Renderer* renderer, FontAndColors* colors, int displayMode);

	void MoveCursor(FontAndColors* color, const int x_offset, const int y_offset);
	void MoveCursorToEnd(FontAndColors* color);

	void AppendAndMoveToLine();
	void CursorFromRight(FontAndColors* color);

	void LoadOtherFile(const std::string& filesName);
};
