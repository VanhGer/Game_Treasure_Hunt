#include "Map4.h"
void loadArrow(SDL_Renderer *ren){
    for (int i = 0; i < 4; i++){
        Arrow[i].loadWeapon(ren, "GameHKI/Map4/Arrow/Arrow" + std::to_string(idArrow) + ".png");
        Arrow[i].setAdd(32); Arrow[i].setRObj(0, 0, 32, 32);
        Arrow[i].setSpeed(ArrowSpeed);
    }
    AttackSpeed = 500 - (idArrow - 1)* 100;
    DamnRange = 25 + (idArrow - 1) * 2;
    if (idArrow >= 4) DamnRange = 25;
}
bool Can_Move(int x, int y, int id){
    for (int i = 1; i <= NumEnemy; i++){
        if (! EnemyList[i].GetSpawn()) continue;
        if (i == id) continue;
        if (EnemyList[i].getX() == x && EnemyList[i].getY() == y)
            return 0;
    }
    return 1;
}
void Enemy_Move(MainCharacter &Explorer){
    int Epl_x = Explorer.getX();
    int Epl_y = Explorer.getY();
    for (int i = 1; i <= NumEnemy; i++){
        if (! EnemyList[i].GetSpawn()) continue;
        int x = EnemyList[i].getX();
        int y = EnemyList[i].getY();
        if (x < Epl_x) EnemyList[i].goRight(Epl_x, EnemyFrames, 2 + (x < Epl_x));
        else if (x > Epl_x) EnemyList[i].goLeft(Epl_x, EnemyFrames, 2 + (x < Epl_x));
        if (y < Epl_y) EnemyList[i].goDown(Epl_y, EnemyFrames, 2 + (x < Epl_x));
        else if (y > Epl_y) EnemyList[i].goUp(Epl_y, EnemyFrames, 2 + (x < Epl_x));
        if (! Can_Move(EnemyList[i].getX(), EnemyList[i].getY(), i)){
            EnemyList[i].setX(x); EnemyList[i].setY(y);
        }
    }
}
void Enemy_Display(SDL_Renderer* ren){
    for (int i = 1; i <= NumEnemy; i++){
        if (! EnemyList[i].GetSpawn()) {
            continue;
        }
        int tx = EnemyList[i].getX(), ty = EnemyList[i].getY();
        EnemyList[i].setX(tx - RMap4.x); EnemyList[i].setY(ty - RMap4.y);
        if (Intersect(RMap4.x, RMap4.y, RMap4.h, RMap4.w, tx, ty, 50, 32)){
            EnemyList[i].display(ren);
            EnemyList[i].setSpawnTime(SDL_GetTicks64());
        }
        else {
            if ((SDL_GetTicks64() - EnemyList[i].getSpawnTime())/1000 >= 20)
                EnemyList[i].setSpawn(false);
        }
        EnemyList[i].setX(tx); EnemyList[i].setY(ty);
     }
}
void Set_Arrow(int curx, int cury){
    cury += 20;
    int tmpx = 0, tmpy = 0;
    memset(kt, -1, sizeof(kt));
    for (int i = 0; i < NumOfArrow; i++) {
        Arrow[i].setX(curx);
        Arrow[i].setY(cury);
        Arrow[i].setNum(0);
        int cur = 2e9;
        for (int j = 1; j <= NumEnemy; j++){
            if (! EnemyList[j].GetSpawn()) continue;
            if (kt[0] == j || kt[1] == j || kt[2] == j || kt[3] == j) continue;
            int d = getDistance(curx + 16, cury + 16, EnemyList[j].getX() +  16, EnemyList[j].getY() + 25);
            if (d < cur) {cur = d; kt[i] = j;}
        }
        //cout << i << " a " << kt[i] << '\n';
        tmpx = EnemyList[kt[i]].getX() + 16; tmpy = EnemyList[kt[i]].getY() + 25;
        if (tmpx < curx + 16){
            Arrow[i].setVx(max(-ArrowSpeed, (tmpx - curx - 16) / 5));
        }
        else {
            Arrow[i].setVx(min(ArrowSpeed, (tmpx - curx - 16) / 5));
        }
        if (tmpy < cury + 16){
            Arrow[i].setVy(max(-ArrowSpeed, (tmpy - cury - 16) / 5));
        }
        else {
            Arrow[i].setVy(min(ArrowSpeed, (tmpy - cury - 16) / 5));
        }
    }
}
void Arrow_Shot(int frame, SDL_Renderer *ren){
    for (int i = 0; i < NumOfArrow; i++){
        if (finish[i]) canShoot[i] = false;
        if (! canShoot[i]) continue;
        if (kt[i] == -1) continue;
        if (Arrow[i].getVx() < 0) Arrow[i].setX(max(Arrow[i].getX() + Arrow[i].getVx(), EnemyList[kt[i]].getX() + 16));
        else Arrow[i].setX(min(Arrow[i].getX() + Arrow[i].getVx(), EnemyList[kt[i]].getX() + 16));
        if (Arrow[i].getVy() < 0) Arrow[i].setY(max(Arrow[i].getY() + Arrow[i].getVy(), EnemyList[kt[i]].getY() + 16));
        else Arrow[i].setY(min(Arrow[i].getY() + Arrow[i].getVy(), EnemyList[kt[i]].getY() + 16));
        Arrow[i].setNum(frame % ArrowFrames);
        if (Intersect(Arrow[i].getX(), Arrow[i].getY(), 32, 32,
            EnemyList[kt[i]].getX() + 3, EnemyList[kt[i]].getY() + 10, 30, 26)){
            EnemyList[kt[i]].setHp(EnemyList[kt[i]].getHp() - ArrowDamn - (idArrow - 1) * 2);
            if (EnemyList[kt[i]].getHp() <= 0){
                numEnemyKilled++;
                EnemyList[kt[i]].setSpawn(false);
            }
            finish[i] = true;
            Arrow[i].setNum(2);
        }
    }
}
void Change_Arrow(SDL_Renderer *ren) {
    if (NumOfArrow != 4){
        if (numEnemyKilled >= 30) NumOfArrow = 4;
        else if (numEnemyKilled >= 15) NumOfArrow = 3;
        else if (numEnemyKilled >= 5) NumOfArrow = 2;
    }
    else {
        if (numEnemyKilled - 30 < 25) return;
        if (numEnemyKilled - 30 >= 170) idArrow = 5;
        else if (numEnemyKilled - 30 >= 110) idArrow = 4;
        else if (numEnemyKilled - 30 >= 60) idArrow = 3;
        else if (numEnemyKilled - 30 >= 25) idArrow = 2;
        loadArrow(ren);
     }
}
void show_game_time(SDL_Renderer *ren, TTF_Font *font, Uint64 RunningTime){
    TextObject Gametime;
    Gametime.SetColor(TextObject::BLACK_TEXT);
    //Show game time
    std::string str_time = "";
    Uint64 time_val = (SDL_GetTicks64() - RunningTime)/ 1000;
    Uint64 cur_time = 360 - time_val;
    if (cur_time <= 0) {
        Gametime.Free();
        return;
    }
    Uint64 cur_min = cur_time / 60;
    Uint64 cur_sec = cur_time % 60;
    std::string str_min = std::to_string(cur_min);
    std::string str_sec = std::to_string(cur_sec);
    if (cur_min != 0){
        str_time += str_min; str_time += "h : ";
    }
     str_time += str_sec; str_time += "p";
    Gametime.setText(str_time);
    Gametime.LoadFromRenderText(font, ren);
    Gametime.RenderText(ren, 50, 95);
    Gametime.Free();
}
void show_Enemy_Killed(SDL_Renderer *ren, TTF_Font *font){
    TextObject NumEK;
    NumEK.SetColor(TextObject::BLACK_TEXT);
    std::string str_ek = ": ";
    std::string str_num = std::to_string(numEnemyKilled);
    str_ek += str_num;
    NumEK.setText(str_ek);
    NumEK.LoadFromRenderText(font, ren);
    NumEK.RenderText(ren, 100, 35);
    NumEK.Free();
}
void show_Damn(SDL_Renderer *ren, TTF_Font *font){
    TextObject Damn;
    Damn.SetColor(TextObject :: BLACK_TEXT);
    std::string str = ": ";
    str += std::to_string(ArrowDamn + (idArrow - 1));
    Damn.setText(str);
    Damn.LoadFromRenderText(font, ren);
    Damn.RenderText(ren, 100, 65);
    Damn.Free();
}
void CheckCollision4(MainCharacter &Explorer, Uint64 &lastTimeDamage){
    for (int i = 1; i <= NumEnemy; i++) {
        if (! EnemyList[i].GetSpawn()) continue;
        if (SDL_GetTicks64() - lastTimeDamage < 500) continue;
        if (Intersect(Explorer.getX() + 5, Explorer.getY() + 38, 18, 30,
                      EnemyList[i].getX() + 5, EnemyList[i].getY() + 40, 10, 22)){
            Explorer.setHp(Explorer.getHp() - damn[EnemyList[i].GetTypes() - 1]);
            lastTimeDamage = SDL_GetTicks64();
        }
    }
}
void Circle_Spawn(SDL_Renderer *ren, int tt, int curx){
     std::ifstream f("GameHKI/Map4/Circle.txt");
     int n, x, y;
     int q = Rand(min(3, 1 + tt/40) , min(4, 2 + tt/20)), j = 1;
     f >> n;
     for (int i = 1; i <= n; i++){
        f >> x >> y;
        x += RMap4.x; y += RMap4.y;
        bool obturate = false;
        while (j <= NumEnemy){
            if (EnemyList[j].GetSpawn() == false){
                obturate = true;
                EnemyList[j].setX(x); EnemyList[j].setY(y);
                EnemyList[j].setId(2 + (x < curx)); EnemyList[j].setSpeed(1 + (q - 1) / 2);
                EnemyList[j].setSpawn(true); EnemyList[j].setSpawnTime(SDL_GetTicks64());
                EnemyList[j].setTypes(q); EnemyList[j].setHp(3 + (q - 1) * 2);
                EnemyList[j].loadCharacter(ren, "GameHKI/Map4/" + to_string(EnemyList[j].GetTypes()));
                break;
            }
            else j++;
        }
        if (obturate) continue;
        NumEnemy++;
        EnemyList[NumEnemy].setX(x); EnemyList[NumEnemy].setY(y);
        EnemyList[NumEnemy].setId(2 + (x < curx));  EnemyList[NumEnemy].setSpeed(1 + (q - 1) / 2);
        EnemyList[NumEnemy].setSpawn(true); EnemyList[NumEnemy].setSpawnTime(SDL_GetTicks64());
        EnemyList[NumEnemy].setTypes(q); EnemyList[NumEnemy].setHp(3 + (q - 1) * 2);
        EnemyList[NumEnemy].loadCharacter(ren, "GameHKI/Map4/" + to_string(EnemyList[NumEnemy].GetTypes()));
     }
     f.close();
}
void Normal_Spawn(SDL_Renderer *ren, int tt, int curx){
    std::ifstream f("GameHKI/Map4/Circle.txt");
    int cnt = Rand(3, 7), q = Rand(min(3, 1 + tt/40), min(5, 1 + tt/10)), j = 1, n, x, y;
    f >> n;
    for (int i = 1; i <= n; i++){
        f >> x >> y;
        x += RMap4.x; y += RMap4.y;
        if (cnt == 0) break;
        if (n - i + 1 > cnt) if (Rand(0, 1)) continue;
        cnt--;
        bool obturate = false;
        while (j <= NumEnemy){
            if (EnemyList[j].GetSpawn() == false){
                obturate = true;
                EnemyList[j].setX(x); EnemyList[j].setY(y);
                EnemyList[j].setId(2 + (x < curx)); EnemyList[j].setSpeed(1 + (q - 1) / 2);
                EnemyList[j].setSpawn(true); EnemyList[j].setSpawnTime(SDL_GetTicks64());
                EnemyList[j].setTypes(q); EnemyList[j].setHp(3 + (q - 1) * 2);
                EnemyList[j].loadCharacter(ren, "GameHKI/Map4/" + to_string(EnemyList[j].GetTypes()));
                break;
            }
            else j++;
        }
        if (obturate) continue;
        NumEnemy++;
        EnemyList[NumEnemy].setX(x); EnemyList[NumEnemy].setY(y);
        EnemyList[NumEnemy].setId(2 + (x < curx)); EnemyList[NumEnemy].setSpeed(1 + (q - 1) / 2);
        EnemyList[NumEnemy].setSpawn(true); EnemyList[NumEnemy].setSpawnTime(SDL_GetTicks64());
        EnemyList[NumEnemy].setTypes(q); EnemyList[NumEnemy].setHp(3 + (q - 1) * 2);
        EnemyList[NumEnemy].loadCharacter(ren, "GameHKI/Map4/" + to_string(EnemyList[NumEnemy].GetTypes()));
     }
     f.close();

}
void ChangeRMap4(int curx, int cury){
    int ny = max(0, cury - 315);
    ny = min(ny, 3370);
    RMap4.y = ny;
    int nx = max(0, curx - 560);
    nx = min(nx, 2880);
    RMap4.x = nx;
}
void LoadMap4(SDL_Renderer *ren){
    BackGround[4].loadFromFile("GameHKI/Map4/finalmap.png", ren);
    RMap4 = {(4000 - SCREEN_WIDTH) / 2, (4000 - SCREEN_HEIGHT) / 2, SCREEN_WIDTH, SCREEN_HEIGHT};
    // load MainCharacter
    Explorer.loadCharacter(ren);
    Explorer.setSpeed(MainSpeed);
    Explorer.setHp(MainHp);
    loadArrow(ren);
    Blood.loadFromFile("GameHKI/BloodBar/blood.png", ren);
    Bar.loadFromFile("GameHKI/BloodBar/empty.png", ren);
    youdied.loadFromFile("GameHKI/Map3/died.png", ren);
    die.loadFromFile("GameHKI/Map3/die.png", ren);
    glow.loadFromFile("GameHKI/Map4/glow.png", ren);
}
void CloseMap4(SDL_Renderer *ren){
    BackGround[4].free();
    Explorer.clean();
    for (int i = 0; i < 4; i++)
        Arrow[i].clean();
    Blood.free();
    Bar.free();
    youdied.free();
    die.free();
    glow.free();
}

bool RunMap4(SDL_Renderer *ren, TTF_Font *font, bool &RunGame){
    LoadMap4(ren);
    ///
    NumOfArrow = 4;
    idArrow = 3;
    numEnemyKilled = 60;
    ///
    Intro(ren, RunGame, 4);
    SDL_Event e;
    Explorer.setX(SCREEN_WIDTH/2);
    Explorer.setY(SCREEN_HEIGHT/2);
    bool Running = true, Shooting = false, dead = false, win = false, key = false;
    Uint64 fpstime = SDL_GetTicks64(), lastTimeDamage = SDL_GetTicks64(), circle = SDL_GetTicks64(), normal = SDL_GetTicks64();
    Uint64 EMoveTime = SDL_GetTicks64(), ArrowSpawnTime = SDL_GetTicks64(), ArrowShootTime = SDL_GetTicks64(), AttackSpeed = 500;
    Uint64 RunningTime = SDL_GetTicks64();
    int cnt = 0, tt = 30, curx = 2000, cury = 2000;
    NumEnemy = 0;
    //Circle_Spawn(ren, tt, curx);
    while (Running) {
        // Explorer Move.
        while(SDL_PollEvent(&e) != 0){
            if (e.type == SDL_QUIT){
                RunGame = false;
                CloseMap4(ren);
                return 0;
            }
            else if (e.type == SDL_KEYDOWN){
                Explorer.setX(curx); Explorer.setY(cury);
                Explorer_Move(Explorer, e, 0, 4000 - CharacterH, 0, 4000 - CharacterW);
                if (!key) ChangeRMap4(curx, cury);
                cury = Explorer.getY();
                curx = Explorer.getX();
             }
        }

        //Enemy_Spawn
        if ((SDL_GetTicks64() - circle)/1000 >= max(10, 30 - tt/10)){
            Circle_Spawn(ren, tt, curx);
            circle = SDL_GetTicks64();
            tt++;
        }
        else if ((SDL_GetTicks64() - normal)/1000 >= max(1, 5 - tt/20)){
            Normal_Spawn(ren, tt, curx);
            normal = SDL_GetTicks64();
            tt++;
        }

        //Enemy_Move
        if (SDL_GetTicks64() - EMoveTime >= 80) {
            Explorer.setX(curx); Explorer.setY(cury);
            Enemy_Move(Explorer);
            EMoveTime = SDL_GetTicks64();
        }
        // Arrow Move:
        if (SDL_GetTicks64() - ArrowSpawnTime >= AttackSpeed) {
            if (Shooting == false) {
                Change_Arrow(ren);
                Set_Arrow(curx, cury);
                Shooting = true;
                for (int i = 0; i < NumOfArrow; i++) {canShoot[i] = true; finish[i] = false;}
                ArrowShootTime = SDL_GetTicks64();
            }
            else if (SDL_GetTicks64() - ArrowShootTime >= 20){
                Arrow_Shot(cnt, ren);
                cnt = (cnt + 1) % DamnRange;
                if (cnt == DamnRange - 1) {Shooting = false; ArrowSpawnTime = SDL_GetTicks64();}
                ArrowShootTime = SDL_GetTicks64();
            }
        }
        //
        // checkCollision
        if (!key) CheckCollision4(Explorer, lastTimeDamage);
        if (Explorer.getHp() <= 0) {
            dead = true;
        }
        //render
        if (SDL_GetTicks64() - fpstime >= 30){
            SDL_RenderClear(ren);
            BackGround[4].render(0, 0, ren, &RMap4);
            if (RMap4.y != 0 && RMap4.y != 3370 && !key)
                Explorer.setY(SCREEN_HEIGHT/2);
            else  Explorer.setY(cury - RMap4.y);
            if (RMap4.x != 0 && RMap4.x != 2880 && !key)
                Explorer.setX(SCREEN_WIDTH/2);
            else  Explorer.setX(curx - RMap4.x);
            if (key) glow.render(535, 60, ren, NULL);
            if (! dead) Explorer.display(ren);
            else die.render(Explorer.getX(), Explorer.getY(), ren, NULL);
            show_BloodBar(Explorer.getX() - 5,Explorer.getY() + 60, Explorer.getHp(), ren, Bar, Blood);
            // show Enemy and Arrow
            if (! key){
                Enemy_Display(ren);
                if (Shooting){
                    for (int i = 0; i < NumOfArrow; i++){
                        if (! canShoot[i] || kt[i] == -1) continue;
                        int x = Arrow[i].getX(), y = Arrow[i].getY();
                        Arrow[i].setX(x - RMap4.x);
                        Arrow[i].setY(y - RMap4.y);
                        Arrow[i].display(ren);
                        Arrow[i].setX(x); Arrow[i].setY(y);
                    }
                }
            }
            // show frame
            show_frame(ren, font, 4);
            if (!key) show_game_time(ren, font, RunningTime);
            show_Enemy_Killed(ren, font);
            show_ExHp(Explorer, ren, font, 100, 3);
            show_Damn(ren, font);
            if (key && CheckIntersect(Explorer.getX() + 4, Explorer.getY(), 24, CharacterH, 545, 70, 44, 44)){
                win = true;
                Running = false;
            }
            if ((SDL_GetTicks64() - RunningTime >= 360000) && !dead && !key) key = true;
            if (dead)
                youdied.render(450, 200, ren, NULL);
            SDL_RenderPresent(ren);
            if (dead) Running = false;
            if (! Running) SDL_Delay(2000);
            fpstime = SDL_GetTicks64();
        }
        SDL_RenderPresent(ren);
    }
    if (win) {
        Running = true;
        cnt = 0;
        fpstime = SDL_GetTicks64();
        arr.loadFromFile("GameHKI/Map4/key/key.png", ren);
    }
    SDL_Rect Rcur;
    LoadObject congra;
    congra.loadFromFile("GameHKI/Map4/paper.png", ren);
    Rcur.x = 0; Rcur.y = 0; Rcur.h = 390; Rcur.w = 200;
    while (Running){
        SDL_RenderClear(ren);
        if (SDL_GetTicks64() - fpstime >= 3){
            BackGround[4].render(0, 0, ren, &RMap4);
            cnt = (cnt + 1) % 8;
            Rcur.x = cnt * 200;
            arr.render(100, 100, ren, &Rcur);
            congra.render(550, 40, ren, NULL);
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    RunGame = false;
                    congra.free();
                    CloseMap4(ren);
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
    congra.free();
    return win;
}
