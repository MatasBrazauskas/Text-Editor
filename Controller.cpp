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

	errorArea->DisplayFileArea(renderer, FontAndColors::GetColor(FontAndColors::Colors::BACKGROUND_AREA_COLOR));
    fileArea->DisplayFileArea(renderer, FontAndColors::GetColor(FontAndColors::Colors::BACKGROUND_AREA_COLOR));
    textArea->DisplayFileArea(renderer, FontAndColors::GetColor(FontAndColors::Colors::BACKGROUND_AREA_COLOR), FontAndColors::GetColor(FontAndColors::Colors::TEXT_COLOR), fontAndColors->TTFont);
    textArea->DisplayCursor(renderer);
    commandLineArea->DisplayFileArea(renderer, FontAndColors::GetColor(FontAndColors::Colors::BACKGROUND_AREA_COLOR));
}

Controller::~Controller()
{

}	

void Controller::DistributeCommands()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if(e.type == SDL_QUIT)
            runLoop = false;  // Close the window
        if(e.key.keysym.sym == SDLK_ESCAPE)
            currentMode = mode::NORMAL;

        /*case SDL_KEYDOWN:
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
        }*/

        switch ((int)currentMode)
        {
			case (int)mode::INSERT:
				if (e.type == SDL_KEYDOWN)
				{
					textArea->InsertNearTheCursor(e.key.keysym.sym);
					std::cout << "Need to insert: " << e.key.keysym.sym << '\n';
					textArea->DisplayFileArea(renderer, FontAndColors::GetColor(FontAndColors::Colors::BACKGROUND_AREA_COLOR), FontAndColors::GetColor(FontAndColors::Colors::TEXT_COLOR), fontAndColors->TTFont);
				}
            case (int)mode::NORMAL:
                if (e.key.keysym.sym == SDLK_SEMICOLON && (e.key.keysym.mod & KMOD_SHIFT))
                {
                    currentMode = mode::COMMAND;  // Switch to COMMAND mode on ':'
                }
                // Handle other key inputs
                switch (e.key.keysym.sym)
                {
					case SDLK_i: currentMode = mode::INSERT; break;
                    case SDLK_a: currentMode = mode::INSERT; textArea->cursorColumn++; break;
					case SDLK_o: currentMode = mode::INSERT; break; // Switch to INSERT mode for 'i', 'a', or 'o'
					case SDLK_v: currentMode = mode::VISUAL; break;  // Switch to VISUAL mode for 'v'
					}
                break;
        }
    }
    textArea->DisplayCursor(renderer);
	std::cout << (int)currentMode << std::endl;
}

bool Controller::RunLoop()
{
	return runLoop;
}
