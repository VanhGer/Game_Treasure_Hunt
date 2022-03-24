#include "Map4.h"
void loadArrow(SDL_Renderer *ren){
    if (idArrow == 1)
         for (int i = 0; i < 4; i++){
            Arrow[i].loadWeapon(ren, "Arrow2.png");
            Arrow[i].setSpeed(ArrowSpeed);
        }
    else
        for (int i = 0; i < 4; i++){
            Arrow[i].loadWeapon(ren, "Arrow3.png");
            Arrow[i].setSpeed(ArrowSpeed);
        }
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
void Enemy_Spawn(int id, SDL_Renderer *ren){
    pii tmp;
    for (int i = 0; i < type[id].fi; i++){
        int x = EnemyCount[0];
        E[0][x].loadCharacter(ren, "GameHKI/Map4/Skeleton");
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
        E[1][x].loadCharacter(ren, "GameHKI/Map4/Zombie");
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
        E[2][x].loadCharacter(ren, "GameHKI/Map4/Shrek");
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
void Arrow_Shot(int frame, SDL_Renderer *ren){
    for (int i = 0; i < NumOfArrow; i++){
        if (! canShoot[i]) continue;
        Arrow[i].setX(Arrow[i].getX() + Arrow[i].getVx());
        Arrow[i].setY(Arrow[i].getY() + Arrow[i].getVy());
        //cout << Arrow[i].getX() << " " << Arrow[i].getY() << '\n';
        Arrow[i].setNum(frame/ 2 % ArrowFrames);
        Arrow[i].display(ren);
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
                            loadArrow(ren);
                        }
                    }
                }
                canShoot[i] = false;
                Arrow[i].setNum(2);
                Arrow[i].display(ren);
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
void Explorer_Move(MainCharacter &Explorer, SDL_Event e, int minY, int maxY, int minX, int maxX){
    //cout << minY << " " << maxY << " " << minX << " " << maxX << '\n';
    //cout << Explorer.getX() << " b " << Explorer.getY() << '\n';
    if (e.key.keysym.sym == SDLK_UP){
        Explorer.goUp(minY, MainFrames);
    }
    else if (e.key.keysym.sym == SDLK_DOWN){
        Explorer.goDown(maxY, MainFrames);
    }
    else if (e.key.keysym.sym == SDLK_LEFT){
        Explorer.goLeft(minX, MainFrames);
    }
    else if (e.key.keysym.sym == SDLK_RIGHT){
        Explorer.goRight( maxX, MainFrames);
    }
    //cout << Explorer.getX() << " a " << Explorer.getY() << '\n';
}
void show_game_time(SDL_Renderer *ren, TTF_Font *font){
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
    Gametime.LoadFromRenderText(font, ren);
    Gametime.RenderText(ren, SCREEN_WIDTH - 200, 15);
}
void show_Enemy_Killed(SDL_Renderer *ren, TTF_Font *font){
    TextObject NumEK;
    NumEK.SetColor(TextObject::WHITE_TEXT);
    std::string str_ek = "Enemies Killed: ";
    std::string str_num = std::to_string(numEnemyKilled);
    str_ek += str_num;
    NumEK.setText(str_ek);
    NumEK.LoadFromRenderText(font, ren);
    NumEK.RenderText(ren, SCREEN_WIDTH/2, 15);
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
void LoadMap4(SDL_Renderer *ren){
    BackGround[4].loadFromFile("GameHKI/Map4/map4.png", ren);
    // load MainCharacter
    Explorer.loadCharacter(ren);
    Explorer.setSpeed(MainSpeed);
    Explorer.setHp(MainHp);
    loadArrow(ren);
}
bool RunMap4(SDL_Renderer *ren, TTF_Font *font){
    LoadMap4(ren);

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
            Enemy_Spawn(tt, ren);
        }
        while(SDL_PollEvent(&e) != 0){
            if (e.type == SDL_QUIT)
                Running  = false;
            else if (e.type == SDL_KEYDOWN){
                Explorer_Move(Explorer, e, 0, SCREEN_HEIGHT - CharacterH, 0, SCREEN_WIDTH - CharacterW);
                if (! EnemySpawn && SDL_GetTicks64() > PlayingTime + 3000){
                    Enemy_Spawn(0, ren);
                    EnemySpawn = true;
                    PlayingTime = SDL_GetTicks64();
                }
             }
        }
        //render
        if (SDL_GetTicks64() - tmp >= 30){
            SDL_SetRenderDrawColor( ren, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear(ren);
            BackGround[4].render(0, 0, ren, NULL);
            Explorer.display(ren);
            if (! Check_Explorer(lastTimeDamage)) return false;
            for (int i = 0; i < EnemyList.size(); i++){
                pii Q = EnemyList[i];
                //cout << i << " " << EnemyList[i].getX() << " " << EnemyList[i].getY() << '\n';
                E[Q.fi][Q.se].display(ren);
            }
            //Show game time

            if (EnemyMove && Shooting == false){
                Arrow_Move();
                for (int i = 0; i < NumOfArrow; i++) canShoot[i] = true;
                Shooting = true;
            }
            if (Shooting){
                Arrow_Shot(cnt, ren);
                cnt++; cnt %= 20;
                if (cnt == 19) { Shooting = false;}
            }
            show_game_time(ren, font);
            show_Enemy_Killed(ren, font);
            show_ExHp(Explorer, ren, font);
            tmp = SDL_GetTicks64();
        }
        SDL_RenderPresent(ren);
        //SDL_Delay(3000);
    }
    return true;
}
