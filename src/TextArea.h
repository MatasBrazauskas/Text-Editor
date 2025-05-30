#pragma once
#include "SDL.h"
#include "FontAndColors.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <string_view>

class TextArea
{
public:
	TextArea(FontAndColors* color);
	~TextArea() noexcept;

	struct textFileInfo
	{
		size_t Row{};
		size_t Col{};
		std::vector<std::string> text;
	};

	size_t starting_X, starting_Y, ending_X, ending_Y;
	const size_t tabLen = 4;
	const std::string Tab = std::string(tabLen, ' ');
	enum class Offsets { pixelsBetweenLines = 2, cursorWidth = 2 };
	bool showNumbers = false, relativeLineNumbers = true;

	std::string currentFileName;

	std::vector<std::string> activeFiles;
	std::unordered_map<std::string, textFileInfo> filesHashMap;

	int charWidth, charHeight;

	size_t offTheEdgeX, offTheEdgeY;

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
	void AppendAndCopyToLine();

	void CursorFromLeft(FontAndColors* color);
	void CursorFromRight(FontAndColors* color);

	void LoadOtherFile(const std::string& filesName);
	void CloseFile();

	void ChangeCurrentFile(int index);
	void JumpToBuffer(std::string_view index);
};
