#include "Common_Function.h"
#include "LoadObject.h"
#include "Character.h"
#include "MainCharacter.h"
#include "Enemy.h"
#include "Weapon.h"
#include "TextObject.h"
#include "Map1.h"
#include "Map2.h"
#include "Map3.h"
#include "Map4.h"
#include "Map5.h"
bool create_GRenderer() {
    GRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                      SDL_RENDERER_PRESENTVSYNC);

    if (GRenderer == nullptr) {
            logSDLError(std::cout, "CreateRenderer", true);
            return false;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(GRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    return true;
}
bool create_PNG(){
    SDL_SetRenderDrawColor( GRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)){
        logSDLError(std::cout, "SDL_image could not be initialized", true);
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
            success = create_GRenderer();
            if (! success) return false;
            success = create_PNG();
            if (! success) return false;
            if( TTF_Init() == -1 ){
                logSDLError(std::cout, "TTF could not be initialized", true);
                return false;
            }
        }
    }
    return true;
}
void close(){
    SDL_DestroyRenderer(GRenderer);
    SDL_DestroyWindow(window);
    window = NULL;
	GRenderer = NULL;
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
bool loadFont(){
    gFont = TTF_OpenFont( "font/OpenSans-Regular.ttf", 15);
    LFont = TTF_OpenFont( "font/OpenSans-ExtraBold.ttf", 25);
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		return false;
	}
	return true;
}
bool loadMedia() {
    return loadFont();
}
void Loading(){
    LoadObject loading;
    TextObject Percent;
    loading.loadFromFile("GameHKI/Loading/loading.png", GRenderer);
    Percent.SetColor(TextObject::WHITE_TEXT);
    Uint64 stime = SDL_GetTicks64();
    int cur = 0;
    string str_val;
    while (cur != 100){
        if (SDL_GetTicks64() - stime >= 15){
            SDL_RenderClear(GRenderer);
            loading.render(0, 0, GRenderer, NULL);
            cur++;
            str_val = std::to_string(cur);
            str_val += "%";
            Percent.setText(str_val);
            Percent.LoadFromRenderText(LFont, GRenderer);
            Percent.RenderText(GRenderer, 532, 575);
            stime = SDL_GetTicks64();
            SDL_RenderPresent(GRenderer);
        }
    }
    loading.free(); Percent.Free();
}
bool LoadMainMenu(){
    SDL_Rect RWel;
    BackGround[0].loadFromFile("GameHKI/Welcome/welcome.png", GRenderer);
    RWel = {0, 0, 1120, 630};
    bool Running = true;
    int x, y, inside = 0;
    SDL_Event e;
    while (Running){
        while( SDL_PollEvent( &e ) != 0 ){
            //User requests quit
            if( e.type == SDL_QUIT ){
                Running = false;
            }
            //Handle button events
            if(e.type == SDL_MOUSEBUTTONDOWN){
                SDL_GetMouseState(&x, &y);
                inside = 0;
                if (x >= 254 && x <= 421 && y >= 405 && y <= 442) inside = 2;
                if (x >= 708 && x <= 839 && y >= 405 && y <= 442) inside = 1;
            }
        }
        SDL_RenderClear(GRenderer);
        RWel.x = inside * 1120;
        BackGround[0].render(0, 0, GRenderer, &RWel);
        SDL_RenderPresent(GRenderer);
        if (inside) {
            SDL_Delay(500);
            break;
        }
    }
    BackGround[0].free();
    return (inside == 2);
}
bool LoadGameOver(){
    LoadObject Gover, choose;
    SDL_RenderClear(GRenderer);
    choose.loadFromFile("GameHKI/choose.png", GRenderer);
    Gover.loadFromFile("GameHKI/GameOver.png", GRenderer);
    SDL_Event e;
    int cur = -1; bool Running = true;
    int x, y;
    while (Running) {
        while(SDL_PollEvent(&e) != 0){
            if (e.type == SDL_QUIT){
                choose.free(); Gover.free(); return false;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN){
                SDL_GetMouseState(&x, &y);
                if (380 <= y && y <= 420){
                    if (325 <= x && x <= 430) cur = 0;
                    if (685 <= x && x <= 765) cur = 1;
                }
            }
        }
        if (cur != -1) Running = false;
        Gover.render(0, 0, GRenderer, NULL);
        if (cur != -1) choose.render(330 + cur * 345, 430, GRenderer, NULL);
        SDL_RenderPresent(GRenderer);
        if (! Running) SDL_Delay(1500);

    }
    Gover.free();
    choose.free();
    return 1 - cur;
}
int main(int argc, char* argv[])
{
    if (! initSDL(window)) {
        logSDLError(cout, "Can not initialize \n", 1);
    }
    else{
        WindowSurface = SDL_GetWindowSurface(window);
//    // Your drawing code here
//    // use SDL_RenderPresent(renderer) to show it
        if (!loadMedia()){
            logSDLError(cout, "Can not load media \n", 1);
        }
        else {
            int level = 0;
            bool Running = true;
            while (Running){
                if (level == 0) {
                    if (LoadMainMenu()) level = 1;
                    else Running = false;
                }
                if (!Running) break;
                if (level == 1) {
                    Loading();
                    if (RunMap1(GRenderer, LFont, Running)) level = 2;
                    else {
                        if (!LoadGameOver()) level = 0;
                    }
                }
                if (!Running) break;
                if (level == 2) {
                    Loading();
                    if (RunMap2(GRenderer, LFont, Running)) level = 3;
                    else {
                        if (!LoadGameOver()) level = 0;
                    }
                }
                if (!Running) break;
                if (level == 3) {
                    Loading();
                    if (RunMap3(GRenderer, LFont, Running)) level = 4;
                    else {
                        if (!LoadGameOver()) level = 0;
                    }
                }
                if (!Running) break;
                if (level == 4) {
                    Loading();
                    if (RunMap4(GRenderer, LFont, Running)) level = 5;
                    else {
                        if (!LoadGameOver()) level = 0;
                    }
                }
                if (!Running) break;
                if (level == 5) {
                    Loading();
                    RunMap5(GRenderer, Running);
                    level = 0;
                }
                if (!Running) break;
            }
        }
    }
    return 0;
}
