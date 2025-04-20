#include "FontAndColors.h"

FontAndColors::FontAndColors()
{
	editorColors[0] = { 13, 17, 23, 255 };
	editorColors[1] = { 255, 255, 255, 255 };
	editorColors[2] = { 0,0,0,255 };

	TTF_Init();
	//TTFont = TTF_OpenFont("C:\\Users\\matas\\Desktop\\Roboto.ttf", CFontHeight);
	TTFont = TTF_OpenFont("C:\\Users\\matas\\Desktop\\CONSOLA.TTF", CFontHeight);
	filesAreaTTFont = TTF_OpenFont("C:\\Users\\matas\\Desktop\\CONSOLA.TTF", 16);
}

FontAndColors::~FontAndColors()
{
	// Destructor implementation
}

SDL_Color FontAndColors::GetColor(Colors color)
{
	return editorColors[static_cast<int>(color)];
}