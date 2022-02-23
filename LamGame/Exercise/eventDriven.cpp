#include <iostream>
#include <SDL.h>
#include <Windows.h>
#include <string.h>
using namespace std;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Hunting Treasure";
// key press surface constants;
enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};
void logSDLError(std::ostream& os, const std::string &msg, bool fatal){
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       //SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);

    //Khi chạy trong môi trường bình thường (không chạy trong máy ảo)
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    //Khi chạy ở máy ảo (ví dụ tại máy tính trong phòng thực hành ở trường)
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
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

SDL_Surface *gKeyPressSurface[KEY_PRESS_SURFACE_TOTAL], *ScreenSurface = NULL, *CurrentSurface = NULL;
SDL_Surface *loadSurface(string path) {
    SDL_Surface *SF = SDL_LoadBMP(path.c_str());
    if (SF == NULL) {
        logSDLError(cout, "Unable to load image", true);
    }
    return SF;
}
bool loadMedia() {
    bool success = true;
    //default
    gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("Pic/press.bmp");
    if (gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT] == NULL){
        logSDLError(cout, "Unable to load image", true);
        success = false;
    }
    //up
    gKeyPressSurface[KEY_PRESS_SURFACE_UP] = loadSurface("Pic/up.bmp");
    if (gKeyPressSurface[KEY_PRESS_SURFACE_UP] == NULL){
        logSDLError(cout, "Unable to load image", true);
        success = false;
    }
    //down
    gKeyPressSurface[KEY_PRESS_SURFACE_DOWN] = loadSurface("Pic/down.bmp");
  if (gKeyPressSurface[KEY_PRESS_SURFACE_DOWN] == NULL){
        logSDLError(cout, "Unable to load image", true);
        success = false;
    }
    //left
    gKeyPressSurface[KEY_PRESS_SURFACE_LEFT] = loadSurface("Pic/left.bmp");
    if (gKeyPressSurface[KEY_PRESS_SURFACE_LEFT] == NULL){
        logSDLError(cout, "Unable to load image", true);
        success = false;
    }
    //right
    gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT] = loadSurface("Pic/right.bmp");
    if (gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT] == NULL){
        logSDLError(cout, "Unable to load image", true);
        success = false;
    }
    return success;
}
void close(){
    for (int i =0 ; i < KEY_PRESS_SURFACE_TOTAL; i++){
        SDL_FreeSurface(gKeyPressSurface[i]);
        gKeyPressSurface[i] = NULL;
    }
}
int main(int argc, char* argv[])
{
    SDL_Renderer* renderer;
    SDL_Window *window = NULL;
    initSDL(window, renderer);
    ScreenSurface = SDL_GetWindowSurface(window);
    // Your drawing code here
    // use SDL_RenderPresent(renderer) to show it
    bool quit = false;
    SDL_Event e;
    // set default
    if (! loadMedia()) {
        logSDLError(cout, "Unable to load image", true);
    }
    else {
        CurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];
        SDL_Rect sRect;
        sRect.x = 0;
        sRect.y = 0;
        sRect.w = SCREEN_WIDTH;
        sRect.h = SCREEN_HEIGHT;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT){
                    quit = true;
                }
                else {
                    if (e.type == SDL_KEYDOWN){
                        switch(e.key.keysym.sym){
                            //up
                            case SDLK_UP:
                            CurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_UP];
                            break;
                            //down
                            case SDLK_DOWN:
                            CurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DOWN];
                            break;
                            //left
                            case SDLK_LEFT:
                            CurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_LEFT];
                            break;
                            //right
                            case SDLK_RIGHT:
                            CurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT];
                            break;
                        }
                    }
                }
            }

            SDL_BlitScaled(CurrentSurface, NULL, ScreenSurface, &sRect);
            SDL_UpdateWindowSurface(window);
        }
    }
//    waitUntilKeyPressed();
//    quitSDL(window, renderer);
    return 0;
}
