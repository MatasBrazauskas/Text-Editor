#pragma once

#include "SDL.h"
#include "SDL_ttf.h"

class FontAndColors
{
public:
	FontAndColors();
	~FontAndColors();

	enum class Colors { BACKGROUND_AREA_COLOR, TEXT_COLOR };
	static SDL_Color GetColor(Colors color);
	static SDL_Color editorColors[2];

	TTF_Font* TTFont = nullptr;
};
