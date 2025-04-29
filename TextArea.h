#pragma once
#include "SDL.h"
#include "FontAndColors.h"
#include <vector>
#include <string>
#include <unordered_map>

class TextArea
{
public:
	TextArea(FontAndColors* color);
	~TextArea();

	struct textFileInfo
	{
		size_t Row{};
		size_t Col{};
		std::vector<std::string> text;
	};

	size_t starting_X, starting_Y, ending_X, ending_Y;
	std::string currentFileName = "";

	std::vector<std::string> activeFiles; // maybe remove this
	std::unordered_map<std::string, textFileInfo> filesHashMap;

	int charWidth, charHeight;

	size_t offTheEdgeX, offTheEdgeY;

	enum class Offsets { pixelsBetweenLines = 2 , cursorWidth = 2};

	bool showNumbers = false, relativeLineNumbers = false;

	std::string LineNumbers(size_t index);

	void DisplayTextArea(SDL_Renderer* renderer, FontAndColors* colors);

	void ReadCurrentFile();
	void WriteIntoCurrentFile();

	void InsertNearTheCursor(FontAndColors* color, std::string letter);
	void DeleteCurrentLetter(FontAndColors* color);

	void DisplayCursor(SDL_Renderer* renderer, FontAndColors* colors, const int displayMode);

	void MoveCursor(FontAndColors* color, const int x_offset, const int y_offset);
	void MoveCursorToEnd(FontAndColors* color);

	void AppendAndMoveToLine();
	void CursorFromRight(FontAndColors* color);

	void LoadOtherFile(const std::string& filesName);
	void CloseFile();

	void ChangeCurrentFile(int index);
	void JumpToBuffer(const std::string& index);
};
