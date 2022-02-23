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
SDL_Renderer* renderer = NULL;
SDL_Texture *texture1 = NULL;
void logSDLError(std::ostream& os, const std::string &msg, bool fatal){
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}
bool create_PNG(){
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)){
        logSDLError(cout, "SDL_image could not be initialized", true);
        return false;
    }
    return true;
}
bool create_Renderer() {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        logSDLError(cout, "Renderer coud not be created", 1);
        return false;
    }
    return true;
}
bool initSDL(SDL_Window* &window) {
    bool success = true;
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
            success = create_PNG();
            if (! success) return false;
            success = create_Renderer();
            if (! success) return false;
        }
    }
    return true;
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
SDL_Texture *loadTexture(string path) {
    SDL_Texture *newTexture = NULL;
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        logSDLError(cout, "Unable to load image", 1);
    }
    else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL)
            logSDLError(cout, "Unable to load Texture", 1);
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}
bool loadMedia() {
    bool success = true;
    texture1 = loadTexture("GameHKI/welcome.png");
    if (texture1 == NULL) {
        success = false;
        logSDLError(cout, "Unable to load texture", true);
    }
    return success;
}
void close(){
    SDL_DestroyTexture(texture1);
    texture1 = NULL;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;
    IMG_Quit();
	SDL_Quit();


}
int main(int argc, char* argv[])
{
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
            int cnt = 0;
            while (!quit) {
                while(SDL_PollEvent(&e) != 0){
                    if (e.type == SDL_QUIT)
                        quit = true;
                }
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture1, NULL, NULL);
                SDL_RenderPresent(renderer);
            }
        }
    }
    //waitUntilKeyPressed();
    //quitSDL(window, renderer);
    return 0;
}
