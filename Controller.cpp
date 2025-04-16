#include "Controller.h"
#include <iostream>

Controller::Controller(SDL_Renderer* renderer)
{
	errorArea = new ErrorArea();
	fileArea = new FileArea();
	textArea = new TextArea();
	commandLineArea = new CommandLineArea();
    fontAndColors = new FontAndColors();


    this->renderer = renderer;

	errorArea->DisplayFileArea(renderer, FontAndColors::GetColor(FontAndColors::Colors::BACKGROUND_ERROR_AREA_COLOR));
    fileArea->DisplayFileArea(renderer, FontAndColors::GetColor(FontAndColors::Colors::BACKGROUND_FILE_AREA_COLOR));
	textArea->DisplayFileArea(renderer, FontAndColors::GetColor(FontAndColors::Colors::BACKGROUND_TEXT_AREA_COLOR), FontAndColors::GetColor(FontAndColors::Colors::TEXT_COLOR), fontAndColors->TTFont);
    commandLineArea->DisplayFileArea(renderer, FontAndColors::GetColor(FontAndColors::Colors::BACKGROUND_COMMAND_LINE_AREA_COLOR));
}

Controller::~Controller()
{

}	

void Controller::DistributeCommands()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        switch (e.type)
        {
        case SDL_QUIT:
            runLoop = false;  // Close the window
            break;

        case SDL_KEYDOWN:
            // Check if SHIFT key is pressed for ':' key
            if (e.key.keysym.sym == SDLK_SEMICOLON && (e.key.keysym.mod & KMOD_SHIFT))
            {
                currentMode = mode::COMMAND;  // Switch to COMMAND mode on ':'
            }
            // Handle other key inputs
            switch (e.key.keysym.sym)
            {
            case SDLK_ESCAPE: currentMode = mode::NORMAL; break;  // Switch to NORMAL mode on ESC
            case SDLK_i:
            case SDLK_a:
            case SDLK_o: currentMode = mode::INSERT; break; // Switch to INSERT mode for 'i', 'a', or 'o'
            case SDLK_v: currentMode = mode::VISUAL; break;  // Switch to VISUAL mode for 'v'
            }
            break;
        }
    }
	//std::cout << (int)currentMode << std::endl;
}

bool Controller::RunLoop()
{
	return runLoop;
}
