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

class Ltexture {
private:
    SDL_Texture* MainTexture;
    int MainW, MainH;
public:
    Ltexture(){
        MainH = 0;
        MainW = 0;
        MainTexture =NULL;
    }
    ~Ltexture(){
        free();
    }
    bool loadFromFile(string path);
    void free() {
        if (MainTexture != NULL){
            SDL_DestroyTexture(MainTexture);
            MainTexture = NULL;
            MainH = MainW = 0;
        }
    }
    //render texture at given point;
    void render(int x, int y, SDL_Rect *clip = NULL);
    int getH(){return MainH;}
    int getW(){return MainW;}
};
SDL_Rect gSpriteClip;
Ltexture gSpriteSheetTexture;
void Ltexture :: render(int x, int y, SDL_Rect* clip) {
    //Set rendering space
    SDL_Rect renderQuad = {x, y, MainW, MainH};
    //set clip rendering dimension:
    if (clip != NULL) {
        renderQuad.h = clip->h;
        renderQuad.w = clip->w;
    }
    //to Screen
    SDL_RenderCopy(renderer, MainTexture, clip, &renderQuad);
}
bool Ltexture :: loadFromFile(string path) {
    free(); // clean preTexture

    SDL_Texture *newTexture = NULL;
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        logSDLError(cout, "Unable to load image", 1);
    }
    else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL)
            logSDLError(cout, "Unable to load Texture", 1);
        MainH = loadedSurface->h;
        MainW = loadedSurface->w;
        SDL_FreeSurface(loadedSurface);
    }
    MainTexture = newTexture;
    return MainTexture != NULL;
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
bool loadMedia() {
    if (! gSpriteSheetTexture.loadFromFile("GameHKI/lastmap3.png")){
        logSDLError(cout, "Unable to load sheet texture", 1);
        return false;
    }
    else {
        // set top left sprite
       gSpriteClip.h = 630;
       gSpriteClip.w = 1120;
       gSpriteClip.x = 690;
       gSpriteClip.y = 935;
    }
    return true;
}
void close(){
    gSpriteSheetTexture.free();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
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
                    else {
                        if (e.key.keysym.sym == SDLK_DOWN){
                            gSpriteClip.y = min(1870, gSpriteClip.y + 10);
                        }
                        else if (e.key.keysym.sym == SDLK_UP){
                            gSpriteClip.y = max(0, gSpriteClip.y - 10);
                        }
                        else if (e.key.keysym.sym == SDLK_RIGHT){
                             gSpriteClip.x = min(1380, gSpriteClip.x + 10);
                        }
                        else if (e.key.keysym.sym == SDLK_LEFT){
                            gSpriteClip.x = max(0, gSpriteClip.x - 10);
                        }
                    }
                }
                SDL_RenderClear(renderer);
                //render
                gSpriteSheetTexture.render(0, 0, &gSpriteClip);
                SDL_RenderPresent(renderer);
            }
        }
    }
    //waitUntilKeyPressed();
    //quitSDL(window, renderer);
    return 0;
}
