#pragma once

//#include "SDL.h"
#include "SDL_ttf.h"

class FontAndColors
{
public:
	FontAndColors();
	~FontAndColors();

	enum class Colors { BACKGROUND_AREA_COLOR, TEXT_COLOR, OPPOSITE_TEXT_COLOR };
	SDL_Color GetColor(Colors color);
	SDL_Color editorColors[3];

	const int CFontHeight = 24;

	TTF_Font* TTFont = nullptr;
};
