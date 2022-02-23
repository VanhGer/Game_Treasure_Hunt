#include<Common_Function.h>
#include<Character.h>
using namespace std;

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
    int getHeight(){return MainH;}
    int getWidth(){return MainW;}
};

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
        SDLCommonFunc::logSDLError(cout, "Unable to load image", 1);
    }
    else {
        // set colorKey
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL)
            SDLCommonFunc::logSDLError(cout, "Unable to load Texture", 1);
        else{
            MainH = loadedSurface->h;
            MainW = loadedSurface->w;
        }
        SDL_FreeSurface(loadedSurface);
    }
    MainTexture = newTexture;
    return MainTexture != NULL;
}
Ltexture BackGround4, MainExplorer;
bool create_Renderer() {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDLCommonFunc::logSDLError(std::cout, "Renderer coud not be created", 1);
        return false;
    }
    return true;
}
bool create_PNG(){
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)){
        SDLCommonFunc::logSDLError(std::cout, "SDL_image could not be initialized", true);
        return false;
    }
    return true;
}
bool initSDL(SDL_Window* &window) {
    bool success = true;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        SDLCommonFunc::logSDLError(std::cout, "SDL_Init", true);
        return false;
    }
    else {
        window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
           SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            SDLCommonFunc::logSDLError(std::cout, "CreateWindow", true);
            return false;
        }
        else {
            success = create_Renderer();
            if (! success) return false;
            success = create_PNG();
            if (! success) return false;

        }
    }
    return true;
}
void close(){
    BackGround4.free();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
	renderer = NULL;
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
SDL_Rect Map4, gExplorer;
class MainCharacter : public Character {
    private:
        Ltexture  Dir[4];
    public:
        MainCharacter(){
            Character();
        }
        ~MainCharacter(){
            clean();
        }
        void clean(){
            for (int i = 0; i < 4; i++){
                Dir[i].free();
                Rdir[i].h = 0; Rdir[i].w = 0;
                Rdir[i].x = 0; Rdir[i].y = 0;
                NumAnimation[i] = 0;
            }
        }
        void setSpeed(int val){
            Speed = val;
        }
        int GetSpeed(){
            return Speed;
        }
        void loadCharacter() {
            if (!Dir[0].loadFromFile("GameHKI/Explorer/up.png"))
                SDLCommonFunc::logSDLError(cout, "Unable to load Character", 1);
            if (!Dir[1].loadFromFile("GameHKI/Explorer/down.png"))
                SDLCommonFunc::logSDLError(cout, "Unable to load Character", 1);
            if (!Dir[2].loadFromFile("GameHKI/Explorer/left.png"))
                SDLCommonFunc::logSDLError(cout, "Unable to load Character", 1);
            if (!Dir[3].loadFromFile("GameHKI/Explorer/right.png"))
                SDLCommonFunc::logSDLError(cout, "Unable to load Character", 1);
        }
        void changeNumAnimation(int id){
            NumAnimation[id]++;
            NumAnimation[id] %= 6;
        }
        void display(int x, int y, int id){
            Rdir[id].x = 64*NumAnimation[id];
            Dir[id].render(x, y, &Rdir[id]);
        }
} Explorer;
void EgoUp(int &x, int &y){
    y = max(0, y - Explorer.GetSpeed());
    if (y < 50) {
        Map4.y = max(0,Map4.y - 50);
        if (Map4.y != 0) y = min(570, y + 50);
    }
}
void EgoDown(int &x, int &y){
    y = min(570, y +  Explorer.GetSpeed());
    if (570 - y < 50)  {
        Map4.y = min(1370,Map4.y + 50);
        if (Map4.y != 1370) y = max(0, y - 50);
    }
}
void EgoLeft(int &x, int &y){
    x = max(0, x -  Explorer.GetSpeed());
    if (x < 50) {
        Map4.x = max(0,Map4.x - 50);
        if (Map4.x != 0) x = min(1080, x + 50);
    }
}
void EgoRight(int &x, int &y){
    x = min(1080, x +  Explorer.GetSpeed());
    if (1080 - x < 50){
        Map4.x = min(880, Map4.x + 50);
        if (Map4.x != 880) x = max(0, x - 50);
    }
}

bool loadMedia() {
    if (! BackGround4.loadFromFile("GameHKI/lastmap3.png")){
        SDLCommonFunc::logSDLError(cout, "Unable to load sheet texture", 1);
        return false;
    }
    else {
       Map4.h = 630;
       Map4.w = 1120;
       Map4.x = 685;
       Map4.y = 440;
    }
    Explorer.loadCharacter();
    Explorer.setSpeed(5);
    return 1;
}
int main(int argc, char* argv[])
{
    if (! initSDL(window)) {
        SDLCommonFunc::logSDLError(cout, "Can not initialize \n", 1);
    }
    else
        {
        WindowSurface = SDL_GetWindowSurface(window);
//    // Your drawing code here
//    // use SDL_RenderPresent(renderer) to show it
        if (!loadMedia()){
            SDLCommonFunc::logSDLError(cout, "Can not load media \n", 1);
        }
        else {
            SDL_Event e;
            int tx, ty, id;
            tx = 0; ty = 0; id = 3;

            bool quit = false;
            while (!quit) {
                while(SDL_PollEvent(&e) != 0){
                    if (e.type == SDL_QUIT)
                        quit = true;
                    else if (e.type == SDL_KEYDOWN){
                        if (e.key.keysym.sym == SDLK_UP){
                            id = 0;
                            Explorer.changeNumAnimation(id);
                            EgoUp(tx, ty);
                        }
                        else if (e.key.keysym.sym == SDLK_DOWN){
                            id = 1;
                            Explorer.changeNumAnimation(id);
                            EgoDown(tx, ty);
                        }
                        else if (e.key.keysym.sym == SDLK_LEFT){
                            id = 2;
                            Explorer.changeNumAnimation(id);
                            EgoLeft(tx, ty);
                        }
                        else if (e.key.keysym.sym == SDLK_RIGHT){
                            id = 3;
                            Explorer.changeNumAnimation(id);
                            EgoRight(tx, ty);
                        }
                     }
                }
                SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear(renderer);
                //render
                BackGround4.render(0, 0, &Map4);
                Explorer.display(tx, ty, id);
                SDL_RenderPresent(renderer);

            }
        }
    }
    //waitUntilKeyPressed();
    //quitSDL(window, renderer);
    return 0;
}
