#include "FontAndColors.h"

SDL_Color FontAndColors::editorColors[5];

FontAndColors::FontAndColors()
{
	editorColors[0] = { 13, 17, 23, 255 };
	editorColors[1] = { 0, 0, 255, 255 };
	editorColors[2] = { 0, 255, 0, 255 };
	editorColors[3] = { 255, 0, 0, 255 };
	editorColors[4] = { 255, 255, 255, 255 };

	TTF_Init();
	TTFont = TTF_OpenFont("C:\\Users\\matas\\Desktop\\Roboto.ttf", 24);
}

FontAndColors::~FontAndColors()
{
	// Destructor implementation
}

SDL_Color FontAndColors::GetColor(Colors color)
{
	return editorColors[static_cast<int>(color)];
}