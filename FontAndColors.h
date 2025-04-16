#pragma once

#include "SDL.h"
#include "SDL_ttf.h"

class FontAndColors
{
public:
	FontAndColors();
	~FontAndColors();

	enum class Colors { BACKGROUND_TEXT_AREA_COLOR, BACKGROUND_FILE_AREA_COLOR, BACKGROUND_COMMAND_LINE_AREA_COLOR, BACKGROUND_ERROR_AREA_COLOR, TEXT_COLOR };
	static SDL_Color GetColor(Colors color);
	static SDL_Color editorColors[5];

	TTF_Font* TTFont = nullptr;
};
