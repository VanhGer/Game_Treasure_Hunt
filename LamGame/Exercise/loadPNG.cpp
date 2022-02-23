#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <Windows.h>
#include <string.h>
using namespace std;
const int SCREEN_WIDTH = 1120;
const int SCREEN_HEIGHT = 630;
const string WINDOW_TITLE = "Hunting Treasure";
SDL_Surface *WindowSurface = NULL;
SDL_Surface* gPNGSurface = NULL;
SDL_Window *window = NULL;
void logSDLError(std::ostream& os, const std::string &msg, bool fatal){
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}
bool initSDL(SDL_Window* &window) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        logSDLError(std::cout, "SDL_Init", true);
        return false;
    }
    else {
        window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
           SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            logSDLError(std::cout, "CreateWindow", true);
            return false;
        }
        else {
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags)){
                logSDLError(cout, "SDL_image could not initialize", true);
                return false;
            }
        }
    }
    return true;
}
void quitSDL(SDL_Window* window, SDL_Renderer* renderer){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void waitUntilKeyPressed(){
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}
void close(){
//Free loaded image
	SDL_FreeSurface( gPNGSurface );
	gPNGSurface = NULL;
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
SDL_Surface* loadSurface( std::string path ){
    SDL_Surface *optimizedSurface = NULL, *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        logSDLError(cout, "Unable to load image", true);
    }
    else {
        optimizedSurface = SDL_ConvertSurface(loadedSurface, WindowSurface -> format, 0);
        if (optimizedSurface == NULL) {
            logSDLError(cout, "Unable to optimize image", true);
        }
        SDL_FreeSurface(loadedSurface);
    }
    return optimizedSurface;

}
bool loadMedia() {
    bool success = true;
    gPNGSurface = loadSurface("GameHKI/welcome.png");
    if (gPNGSurface == NULL) {
        success = false;
        logSDLError(cout, "Unable to load image", true);
    }
    return success;
}

int main(int argc, char* argv[])
{
    SDL_Renderer* renderer;
    if (! initSDL(window)) {
        logSDLError(cout, "Can not initialize \n", 1);
    }
    else {
        WindowSurface = SDL_GetWindowSurface(window);
    // Your drawing code here
    // use SDL_RenderPresent(renderer) to show it
        SDL_Rect SRect;
        SRect.x = 0;
        SRect.y = 0;
        SRect.w = SCREEN_WIDTH;
        SRect.h = SCREEN_HEIGHT;
        if (!loadMedia()){
            logSDLError(cout, "Can not load media \n", 1);
        }
        else {
            SDL_Event e;
            bool quit = false;
            while (!quit) {
                while(SDL_PollEvent(&e) != 0){
                    if (e.type == SDL_QUIT)
                        quit = true;
                }

                SDL_BlitScaled(gPNGSurface, NULL, WindowSurface, &SRect);
                SDL_UpdateWindowSurface(window);
            }
        }
    }
    //waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;
}
