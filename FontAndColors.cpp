#include "FontAndColors.h"

FontAndColors::FontAndColors()
{
	editorColors[0] = { 13, 17, 23, 255 };
	editorColors[1] = { 255, 255, 255, 255 };

	TTF_Init();
	TTFont = TTF_OpenFont("C:\\Users\\matas\\Desktop\\Roboto.ttf", CFontHeight);
}

FontAndColors::~FontAndColors()
{
	// Destructor implementation
}

SDL_Color FontAndColors::GetColor(Colors color)
{
	return editorColors[static_cast<int>(color)];
}