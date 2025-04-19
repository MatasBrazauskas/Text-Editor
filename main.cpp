#include "SDL.h"
#include "SDL_image.h"
//#include <SDL_ttf.h>
#include <iostream>

#include "Controller.h"

int main(int argc, char** argv) 
{
	// Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

	// Initialize SDL_Window for video
    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Window",
        SDL_WINDOWPOS_UNDEFINED,               
        SDL_WINDOWPOS_UNDEFINED,               
        800,                                   
        600,                                  
        SDL_WINDOW_RESIZABLE                     
    );

	// Check if the window was created successfully
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

	//Set the window to fullscreen and then maximize it
    SDL_SetWindowFullscreen(window, 0);
	//Now maximize the window
    SDL_MaximizeWindow(window);

	// Create a renderer for the window
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

	// Set the draw color to white (RGB)
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);

    //Clear the window with selected color
    SDL_RenderClear(renderer);
    //Updates the window with selected color
    SDL_RenderPresent(renderer);

	Controller* controller = new Controller(renderer);

    //Main loop
    while (controller->RunLoop()) {
        controller->DistributeCommands();
    }

    //Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
