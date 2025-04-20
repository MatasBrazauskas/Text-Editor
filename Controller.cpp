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
    SDL_StartTextInput();

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
                else if (e.key.keysym.sym == SDLK_RETURN)
                {
                    textArea->AppendAndMoveToLine();
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
                        textArea->CursorFromRight(fontAndColors);
                        skipNextTextInput = true;
                        break;
                    case SDLK_o:
                        currentMode = mode::INSERT;
                        textArea->AppendAndMoveToLine();
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
                    case SDLK_p :break;
                    }
                }
                break;
            case (int)mode::COMMAND:
                if (e.key.keysym.sym == SDLK_BACKSPACE)
                {
                    commandLineArea->DeleteToCommand();
                }
                else if (e.type == SDL_TEXTINPUT)
                {
                    commandLineArea->AppendToCommand(e.text.text[0]);
                }
                break;
            }
        }
        else if (e.type == SDL_TEXTINPUT)
        {
            switch ((int)currentMode)
            {
            case (int)mode::INSERT:
                if (skipNextTextInput) {
                    skipNextTextInput = false;
                    continue;
                }
                textArea->InsertNearTheCursor(fontAndColors, e.text.text[0]); // Get actual typed character
                break;

            case (int)mode::NORMAL:
                if (e.text.text[0] == '$')
                {
                    textArea->MoveCursorToEnd(fontAndColors);
                    updateTextArea = true;
                }
            }
        }
    }
    //Temp display of a text area
    if (updateTextArea)
    {
        textArea->DisplayTextArea(renderer, fontAndColors);
        textArea->DisplayCursor(renderer, fontAndColors, (int)currentMode);
    }
}

bool Controller::RunLoop()
{
	return runLoop;
}
