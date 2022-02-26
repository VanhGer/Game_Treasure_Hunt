#include "Common_Function.h"
#include "LoadObject.h"
#include "Character.h"
#include "MainCharacter.h"
#include "Enemy.h"
#include "Weapon.h"
#include "TextObject.h"
mt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());
long long Rand(long long l, long long r){
    return uniform_int_distribution<long long> (l, r) (rng);
}
LoadObject BackGround4, MainExplorer;
MainCharacter Explorer;
TTF_Font *gFont = NULL;
Weapon Arrow[4];
Enemy E[3][100];
int check[2000][2000];
int EnemyCount[3];
bool kt[3][100], canShoot[4];
vector <pii> EnemyList;
int NumOfArrow = 1, idArrow = 1, numEnemyKilled = 0;
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
void loadArrow(){
    if (idArrow == 1)
         for (int i = 0; i < 4; i++){
            Arrow[i].loadWeapon(GRenderer, "Arrow2.png");
            Arrow[i].setSpeed(ArrowSpeed);
        }
    else
        for (int i = 0; i < 4; i++){
            Arrow[i].loadWeapon(GRenderer, "Arrow3.png");
            Arrow[i].setSpeed(ArrowSpeed);
        }
}
void loadFont(){
    gFont = TTF_OpenFont( "font/OpenSans-Regular.ttf", 15);
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
	}
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
    loadArrow();
    loadFont();
    return 1;
}
pii RandPos(){
    pii q;
    int Rnd = Rand(0, 1);
    if (Rnd == 0){
        q.fi = Rand(0, 1) * SCREEN_WIDTH;
        q.se = Rand(0, SCREEN_HEIGHT);
    }
    else {
        q.fi = Rand(0, SCREEN_WIDTH);
        q.se = Rand(0, 1) * SCREEN_HEIGHT;
    }
    return q;
}
void Enemy_Spawn(int id){
    pii tmp;
    for (int i = 0; i < type[id].fi; i++){
        int x = EnemyCount[0];
        E[0][x].loadCharacter(GRenderer, "GameHKI/Skeleton");
        E[0][x].setSpeed(SkeletonSpeed);
        tmp = RandPos();
        E[0][x].setX(tmp.fi);
        E[0][x].setY(tmp.se);
        E[0][x].setHp(SkeletonHp);
        kt[0][x] = true;
        EnemyList.push_back(pii(0, x));
        EnemyCount[0]++;
    }
    for (int i = 0; i < type[id].se.fi; i++){
        int x = EnemyCount[1];
        E[1][x].loadCharacter(GRenderer, "GameHKI/Zombie");
        E[1][x].setSpeed(ZombieSpeed);
        tmp = RandPos();
        E[1][x].setX(tmp.fi);
        E[1][x].setY(tmp.se);
        E[1][x].setHp(ZombieHp);
        kt[1][x] = true;
        EnemyList.push_back(pii(1, x));
        EnemyCount[1]++;
    }
    for (int i = 0; i < type[id].se.se; i++){
        int x = EnemyCount[2];
        E[2][x].loadCharacter(GRenderer, "GameHKI/Shrek");
        E[2][x].setSpeed(ShrekSpeed);
        tmp = RandPos();
        E[2][x].setX(tmp.fi);
        E[2][x].setY(tmp.se);
        E[2][x].setHp(ShrekHp);
        kt[2][x] = true;
        EnemyList.push_back(pii(2, x));
        EnemyCount[2]++;
    }
}
bool Can_Move(int x, int y){
    for (int i = 0; i < EnemyList.size(); i++){
        pii Q = EnemyList[i];
        if (E[Q.fi][Q.se].getX() == x && E[Q.fi][Q.se].getY() == y)
            return 0;
    }
    return 1;
}
void Enemy_Move(){
    int Epl_x = Explorer.getX();
    int Epl_y = Explorer.getY();
    for (int i = 0; i < EnemyList.size(); i++){
        pii Q = EnemyList[i];
        if (SDL_GetTicks64() - E[Q.fi][Q.se].getLastTimeMove() < 500)
            continue;
        int cur_x = E[Q.fi][Q.se].getX();
        int cur_y = E[Q.fi][Q.se].getY();
        int cur_speed = E[Q.fi][Q.se].getSpeed();
        if (cur_x == Epl_x){
            if (cur_y < Epl_y && Can_Move(cur_x, min(cur_y + cur_speed, Epl_y))){
                    E[Q.fi][Q.se].goDown(Epl_y, EnemyFrames); continue;
            }
            if (cur_y > Epl_y && Can_Move(cur_x, max(cur_y - cur_speed, Epl_y))){
                E[Q.fi][Q.se].goUp(Epl_y, EnemyFrames); continue;
            }
        }
        if (cur_y == Epl_y){
            if (cur_x < Epl_x && Can_Move(min(cur_x + cur_speed, Epl_x), cur_y)){
                E[Q.fi][Q.se].goRight(Epl_x, EnemyFrames); continue;
            }
            if (cur_x > Epl_x && Can_Move(max(cur_x - cur_speed, Epl_x), cur_y)){
                E[Q.fi][Q.se].goLeft(Epl_x, EnemyFrames); continue;
            }
        }
        int Rnd = Rand(0, 1);
        if (Rnd){
            if (cur_x < Epl_x && Can_Move(min(cur_x + cur_speed, Epl_x), cur_y)){
                E[Q.fi][Q.se].goRight(Epl_x, EnemyFrames); continue;
            }
            if (cur_x > Epl_x && Can_Move(max(cur_x - cur_speed, Epl_x), cur_y)){
                E[Q.fi][Q.se].goLeft(Epl_x, EnemyFrames); continue;
            }
        }
        else {
            if (cur_y < Epl_y && Can_Move(cur_x, min(cur_y + cur_speed, Epl_y))){
               E[Q.fi][Q.se].goDown(Epl_y, EnemyFrames); continue;
            }
            if (cur_y > Epl_y && Can_Move(cur_x, max(cur_y - cur_speed, Epl_y))){
                E[Q.fi][Q.se].goUp(Epl_y, EnemyFrames); continue;
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
    for (int i = 0; i < NumOfArrow; i++) {
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
    for (int i = 0; i < NumOfArrow; i++){
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
                E[Q.fi][Q.se].setHp(E[Q.fi][Q.se].getHp() - ArrowDamn + ((idArrow == 1) ? 0 : 1));
                if (E[Q.fi][Q.se].getHp() <= 0){
                    kt[Q.fi][Q.se] = false;
                    swap(EnemyList[j], EnemyList.back());
                    EnemyList.pop_back();
                    numEnemyKilled++;
                    if (numEnemyKilled >= NumOfArrow * 20 && idArrow != 2){
                        NumOfArrow++;
                        if (NumOfArrow == 5){
                            NumOfArrow = 4;
                            idArrow = 2;
                            loadArrow();
                        }
                    }
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
            //if (add) E[Q.fi][Q.se].setNumFrames(9);
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
TextObject Gametime;
void show_game_time(){
    Gametime.SetColor(TextObject::WHITE_TEXT);
    //Show game time
    std::string str_time = "Time: ";
    Uint64 time_val = SDL_GetTicks64() / 1000;
    Uint64 cur_time = 300 - time_val;
    if (cur_time <= 0){
        cout << "win";
        return;
    }
    Uint64 cur_min = cur_time / 60;
    Uint64 cur_sec = cur_time % 60;
    std::string str_min = std::to_string(cur_min);
    std::string str_sec = std::to_string(cur_sec);
    if (cur_min != 0){
        str_time += str_min; str_time += "m : ";
    }
     str_time += str_sec; str_time += " s";
    Gametime.setText(str_time);
    Gametime.LoadFromRenderText(gFont, GRenderer);
    Gametime.RenderText(GRenderer, SCREEN_WIDTH - 200, 15);
}
void show_Enemy_Killed(){
    TextObject NumEK;
    NumEK.SetColor(TextObject::WHITE_TEXT);
    std::string str_ek = "Enemies Killed: ";
    std::string str_num = std::to_string(numEnemyKilled);
    str_ek += str_num;
    NumEK.setText(str_ek);
    NumEK.LoadFromRenderText(gFont, GRenderer);
    NumEK.RenderText(GRenderer, SCREEN_WIDTH/2, 15);
}
void show_ExHp(){
    TextObject ExHp;
    ExHp.SetColor(TextObject::RED_TEXT);
    std::string str_hp = "HP : ";
    std::string str_num = std::to_string(Explorer.getHp());
    str_hp += str_num;
    ExHp.setText(str_hp);
    ExHp.LoadFromRenderText(gFont, GRenderer);
    ExHp.RenderText(GRenderer, 200, 15);
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
        if (tt <= 9 && SDL_GetTicks64() > PlayingTime + 30000 && EnemySpawn){
            tt++;
            PlayingTime += 30000;
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
        if (SDL_GetTicks64() - tmp >= 30){
            SDL_SetRenderDrawColor( GRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear(GRenderer);
            BackGround4.render(0, 0, GRenderer, &Map4);
            Explorer.display(GRenderer);
            if (! Check_Explorer(lastTimeDamage)) return false;
            for (int i = 0; i < EnemyList.size(); i++){
                pii Q = EnemyList[i];
                //cout << i << " " << EnemyList[i].getX() << " " << EnemyList[i].getY() << '\n';
                E[Q.fi][Q.se].display(GRenderer);
            }
            //Show game time

            if (EnemyMove && Shooting == false){
                Arrow_Move();
                for (int i = 0; i < NumOfArrow; i++) canShoot[i] = true;
                Shooting = true;
            }
            if (Shooting){
                Arrow_Shot(cnt);
                cnt++; cnt %= 25;
                if (cnt == 24) { Shooting = false;}
            }
            show_game_time();
            show_Enemy_Killed();
            show_ExHp();
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
