#include "Common_Function.h"
#include "LoadObject.h"
#include "Character.h"
#include "MainCharacter.h"
#include "Skeleton.h"
#include "Weapon.h"
using namespace std;
mt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());
long long Rand(long long l, long long r){
    return uniform_int_distribution<long long> (l, r) (rng);
}
LoadObject BackGround4, MainExplorer;
MainCharacter Explorer;
Weapon Arrow[4];
Skeleton Ske[30];
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
        }
    }
    return true;
}
void close(){
    BackGround4.free();
    SDL_DestroyRenderer(GRenderer);
    SDL_DestroyWindow(window);
    window = NULL;
	GRenderer = NULL;
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
SDL_Rect Map4;
bool LoadBackGround(string path, SDL_Renderer *ren){
    if (! BackGround4.loadFromFile(path.c_str(), ren)){
        logSDLError(cout, "Unable to load sheet texture", 1);
        return false;
    }
    return true;
}

bool loadMedia() {
    Map4 = {0, 0, 1120, 630};
    if (! LoadBackGround("GameHKI/Map/map4.png", GRenderer)){
        logSDLError(cout, "Unable to load map 4", 1);
        return 0;
    }
    // load MainCharacter
    Explorer.loadCharacter(GRenderer);
    Explorer.setSpeed(MainSpeed);
    // Load Skeleton
    for (int i = 0; i < 30; i++){
        Ske[i].loadCharacter(GRenderer);
        Ske[i].setSpeed(Rand(1, SkeletonSpeed));
    }
    // Load Errow
    for (int i = 0; i < 4; i++){
        Arrow[i].loadWeapon(GRenderer);
        Arrow[i].setSpeed(ArrowSpeed);
    }
    return 1;
}
void Skeleton_Move(){
     for (int i = 0; i < 30; i++){
        int d = Rand(0,1);
        if (d == 0){ // move Y
            if (Ske[i].getX() < Explorer.getX()){
                Ske[i].goRight(Explorer.getX(), SkeletonAnimation);
            }
            else if (Ske[i].getX() > Explorer.getX()){
                Ske[i].goLeft(Explorer.getX(), SkeletonAnimation);
            }
        }
        else {
            if (Ske[i].getY() < Explorer.getY()){
                Ske[i].goDown(Explorer.getY(), SkeletonAnimation);
            }
            else if (Ske[i].getY() > Explorer.getY()){
                Ske[i].goUp(Explorer.getY(), SkeletonAnimation);
            }
        }
    }
}
void Arrow_Move(){
    int cur_x = Explorer.getX();
    int cur_y = Explorer.getY();
    for (int i = 0; i < 4; i++) {
        Arrow[i].setX(cur_x);
        Arrow[i].setY(cur_y);
    }
}
int main(int argc, char* argv[])
{
    if (! initSDL(window)) {
        logSDLError(cout, "Can not initialize \n", 1);
    }
    else
        {
        WindowSurface = SDL_GetWindowSurface(window);
//    // Your drawing code here
//    // use SDL_RenderPresent(renderer) to show it
        if (!loadMedia()){
            logSDLError(cout, "Can not load media \n", 1);
        }
        else {
            SDL_Event e;
            int cnt = 0;
            bool SkeSpawn = false;
            Explorer.setX(SCREEN_WIDTH/2); Explorer.setY(SCREEN_HEIGHT/2);
            bool quit = false;
            while (!quit) {
                if (SkeSpawn) Skeleton_Move();
                while(SDL_PollEvent(&e) != 0){
                    cnt++;
                    cnt % 10;
                    if (e.type == SDL_QUIT)
                        quit = true;
                    else if (e.type == SDL_KEYDOWN){
                        if (e.key.keysym.sym == SDLK_UP){
                            Explorer.goUp(0, MainAnimation);
                        }
                        else if (e.key.keysym.sym == SDLK_DOWN){
                            Explorer.goDown(SCREEN_HEIGHT - CharacterH, MainAnimation);
                        }
                        else if (e.key.keysym.sym == SDLK_LEFT){
                            Explorer.goLeft(0, MainAnimation);
                        }
                        else if (e.key.keysym.sym == SDLK_RIGHT){
                            Explorer.goRight(SCREEN_WIDTH - CharacterW, MainAnimation);
                        }
                        SkeSpawn = true;
                     }
                }
                SDL_SetRenderDrawColor( GRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear(GRenderer);
                //render
                BackGround4.render(0, 0, GRenderer, &Map4);
                Explorer.display(GRenderer);
                for (int i = 0; i < 30; i++)
                    Ske[i].display(GRenderer);
                if (Explorer.getNum() == MainAnimation - 1)
                    Arrow_Move();
                SDL_RenderPresent(GRenderer);

            }
        }
    }
    //waitUntilKeyPressed();
    //quitSDL(window, renderer);
    return 0;
}
