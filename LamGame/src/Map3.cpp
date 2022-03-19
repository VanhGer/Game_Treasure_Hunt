#include "Map3.h"
void LoadObstacle(SDL_Renderer *ren){
    for (int i = 0; i <= 1120; i++)
        for (int j = 0; j <= 4500; j++)
            info[i][j] = 0;
    std::ifstream f("GameHKI/Map3/tdo.txt");
    int n, x, y;
    f >> n;
    for (int i = 1; i <= n; i++){
        f >> x >> y;
        int q = Rand(0, 1);
        nlava++;
        if (q == 0) lava[nlava].loadFromFile("GameHKI/Map3/lava/aa.png", ren);
        else lava[nlava].loadFromFile("GameHKI/Map3/lava/aaa.png", ren);
        CoorOst[nlava] = make_pair(x, y);
        if (q == 0) {
            for (int tx = 0; tx <= 95; tx++)
            for (int ty = 0; ty <= 95; ty++)
                info[x + tx][y + ty] = 1;
        }
        else {
            for (int tx = 0; tx <= 127; tx++)
            for (int ty = 0; ty <= 63; ty++)
                info[x + tx][y + ty] = 2;
        }
    }
    f.close();
    Rlava[0] = {0, 0, 96, 96};
    Rlava[1] = {0, 0, 128, 64};
}
void LoadMap3(SDL_Renderer *ren){
    nlava = 0;
    LoadObstacle(ren);
    BackGround[3].loadFromFile("GameHKI/Map3/Map33.png", ren);
    fire.loadWeapon(ren, "GameHKI/Map3/Enemy/fire.png");
    eball.loadWeapon(ren, "GameHKI/Map3/Enemy/eball.png");
    fire.setAdd(35); eball.setAdd(35);
    fire.setRObj(0, 0, 35, 35); eball.setRObj(0, 0, 35, 35);
    Explorer.loadCharacter(ren);
    Explorer.setSpeed(MainSpeed);
    Explorer.setHp(MainHp);
    Explorer.setX(SCREEN_WIDTH/2);
    Explorer.setY(530);
    RMap3 = {0, 3870, 1120, 630};
}
void CloseMap3(SDL_Renderer *ren){
    BackGround[3].free();
}
void Intro3(SDL_Renderer *ren){

};
void ChangeRMap(int val){
    int ny = max(0, val - 315 + CharacterH);
    ny = min(ny, 3870);
    RMap3.y = ny;

}
bool CheckInside(SDL_Rect R, int x, int y, int h, int w){
    for (int i = x; i  <= x + w - 1; i++)
        for (int j = y; j <= y + h - 1; j++){
            if (i >= R.x && i < R.x + R.w && j >= R.y && j < R.y + R.h)
                return true;
        }
    return false;
}
bool CheckIntersect(int x, int y, int w, int h, int x1, int y1, int w1, int h1){
    int j = y + h - 1;
    if (j < y1 || j >= y1 + h1) return 0;
    for (int i = x; i <= x + w - 1; i++)
        if (i >= x1 && i < x1 + w1) return true;
    return false;
}
void RunMap3(SDL_Renderer *ren){
    LoadMap3(ren);
    Intro3(ren);
    SDL_Event e;
    int cury = 4400, curframe = 0;
    bool Running = true;
    Uint64 fpstime = SDL_GetTicks64(), wtime = SDL_GetTicks64();
    while (Running) {
        if (SDL_GetTicks64() - fpstime >= 33){
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    Running = false;
                }
                else if (e.type == SDL_KEYDOWN) {
                    Explorer.setY(cury);
                    Explorer_Move(Explorer, e, 0, 4500 - CharacterH, 336, 797 - CharacterW);
                    cury = Explorer.getY();
                    ChangeRMap(cury);
                    if (RMap3.y != 0 && RMap3.y != 3870)
                        Explorer.setY(252);
                    else {
                        Explorer.setY(cury - RMap3.y);
                    }
                    //cout << Explorer.getY() << '\n';
                }
            }
            SDL_RenderClear(ren);
            BackGround[3].render(0, 0, ren, &RMap3);
            if (SDL_GetTicks64() - wtime >= 60){
                curframe = (curframe + 1) % 8;
                Rlava[0].x = (Rlava[0].x + Rlava[0].w) % 384;
                Rlava[1].x = (Rlava[1].x + Rlava[1].w) % 512;
                wtime = SDL_GetTicks64();
            }
            for (int i = 1; i <= nlava; i++){
                if (CheckInside(RMap3, CoorOst[i].first, CoorOst[i].second, 96, 96)){
                    int tw, th;
                    if (info[CoorOst[i].first][CoorOst[i].second] == 1) {
                        lava[i].render(CoorOst[i].first - RMap3.x, CoorOst[i].second - RMap3.y, ren, &Rlava[0]);
                        tw = 56; th = 76;
                    }
                    else {
                        lava[i].render(CoorOst[i].first - RMap3.x, CoorOst[i].second - RMap3.y, ren, &Rlava[1]);
                        tw = 88, th = 44;
                    }
                    if (CheckIntersect(Explorer.getX() + 4, Explorer.getY(), 24, CharacterH,
                                       CoorOst[i].first - RMap3.x + 20, CoorOst[i].second - RMap3.y + 20, tw, th))
                    {
                        cout << Explorer.getX() + 4 << " " << Explorer.getY() << " " << 24 << " " << CharacterH << " "
                            << CoorOst[i].first - RMap3.x + 20 << " " <<  CoorOst[i].second - RMap3.y + 20 << " " << tw
                            << " " <<  th << '\n';
                        cout << "a" << '\n';
                    }
                }
            }

            Explorer.display(ren);
            fire.setNum(curframe); eball.setNum(curframe);
            eball.display(ren);
            SDL_RenderPresent(ren);
            if (Running == false)
                SDL_Delay(3000);
            fpstime = SDL_GetTicks64();
        }
    }
}
