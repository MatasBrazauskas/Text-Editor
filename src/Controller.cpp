#include "Controller.h"
#include <iostream>
#include <print>

Controller::Controller(SDL_Renderer* renderer)
{
    fontAndColors = new FontAndColors();
    errorArea = new ErrorArea();
    fileArea = new FileArea();
    textArea = new TextArea(fontAndColors);
    commandLineArea = new CommandLineArea();

    this->renderer = renderer;

	errorArea->DisplayFileArea(renderer, fontAndColors);
    fileArea->DisplayFileArea(renderer, fontAndColors);
    textArea->DisplayTextArea(renderer, fontAndColors);
    commandLineArea->DisplayShellInput(renderer, fontAndColors, (int)currentMode);
    commandLineArea->DisplayShellOutput(renderer, fontAndColors, textArea, runLoop);
}

Controller::~Controller()
{

}	
// Reshape the structure of a mode and key mode placement -> mode then key mode.
void Controller::DistributeCommands()
{
    SDL_Event e;
    SDL_StartTextInput();

    bool skipNextTextInput = false;
    bool updateTextArea = false;
	bool displayCommandLine = false;

    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT) // Close window
            runLoop = false;

        if (e.type == SDL_KEYDOWN)
        {
            updateTextArea = true;
            displayCommandLine = true;
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
                    //textArea->AppendAndMoveToLine();
					textArea->AppendAndCopyToLine();
                }
                else if (e.key.keysym.sym == SDLK_TAB)
                {
                    textArea->InsertNearTheCursor(fontAndColors, "\t");
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
                        textArea->CursorFromLeft(fontAndColors);
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
                    }
                }
                break;
            case (int)mode::COMMAND:
                if (e.key.keysym.sym == SDLK_BACKSPACE)
                {
                    commandLineArea->DeleteToCommand();
					commandLineArea->DisplayShellInput(renderer, fontAndColors, (int)currentMode);
                    displayCommandLine = true;
                }
                else if (e.key.keysym.sym == SDLK_RETURN)
                {
                    currentMode = mode::NORMAL;
					displayCommandLine = true;
                    updateTextArea = true;
					fileArea->DisplayFileArea(renderer, fontAndColors);
                    commandLineArea->DisplayShellOutput(renderer, fontAndColors, textArea, runLoop);
                    commandLineArea->DisplayShellInput(renderer, fontAndColors, (int)currentMode);
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
                textArea->InsertNearTheCursor(fontAndColors, std::string(1, e.text.text[0]));
                break;

            case (int)mode::NORMAL:
                if (e.text.text[0] == '$')
                {
                    textArea->MoveCursorToEnd(fontAndColors);
                    updateTextArea = true;
                }
                break;
            case (int)mode::COMMAND:
                if (e.type == SDL_TEXTINPUT)
                {
                    commandLineArea->AppendToCommand(e.text.text[0]);
					commandLineArea->DisplayShellInput(renderer, fontAndColors, (int)currentMode);
                    displayCommandLine = true;
                }
                break;
            }
        }
    }
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
