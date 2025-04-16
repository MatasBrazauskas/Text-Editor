#pragma once

#include "SDL.h"
#include "SDL_ttf.h"

class FontAndColors
{
public:
	FontAndColors();
	~FontAndColors();

	enum class Colors {BACKGROUND_COLOR, TEXT_COLOR};
private:
	TTF_Font* TTFont = nullptr;
	SDL_Color editorColors[2];

	SDL_Color GetColor(Colors color);
};
