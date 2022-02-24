#include "Common_Function.h"
#include "LoadObject.h"
#include "Character.h"
#include "MainCharacter.h"
#include "Enemy.h"
#include "Weapon.h"
mt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());
long long Rand(long long l, long long r){
    return uniform_int_distribution<long long> (l, r) (rng);
}
LoadObject BackGround4, MainExplorer;
MainCharacter Explorer;
Weapon Arrow[4];
Enemy E[3][100];
int check[2000][2000];
int EnemyCount[3];
bool kt[3][100], canShoot[4];
vector <pii> EnemyList;
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
    Explorer.setHp(MainHp);
    // Load Errow
    for (int i = 0; i < 4; i++){
        Arrow[i].loadWeapon(GRenderer);
        Arrow[i].setSpeed(ArrowSpeed);
    }
    return 1;
}
void Enemy_Spawn(int id){
    for (int i = 0; i < type[id].fi; i++){
        int x = EnemyCount[0];
        E[0][x].loadCharacter(GRenderer, "GameHKI/Skeleton");
        E[0][x].setSpeed(SkeletonSpeed);
        E[0][x].setX(Rand(0, 1) * SCREEN_WIDTH);
        E[0][x].setY(Rand(0, 1) * SCREEN_HEIGHT);
        E[0][x].setHp(SkeletonHp);
        kt[0][x] = true;
        EnemyList.push_back(pii(0, x));
        EnemyCount[0]++;
    }
    for (int i = 0; i < type[id].se.fi; i++){
        int x = EnemyCount[1];
        E[1][x].loadCharacter(GRenderer, "GameHKI/Zombie");
        E[1][x].setSpeed(ZombieSpeed);
        E[1][x].setX(Rand(0, 1) * SCREEN_WIDTH);
        E[1][x].setY(Rand(0, 1) * SCREEN_HEIGHT);
        E[1][x].setHp(ZombieHp);
        kt[1][x] = true;
        EnemyList.push_back(pii(1, x));
        EnemyCount[1]++;
    }
    for (int i = 0; i < type[id].se.se; i++){
        int x = EnemyCount[2];
        E[2][x].loadCharacter(GRenderer, "GameHKI/Shrek");
        E[2][x].setSpeed(ShrekSpeed);
        E[2][x].setX(Rand(0, 1) * SCREEN_WIDTH);
        E[2][x].setY(Rand(0, 1) * SCREEN_HEIGHT);
        E[2][x].setHp(ShrekHp);
        kt[2][x] = true;
        EnemyList.push_back(pii(2, x));
        EnemyCount[2]++;
    }
}
void Enemy_Move(){
     for (int i = 0; i < EnemyList.size(); i++){
        pii Q = EnemyList[i];
        if (E[Q.fi][Q.se].getX() == Explorer.getX()){
            if (E[Q.fi][Q.se].getY() < Explorer.getY()){
               E[Q.fi][Q.se].goDown(Explorer.getY(), EnemyFrames);
            }
            else if (E[Q.fi][Q.se].getY() > Explorer.getY()){
                E[Q.fi][Q.se].goUp(Explorer.getY(), EnemyFrames);
            }
        }
        else if (E[Q.fi][Q.se].getY() == Explorer.getY()){
            if (E[Q.fi][Q.se].getX() < Explorer.getX()){
                E[Q.fi][Q.se].goRight(Explorer.getX(), EnemyFrames);
            }
            else if (E[Q.fi][Q.se].getX() > Explorer.getX()){
                E[Q.fi][Q.se].goLeft(Explorer.getX(), EnemyFrames);
            }
        }
        else {
            int d = Rand(0,5);
            if (d <= 1){ // move Y
                if (E[Q.fi][Q.se].getX() < Explorer.getX()){
                    E[Q.fi][Q.se].goRight(Explorer.getX(), EnemyFrames);
                }
                else if (E[Q.fi][Q.se].getX() > Explorer.getX()){
                    E[Q.fi][Q.se].goLeft(Explorer.getX(), EnemyFrames);
                }
            }
            if (2 <= d && d <= 4) {
                if (E[Q.fi][Q.se].getY() < Explorer.getY()){
                   E[Q.fi][Q.se].goDown(Explorer.getY(), EnemyFrames);
                }
                else if (E[Q.fi][Q.se].getY() > Explorer.getY()){
                    E[Q.fi][Q.se].goUp(Explorer.getY(), EnemyFrames);
                }
            }
        }
    }
    sort(EnemyList.begin(), EnemyList.end(), [](pii Q, pii P){
        return getDistance(E[Q.fi][Q.se].getX(), E[Q.fi][Q.se].getY(), Explorer.getX(), Explorer.getY())
                    < getDistance(E[P.fi][P.se].getX(), E[P.fi][P.se].getY(), Explorer.getX(), Explorer.getY());
    });

}
void Arrow_Move(){
    int cur_x = Explorer.getX();
    int cur_y = Explorer.getY();
    int tmpx = 0, tmpy = 0;
    for (int i = 0; i < 4; i++) {
        Arrow[i].setX(cur_x);
        Arrow[i].setY(cur_y);
        if (! EnemyList.size()) return;
        int id = min(i, int(EnemyList.size() - 1));
        pii Q = EnemyList[id];
        tmpx = E[Q.fi][Q.se].getX();
        tmpy = E[Q.fi][Q.se].getY();
        if (tmpx < cur_x){
            Arrow[i].setVx(max(-ArrowSpeed, (tmpx - cur_x) / 5));
        }
        else {
            Arrow[i].setVx(min(ArrowSpeed, (tmpx - cur_x) / 5));
        }
        if (tmpy < cur_y){
            Arrow[i].setVy(max(-ArrowSpeed, (tmpy - cur_y) / 5));
        }
        else {
            Arrow[i].setVy(min(ArrowSpeed, (tmpy - cur_y) / 5));
        }
    }
}
void Arrow_Shot(int frame){
    for (int i = 0; i < 4; i++){
        if (! canShoot[i]) continue;
        Arrow[i].setX(Arrow[i].getX() + Arrow[i].getVx());
        Arrow[i].setY(Arrow[i].getY() + Arrow[i].getVy());
        //cout << Arrow[i].getX() << " " << Arrow[i].getY() << '\n';
        Arrow[i].setNum(frame/ 2 % ArrowFrames);
        Arrow[i].display(GRenderer);
        for (int j = 0; j < EnemyList.size(); j++){
            pii Q = EnemyList[j];
            if (Intersect(Arrow[i].getX(), Arrow[i].getY(), 32, 32,
            E[Q.fi][Q.se].getX(), E[Q.fi][Q.se].getY(), 60, 40)){
                E[Q.fi][Q.se].setHp(E[Q.fi][Q.se].getHp() - ArrowDamn);
                if (E[Q.fi][Q.se].getHp() <= 0){
                    kt[Q.fi][Q.se] = false;
                    swap(EnemyList[j], EnemyList.back());
                    EnemyList.pop_back();
                }
                canShoot[i] = false;
                Arrow[i].setNum(2);
                Arrow[i].display(GRenderer);
                break;
            }
        }
    }
}
bool Check_Explorer(int &lastTimeDamage){
    for (int j = 0; j < EnemyList.size(); j++){
        int add = 0;
        pii Q = EnemyList[j];
        if (Intersect(Explorer.getX(), Explorer.getY(), 60, 40,
        E[Q.fi][Q.se].getX(), E[Q.fi][Q.se].getY(), 60, 40)){
            if (SDL_GetTicks64() > lastTimeDamage + 1000){
                add = 1;
                lastTimeDamage = SDL_GetTicks64();
            }
            if (Q.fi == 0){
                Explorer.setHp(Explorer.getHp() - add*SkeletonDamn);
            }
            if (Q.fi == 1){
                Explorer.setHp(Explorer.getHp() - add*ZombieDamn);
            }
            if (Q.fi == 2){
                Explorer.setHp(Explorer.getHp() - add*SkeletonDamn);
            }
            if (Explorer.getHp() <= 0){
               return false;
            }
        }
    }
    return true;
}
void Explorer_Move(SDL_Event e){
    if (e.key.keysym.sym == SDLK_UP){
        Explorer.goUp(0, MainFrames);
    }
    else if (e.key.keysym.sym == SDLK_DOWN){
        Explorer.goDown(SCREEN_HEIGHT - CharacterH, MainFrames);
    }
    else if (e.key.keysym.sym == SDLK_LEFT){
        Explorer.goLeft(0, MainFrames);
    }
    else if (e.key.keysym.sym == SDLK_RIGHT){
        Explorer.goRight(SCREEN_WIDTH - CharacterW, MainFrames);
    }
}
bool RunMap4(){
    SDL_Event e;
    bool EnemySpawn = false, EnemyMove = false;
    Explorer.setX(SCREEN_WIDTH/2);
    Explorer.setY(SCREEN_HEIGHT/2);
    bool Running = true;
    int tmp = SDL_GetTicks64(), lastTimeDamage = SDL_GetTicks64();
    int PlayingTime = SDL_GetTicks64();
    bool Shooting = false;
    int cnt = 0, tt = 0;
    while (Running) {
        if (EnemySpawn) {
            Enemy_Move();
            EnemyMove = true;
        }
        if (tt <= 9 && SDL_GetTicks64() > PlayingTime + 20000 && EnemySpawn){
            tt++;
            PlayingTime += 20000;
            Enemy_Spawn(tt);
        }
        while(SDL_PollEvent(&e) != 0){
            if (e.type == SDL_QUIT)
                Running  = false;
            else if (e.type == SDL_KEYDOWN){
                Explorer_Move(e);
                if (! EnemySpawn && SDL_GetTicks64() > PlayingTime + 3000){
                    Enemy_Spawn(0);
                    EnemySpawn = true;
                    PlayingTime = SDL_GetTicks64();
                }
             }
        }
        //render
        if (SDL_GetTicks64() - tmp >= 1){
            SDL_SetRenderDrawColor( GRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear(GRenderer);
            BackGround4.render(0, 0, GRenderer, &Map4);
            Explorer.display(GRenderer);
            for (int i = 0; i < EnemyList.size(); i++){
                pii Q = EnemyList[i];
                //cout << i << " " << EnemyList[i].getX() << " " << EnemyList[i].getY() << '\n';
                E[Q.fi][Q.se].display(GRenderer);
            }
            if (! Check_Explorer(lastTimeDamage)) return false;
            if (EnemyMove && Shooting == false){
                Arrow_Move();
                for (int i = 0; i < 4; i++) canShoot[i] = true;
                Shooting = true;
            }
            if (Shooting){
                Arrow_Shot(cnt);
                cnt++; cnt %= 20;
                if (cnt == 19) { Shooting = false;}
            }
            tmp = SDL_GetTicks64();
        }
        SDL_RenderPresent(GRenderer);
        //SDL_Delay(3000);
    }
    return true;
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
            if (RunMap4()){
                cout << "win";
            }
            else cout << "ga vai lon";
            SDL_SetRenderDrawColor( GRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear(GRenderer);
            LoadBackGround("GameHKI/GameOver.png", GRenderer);
            BackGround4.render(0, 0, GRenderer, &Map4);
            SDL_RenderPresent(GRenderer);
            SDL_Delay(1000);
        }
    }
    //waitUntilKeyPressed();
    //quitSDL(window, renderer);
    return 0;
}
