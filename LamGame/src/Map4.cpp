#include "Map4.h"
void loadArrow(SDL_Renderer *ren){
    for (int i = 0; i < 4; i++){
        Arrow[i].loadWeapon(ren, "GameHKI/Map4/Arrow/Arrow" + std::to_string(idArrow) + ".png");
        Arrow[i].setAdd(32); Arrow[i].setRObj(0, 0, 32, 32);
        Arrow[i].setSpeed(ArrowSpeed);
    }
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
    int kt[4], cur = 2e9;
    memset(kt, -1, sizeof(kt));
    for (int i = 0; i < NumOfArrow; i++) {
        Arrow[i].setX(curx);
        Arrow[i].setY(cury);
        Arrow[i].setNum(0);
        for (int j = 1; j <= NumEnemy; j++){
            if (! EnemyList[j].GetSpawn()) continue;
            if (kt[0] == j || kt[1] == j || kt[2] == j || kt[3] == j) continue;
            int d = getDistance(curx, cury, EnemyList[j].getX(), EnemyList[j].getY());
            if (d < cur) {cur = d; kt[i] = j;}
        }
        tmpx = EnemyList[kt[i]].getX(); tmpy = EnemyList[kt[i]].getY() + 10;
        if (tmpx < curx){
            Arrow[i].setVx(max(-ArrowSpeed, (tmpx - curx) / 5));
        }
        else {
            Arrow[i].setVx(min(ArrowSpeed, (tmpx - curx) / 5));
        }
        if (tmpy < cury){
            Arrow[i].setVy(max(-ArrowSpeed, (tmpy - cury) / 5));
        }
        else {
            Arrow[i].setVy(min(ArrowSpeed, (tmpy - cury) / 5));
        }
    }
}
void Arrow_Shot(int frame, SDL_Renderer *ren){
    for (int i = 0; i < NumOfArrow; i++){
        if (finish[i]) canShoot[i] = false;
        if (! canShoot[i]) continue;
        Arrow[i].setX(Arrow[i].getX() + Arrow[i].getVx());
        Arrow[i].setY(Arrow[i].getY() + Arrow[i].getVy());
        cout << Arrow[i].getX() << " " << Arrow[i].getY() << '\n';
        Arrow[i].setNum(frame/ 2 % ArrowFrames);
        for (int j = 1; j <= NumEnemy; j++){
            if (Intersect(Arrow[i].getX(), Arrow[i].getY(), 32, 32,
                EnemyList[j].getX() + 3, EnemyList[j].getY() + 10, 30, 26)){
                EnemyList[j].setHp(EnemyList[j].getHp() - ArrowDamn - idArrow);
                if (EnemyList[j].getHp() <= 0){
                    numEnemyKilled++;
//                    if (numEnemyKilled >= NumOfArrow * 20 && idArrow != 2){
//                        NumOfArrow++;
//                        if (NumOfArrow == 5){
//                            NumOfArrow = 4;
//                            idArrow = 2;
//                            loadArrow(ren);
//                        }
//                    }
                }
                finish[i] = true;
                Arrow[i].setNum(2);
                break;
            }
        }
    }
}
//bool Check_Explorer(int &lastTimeDamage){
//    for (int j = 0; j < EnemyList.size(); j++){
//        int add = 0;
//        pii Q = EnemyList[j];
//        if (Intersect(Explorer.getX(), Explorer.getY(), 60, 40,
//        EnemyList[j].getX(), EnemyList[j].getY(), 60, 40)){
//            if (SDL_GetTicks64() > lastTimeDamage + 1000){
//                add = 1;
//                lastTimeDamage = SDL_GetTicks64();
//            }
//            //if (add) EnemyList[j].setNumFrames(9);
//            if (Q.fi == 0){
//                Explorer.setHp(Explorer.getHp() - add*SkeletonDamn);
//            }
//            if (Q.fi == 1){
//                Explorer.setHp(Explorer.getHp() - add*ZombieDamn);
//            }
//            if (Q.fi == 2){
//                Explorer.setHp(Explorer.getHp() - add*SkeletonDamn);
//            }
//            if (Explorer.getHp() <= 0){
//               return false;
//            }
//        }
//    }
//    return true;
//}
//void show_game_time(SDL_Renderer *ren, TTF_Font *font){
//    Gametime.SetColor(TextObject::WHITE_TEXT);
//    //Show game time
//    std::string str_time = "Time: ";
//    Uint64 time_val = SDL_GetTicks64() / 1000;
//    Uint64 cur_time = 300 - time_val;
//    if (cur_time <= 0){
//        cout << "win";
//        return;
//    }
//    Uint64 cur_min = cur_time / 60;
//    Uint64 cur_sec = cur_time % 60;
//    std::string str_min = std::to_string(cur_min);
//    std::string str_sec = std::to_string(cur_sec);
//    if (cur_min != 0){
//        str_time += str_min; str_time += "m : ";
//    }
//     str_time += str_sec; str_time += " s";
//    Gametime.setText(str_time);
//    Gametime.LoadFromRenderText(font, ren);
//    Gametime.RenderText(ren, SCREEN_WIDTH - 200, 15);
//}
//void show_Enemy_Killed(SDL_Renderer *ren, TTF_Font *font){;p
//    TextObject NumEK;
//    NumEK.SetColor(TextObject::WHITE_TEXT);
//    std::string str_ek = "Enemies Killed: ";
//    std::string str_num = std::to_string(numEnemyKilled);
//    str_ek += str_num;
//    NumEK.setText(str_ek);
//    NumEK.LoadFromRenderText(font, ren);
//    NumEK.RenderText(ren, SCREEN_WIDTH/2, 15);
//}
void Circle_Spawn(SDL_Renderer *ren, int tt, int curx){
     std::ifstream f("GameHKI/Map4/Circle.txt");
     int n, x, y;
     int q = Rand(1, 2), j = 1;
     f >> n;
     for (int i = 1; i <= n; i++){
        f >> x >> y;
        x += RMap4.x; y += RMap4.y;
        bool obturate = false;
        while (j <= NumEnemy){
            if (EnemyList[j].GetSpawn() == false){
                obturate = true;
                EnemyList[j].setX(x); EnemyList[j].setY(y);
                EnemyList[j].setId(2 + (x < curx)); EnemyList[j].setSpeed(1);
                EnemyList[j].setSpawn(true); EnemyList[j].setSpawnTime(SDL_GetTicks64());
                EnemyList[j].setTypes(q); EnemyList[j].setHp(3);
                EnemyList[j].loadCharacter(ren, "GameHKI/Map4/" + to_string(EnemyList[j].GetTypes()));
                break;
            }
            else j++;
        }
        if (obturate) continue;
        NumEnemy++;
        EnemyList[NumEnemy].setX(x); EnemyList[NumEnemy].setY(y);
        EnemyList[NumEnemy].setId(2 + (x < curx));  EnemyList[NumEnemy].setSpeed(1);
        EnemyList[NumEnemy].setSpawn(true); EnemyList[NumEnemy].setSpawnTime(SDL_GetTicks64());
        EnemyList[NumEnemy].setTypes(q); EnemyList[NumEnemy].setHp(3);
        EnemyList[NumEnemy].loadCharacter(ren, "GameHKI/Map4/" + to_string(EnemyList[NumEnemy].GetTypes()));
     }
     f.close();
}
void Normal_Spawn(SDL_Renderer *ren, int tt, int curx){
    std::ifstream f("GameHKI/Map4/Circle.txt");
    int cnt = Rand(3, 10), q = Rand(1, min(5, 1 + tt/10)), j = 1, n, x, y, cur = 0;
    cur = cnt;
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
                EnemyList[j].setId(2 + (x < curx)); EnemyList[j].setSpeed(1);
                EnemyList[j].setSpawn(true); EnemyList[j].setSpawnTime(SDL_GetTicks64());
                EnemyList[j].setTypes(q); EnemyList[j].setHp(3);
                EnemyList[j].loadCharacter(ren, "GameHKI/Map4/" + to_string(EnemyList[j].GetTypes()));
                break;
            }
            else j++;
        }
        if (obturate) continue;
        NumEnemy++;
        EnemyList[NumEnemy].setX(x); EnemyList[NumEnemy].setY(y);
        EnemyList[NumEnemy].setId(2 + (x < curx)); EnemyList[NumEnemy].setSpeed(1);
        EnemyList[NumEnemy].setSpawn(true); EnemyList[NumEnemy].setSpawnTime(SDL_GetTicks64());
        EnemyList[NumEnemy].setTypes(q); EnemyList[NumEnemy].setHp(3);
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
}
void CloseMap4(SDL_Renderer *ren){
    BackGround[4].free();
    Explorer.clean();
    for (int i = 0; i < 4; i++)
        Arrow[i].clean();
    Gametime.Free();

}
bool RunMap4(SDL_Renderer *ren, TTF_Font *font, bool &RunGame){
    LoadMap4(ren);
    SDL_Event e;
    Explorer.setX(SCREEN_WIDTH/2);
    Explorer.setY(SCREEN_HEIGHT/2);
    bool Running = true;
    Uint64 fpstime = SDL_GetTicks64(), lastTimeDamage = SDL_GetTicks64(), circle = SDL_GetTicks64(), normal = SDL_GetTicks64();
    Uint64 EMoveTime = SDL_GetTicks64(), ArrowSpawnTime = SDL_GetTicks64(), ArrowShootTime = SDL_GetTicks64();
    bool Shooting = false;
    int cnt = 0, tt = 0, curx = 2000, cury = 2000;
    NumEnemy = 0;
    //Circle_Spawn(ren, tt, curx);
    while (Running) {
        // Explorer Move.
        while(SDL_PollEvent(&e) != 0){
            if (e.type == SDL_QUIT)
                Running  = false;
            else if (e.type == SDL_KEYDOWN){
                Explorer.setX(curx); Explorer.setY(cury);
                Explorer_Move(Explorer, e, 0, 4000 - CharacterH, 0, 4000 - CharacterW);
                ChangeRMap4(curx, cury);
                cury = Explorer.getY();
                curx = Explorer.getX();
             }
        }

        //Enemy_Spawn
        if ((SDL_GetTicks64() - circle)/1000 >= max(10, 20 - tt/10)){
            Circle_Spawn(ren, tt, curx);
            circle = SDL_GetTicks64();
            tt++;
        }
        else if ((SDL_GetTicks64() - normal)/1000 >= max(1, 5 - tt/10)){
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
        if (SDL_GetTicks64() - ArrowSpawnTime >= 500) {
            if (Shooting == false) {
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
//        // checkCollision
//        CheckCollision4(Explorer, lastTimeDamage);

        //render
        if (SDL_GetTicks64() - fpstime >= 30){
            SDL_SetRenderDrawColor( ren, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear(ren);
            BackGround[4].render(0, 0, ren, &RMap4);
            if (RMap4.y != 0 && RMap4.y != 3370)
                Explorer.setY(SCREEN_HEIGHT/2);
            else  Explorer.setY(cury - RMap4.y);
            if (RMap4.x != 0 && RMap4.x != 2880)
                Explorer.setX(SCREEN_WIDTH/2);
            else  Explorer.setX(curx - RMap4.x);
            Explorer.display(ren);
            Enemy_Display(ren);
            if (Shooting){
                for (int i = 0; i < NumOfArrow; i++){
                    if (! canShoot[i]) continue;
                    int x = Arrow[i].getX(), y = Arrow[i].getY();
                    Arrow[i].setX(x - RMap4.x);
                    Arrow[i].setY(y - RMap4.y);
                    Arrow[i].display(ren);
                    Arrow[i].setX(x); Arrow[i].setY(y);
                }
            }
//            if (! Check_Explorer(lastTimeDamage)) return false;
//
//            if (EnemyMove && Shooting == false){
//                Arrow_Move();
//                for (int i = 0; i < NumOfArrow; i++) canShoot[i] = true;
//                Shooting = true;
//            }
//            if (Shooting){
//                Arrow_Shot(cnt, ren);
//                cnt++; cnt %= 20;
//                if (cnt == 19) { Shooting = false;}
//            }
//            show_game_time(ren, font);
//            show_Enemy_Killed(ren, font);
//            show_ExHp(Explorer, ren, font);
            fpstime = SDL_GetTicks64();
        }
        SDL_RenderPresent(ren);
        //SDL_Delay(3000);
    }
    return true;
}
