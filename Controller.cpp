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

	errorArea->DisplayFileArea(renderer, fontAndColors);
    fileArea->DisplayFileArea(renderer, fontAndColors);
    textArea->DisplayTextArea(renderer, fontAndColors);
    commandLineArea->DisplayFileArea(renderer, fontAndColors);
}

Controller::~Controller()
{

}	

void Controller::DistributeCommands()
{
    SDL_Event e;
    bool skipNextTextInput = false;
    bool updateTextArea = false;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT) // Close window
            runLoop = false;

        if (e.type == SDL_KEYDOWN)
        {
            updateTextArea = true;
            if (e.key.keysym.sym == SDLK_ESCAPE) // Switch modes
                currentMode = mode::NORMAL;

            switch ((int)currentMode)
            {
            case (int)mode::INSERT:
                if (e.key.keysym.sym == SDLK_BACKSPACE)
                {
                    textArea->DeleteCurrentLetter(fontAndColors); // Delete current cursor position
                }
                break;

            case (int)mode::NORMAL:
                if (e.key.keysym.sym == SDLK_SEMICOLON && (e.key.keysym.mod & KMOD_SHIFT))
                {
                    currentMode = mode::COMMAND;
                }
                else
                {
                    switch (e.key.keysym.sym)
                    {
                    case SDLK_i: // Switch to insert mode
                        currentMode = mode::INSERT;
                        skipNextTextInput = true;
                        break;
                    case SDLK_a:
                        currentMode = mode::INSERT;
                        skipNextTextInput = true;
                        break;
                    case SDLK_o:
                        currentMode = mode::INSERT;
                        skipNextTextInput = true;
                        break;
                    case SDLK_v: // Switch to visual mode
                        currentMode = mode::VISUAL;
                        break;
                    case SDLK_l:
                        textArea->MoveCursor(fontAndColors, 1, 0); // Move to left
                        break;
                    case SDLK_h:
                        textArea->MoveCursor(fontAndColors, -1, 0); // Move to left
                        break;
                    case SDLK_k:
                        textArea->MoveCursor(fontAndColors, 0, -1); 
                        break;
                    case SDLK_j:
                        textArea->MoveCursor(fontAndColors, 0, 1); 
                        break;
                    }
                }
                break;
            }
        }
        else if (e.type == SDL_TEXTINPUT && currentMode == mode::INSERT)
        {
            if (skipNextTextInput) {
                skipNextTextInput = false;
                continue;
            }
            textArea->InsertNearTheCursor(fontAndColors, e.text.text[0]); // Get actual typed character
        }
    }
    //Temp display of a text area
    if (updateTextArea)
    {
        textArea->DisplayTextArea(renderer, fontAndColors);
        textArea->DisplayCursor(renderer, fontAndColors, (int)currentMode);
    }
	//std::cout << (int)currentMode << std::endl;
}

bool Controller::RunLoop()
{
	return runLoop;
}
