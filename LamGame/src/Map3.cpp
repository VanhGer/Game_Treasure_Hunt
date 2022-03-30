#include "Map3.h"
void LoadObstacle(SDL_Renderer *ren){
    for (int i = 0; i <= 1120; i++)
        for (int j = 0; j <= 6000; j++)
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
    f >> n;
    nsnake = n * 2;
    for (int i = 1; i <= n * 2; i+= 2){
        f >> y;
        snake[i].loadWeapon(ren, "GameHKI/Map3/Snake/right.png"); dir_snake[i] = 1;
        snake[i + 1].loadWeapon(ren, "GameHKI/Map3/Snake/left.png"); dir_snake[i + 1] = 2;
        snake[i].setAdd(24); snake[i].setRObj(0, 0, 24, 21);
        snake[i + 1].setAdd(24); snake[i + 1].setRObj(0, 0, 24, 21);
        snake[i].setX(Rand(350, 750)); snake[i].setY(y); snake[i].setVx(i/10 + 1);
        snake[i + 1].setX(Rand(350, 750)); snake[i + 1].setY(y); snake[i + 1].setVx(-(i/10 + 1));
        snake[i].setDamn(1); snake[i + 1].setDamn(1);
    }
    f.close();
    Rlava[0] = {0, 0, 96, 96};
    Rlava[1] = {0, 0, 128, 64};
}
void LoadMap3(SDL_Renderer *ren){
    nlava = 0;
    LoadObstacle(ren);
    glow.loadFromFile("GameHKI/Map3/glow.png", ren);
    arr.loadFromFile("GameHKI/Map3/arr.png", ren);
    youdied.loadFromFile("GameHKI/Map3/died.png", ren);
    die.loadFromFile("GameHKI/Map3/die.png", ren);
    BackGround[3].loadFromFile("GameHKI/Map3/Map33.png", ren);
    for (int i = 0; i < 10; i++){
        fire[i].loadWeapon(ren, "GameHKI/Map3/Enemy/fire2.png");
        eball[i].loadWeapon(ren, "GameHKI/Map3/Enemy/eball.png");
        fire[i].setAdd(35); eball[i].setAdd(35);
        fire[i].setRObj(0, 0, 35, 35); eball[i].setRObj(0, 0, 35, 35);
    }
    Explorer.loadCharacter(ren);
    Explorer.setSpeed(5);
    Explorer.setHp(25000);
    Explorer.setX(SCREEN_WIDTH/2);
    Explorer.setY(530);
    RMap3 = {0, 5370, 1120, 630};
    Rarr = {0, 0, 500, 500};
}
void CloseMap3(SDL_Renderer *ren){
    BackGround[3].free();
    youdied.free(); die.free();
    for (int i = 0; i < 10; i++) {
        fire[i].clean(); eball[i].clean();
    }
    for (int i = 1; i <= nsnake; i++) snake[i].clean();
    for (int i = 1; i <= nlava; i++) lava[i].free();
    Explorer.clean();
}
void Intro3(SDL_Renderer *ren, bool &RunGame){
    LoadObject intro1;
    intro1.loadFromFile("GameHKI/Intro/intro3.png", ren);
    SDL_Event e; bool Running = true;
    while (Running) {
        while(SDL_PollEvent( &e ) != 0 ){
            //User requests quit
            if( e.type == SDL_QUIT ){
                Running = false;
                RunGame = false;
            }
            //Handle button events
            if (e.type == SDL_MOUSEBUTTONDOWN)
                Running = false;
        }
        BackGround[3].render(0, 0, ren, &RMap3);
        intro1.render(0, 0, ren, NULL);
        SDL_RenderPresent(ren);
    }
    intro1.free();
}
void ChangeRMap3(int val){
    int ny = max(0, val - 315 + CharacterH);
    ny = min(ny, 5370);
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
void Spawn_Ost(SDL_Renderer *ren, int n){
    int q1 = Rand(2 + n / 6, 3 + n / 6);
    int q2 = Rand(1 + n / 6, 2 + n / 6);
    for (int i = 0; i < 10; i++){
        if (Fappear[i]) continue;
        if (q1 == 0) break;
        q1--;
        Fappear[i] = true;
        Ftime[i] = SDL_GetTicks64();
        int r = Rand(0, 1);
        if (r == 1) {
            fire[i].setX(Rand(336, 768));
            fire[i].setY(RMap3.y);
            fire[i].setVx(0);
        }
        else {
            fire[i].setX(336 + Rand(0, 1) * 432);
            fire[i].setY(Rand(RMap3.y, RMap3.y + 150));
            if (fire[i].getX() < Explorer.getX()) fire[i].setVx(2 + n / 10);
            else fire[i].setVx(-2 - n/10);
        }
        fire[i].setVy(2 + n / 10); fire[i].setDamn(2);
    }
    for (int i = 0; i < 10; i++){
        if (Eappear[i]) continue;
        if (q2 == 0) break;
        q2--;
        Eappear[i] = true;
        Etime[i] = SDL_GetTicks64();
        int r = Rand(0, 1);
        if (r == 1) {
            eball[i].setX(Rand(336, 768));
            eball[i].setY(RMap3.y);
            eball[i].setVx(0);
        }
        else {
            eball[i].setX(336 + Rand(0, 1) * 432);
            eball[i].setY(Rand(RMap3.y, RMap3.y + 150));
            if (eball[i].getX() < Explorer.getX()) eball[i].setVx(2 + n / 10);
            else eball[i].setVx(-2 - n/10);
        }
        eball[i].setVy(2 + n / 10); eball[i].setDamn(3);
    }
}
void Ost_move(SDL_Renderer *ren){
    for (int i = 0; i < 10; i++){
        if (! Fappear[i]) continue;
        fire[i].setX(fire[i].getX() + fire[i].getVx());
        fire[i].setY(fire[i].getY() + fire[i].getVy());
        if (fire[i].getX() <= 336 || fire[i].getX() >= 768)
            fire[i].setVx(-fire[i].getVx());
    }
    for (int i = 0; i < 10; i++){
        if (! Eappear[i]) continue;
        eball[i].setX(eball[i].getX() + eball[i].getVx());
        eball[i].setY(eball[i].getY() + eball[i].getVy());
        if (eball[i].getX() <= 336 || eball[i].getX() >= 768)
            eball[i].setVx(-eball[i].getVx());
    }
    for (int i = 1; i <= nsnake; i++){
        snake[i].setX(snake[i].getX() + snake[i].getVx());
        if (snake[i].getX() <= 336 || snake[i].getX() >= 774){
            snake[i].setVx(-snake[i].getVx());
            dir_snake[i] = 3 - dir_snake[i];
            if (dir_snake[i] == 1) snake[i].loadWeapon(ren, "GameHKI/Map3/Snake/right.png");
            else snake[i].loadWeapon(ren, "GameHKI/Map3/Snake/left.png");
        }
    }
}
void CheckCollision3(MainCharacter &Explorer, Uint64 &t){
    for (int i = 1; i <= nsnake; i++){
        if (Intersect(snake[i].getX(), snake[i].getY(), 21, 24, Explorer.getX() + 5, Explorer.getY() + 15, 26, 30)){
            if (SDL_GetTicks64() - t >= 1000) {
                Explorer.setHp(Explorer.getHp() - snake[i].getDamn());
                t = SDL_GetTicks64();
            }
        }
    }
    for (int i = 0; i < 10; i++){
        if (! Fappear[i]) continue;
        if (Intersect(fire[i].getX(), fire[i].getY(), 35, 35,  Explorer.getX() + 5, Explorer.getY() + 15, 26, 30)){
            Explorer.setHp(Explorer.getHp() - fire[i].getDamn());
            Fappear[i] = false;
        }
    }
    for (int i = 0; i < 10; i++){
        if (! Eappear[i]) continue;
        if (Intersect(eball[i].getX(), eball[i].getY(), 35, 35,  Explorer.getX() + 5, Explorer.getY() + 15, 26, 30)){
            Explorer.setHp(Explorer.getHp() - eball[i].getDamn());
            Eappear[i] = false;
        }
    }
}
void show_ExHp(MainCharacter &Explorer, SDL_Renderer *ren, TTF_Font *font){
    TextObject ExHp;
    ExHp.SetColor(TextObject::RED_TEXT);
    std::string str_hp = "HP : ";
    std::string str_num = std::to_string(Explorer.getHp());
    str_hp += str_num;
    ExHp.setText(str_hp);
    ExHp.LoadFromRenderText(font, ren);
    ExHp.RenderText(ren, 200, 15);
}
void Explorer_Move(MainCharacter &Explorer, SDL_Event e, int minY, int maxY, int minX, int maxX){
    //cout << minY << " " << maxY << " " << minX << " " << maxX << '\n';
    //cout << Explorer.getX() << " b " << Explorer.getY() << '\n';
    if (e.key.keysym.sym == SDLK_UP){
        Explorer.goUp(minY, MainFrames, 0);
    }
    else if (e.key.keysym.sym == SDLK_DOWN){
        Explorer.goDown(maxY, MainFrames, 1);
    }
    else if (e.key.keysym.sym == SDLK_LEFT){
        Explorer.goLeft(minX, MainFrames, 2);
    }
    else if (e.key.keysym.sym == SDLK_RIGHT){
        Explorer.goRight( maxX, MainFrames, 3);
    }
    //cout << Explorer.getX() << " a " << Explorer.getY() << '\n';
}
bool RunMap3(SDL_Renderer *ren, TTF_Font *font, bool &RunGame){
    LoadMap3(ren);
    Intro3(ren, RunGame);
    if (!RunGame) {CloseMap3(ren); return 0;}
    SDL_Event e;
    int cury = 5900, curframe = 0, sframe = 0, ntimes = 0, q1, q2;
    bool Running = true, dead = false, Ost_start = false, loadOst = true, win = false;
    Uint64 fpstime = SDL_GetTicks64(), lavatime = SDL_GetTicks64(), sptime = SDL_GetTicks64(),
           snaketime = SDL_GetTicks64(), damntime = SDL_GetTicks64();
    while (Running) {
        if (SDL_GetTicks64() - fpstime >= 33){
            if (loadOst) {
                Ost_move(ren);
                if (SDL_GetTicks64() - sptime >= max(500, 2000 - 70 * ntimes) && !Ost_start){
                    Spawn_Ost(ren, ++ntimes);
                    Ost_start = true;
                }
                if (SDL_GetTicks64() - sptime >= max(2000, 5000 - 100 * ntimes)){
                    Ost_start = false;
                    sptime = SDL_GetTicks64();
                }
            }
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    RunGame = false;
                    CloseMap3(ren);
                    return 0;
                }
                else if (e.type == SDL_KEYDOWN) {
                    Explorer.setY(cury);
                    Explorer_Move(Explorer, e, 0, 6000 - CharacterH, 336, 797 - CharacterW);
                    cury = Explorer.getY();
                    ChangeRMap3(cury);
                }
            }
            if (cury <= 300) loadOst = false;
            if (CheckIntersect(Explorer.getX() + 4, cury, 24, CharacterH, 545, 70, 44, 44)){
                win = true;
                Running = false;
            }
            Explorer.setY(cury);
            CheckCollision3(Explorer, damntime);
            if (RMap3.y != 0 && RMap3.y != 5370)
                Explorer.setY(252);
            else {
                Explorer.setY(cury - RMap3.y);
            }
            if (Explorer.getHp() <= 0) {
                dead = true;
            }
            SDL_RenderClear(ren);
            BackGround[3].render(0, 0, ren, &RMap3);
            if (SDL_GetTicks64() - lavatime >= 70){
                if (dead) Running = false;
                curframe = (curframe + 1) % 8;
                Rlava[0].x = (Rlava[0].x + Rlava[0].w) % 384;
                Rlava[1].x = (Rlava[1].x + Rlava[1].w) % 512;
                lavatime = SDL_GetTicks64();
            }
            if (SDL_GetTicks64() - snaketime >= 200){
                sframe = (sframe + 1) % 3;
                snaketime = SDL_GetTicks64();
            }
            if (CheckInside(RMap3, 535, 60, 64, 64)){
                glow.render(535, 60 - RMap3.y, ren, NULL);
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
                        CoorOst[i].first - RMap3.x + 20, CoorOst[i].second - RMap3.y + 20, tw, th)){
                        Explorer.setHp(0);
                        dead = true;
                    }
                }
            }

            int cur = 0;
            for (int i = 1; i <= nsnake; i++){
                if (! CheckInside(RMap3, snake[i].getX(), snake[i].getY(), 21, 24)) continue;
                snake[i].setNum(sframe); cur = snake[i].getY();
                snake[i].setY(cur - RMap3.y);
                snake[i].display(ren); snake[i].setY(cur);
            }
            if (loadOst){
                for (int i = 0; i < 10; i++) {
                    if (! Eappear[i]) continue;
                    if (SDL_GetTicks64() - Etime[i] > 10000) {Eappear[i] = false; continue;}
                    eball[i].setNum(curframe); cur = eball[i].getY();
                    eball[i].setY(cur - RMap3.y);
                    eball[i].display(ren); eball[i].setY(cur);
                }
                for (int i = 0; i < 10; i++) {
                    if (! Fappear[i]) continue;
                    if (SDL_GetTicks64() - Ftime[i] > 10000) {Fappear[i] = false; continue;}
                    fire[i].setNum(curframe); cur = fire[i].getY();
                    fire[i].setY(cur - RMap3.y);
                    fire[i].display(ren); fire[i].setY(cur);
                }
            }
            if (! dead) Explorer.display(ren);
            else {
                die.render(Explorer.getX(), Explorer.getY(), ren, NULL);
            }
            show_ExHp(Explorer, ren, font);
            if (dead){
                youdied.render(450, 200, ren, NULL);
            }
            SDL_RenderPresent(ren);
            if (! Running) SDL_Delay(2000);
             fpstime = SDL_GetTicks64();
        }
    }
    if (win) Running = true;
    curframe = 0;
    fpstime = SDL_GetTicks64(), sptime = SDL_GetTicks64();
    LoadObject congra;
    congra.loadFromFile("GameHKI/Map3/paper3.png", ren);
    while (Running){
        SDL_RenderClear(ren);
        if (SDL_GetTicks64() - fpstime >= 2){
            BackGround[3].render(0, 0, ren, &RMap3);
           // if (SDL_GetTicks64() - sptime >= 70){
                curframe = 1 - curframe;
                Rarr.x = curframe * 500;
                sptime = SDL_GetTicks64();
          //  }
            arr.render(50, 40, ren, &Rarr);
            congra.render(550, 40, ren, NULL);
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    RunGame = false;
                    congra.free();
                    CloseMap3(ren);
                    return 0;
                }
                else if (e.type == SDL_MOUSEBUTTONDOWN) {
                    Running = false;
                }
            }
            SDL_RenderPresent(ren);
            fpstime = SDL_GetTicks64();
        }
    }

    CloseMap3(ren);
    return win;
}
