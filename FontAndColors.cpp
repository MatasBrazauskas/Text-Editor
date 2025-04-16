#include "FontAndColors.h"

FontAndColors::FontAndColors()
{
	editorColors[0] = { 0, 0, 0, 255 };
	editorColors[1] = { 255, 255, 255, 255 };
}

FontAndColors::~FontAndColors()
{
	// Destructor implementation
}

SDL_Color FontAndColors::GetColor(Colors color)
{
	return editorColors[static_cast<int>(color)];
}