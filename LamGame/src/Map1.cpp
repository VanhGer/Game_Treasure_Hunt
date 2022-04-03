#include "Map1.h"
void show_frame(SDL_Renderer *ren, TTF_Font *font, int id){
    LoadObject Frame;
    Frame.loadFromFile("GameHKI/Frame/frame" + std::to_string(id) + ".png", ren);
    Frame.render(0, 0, ren, NULL);
    Frame.free();
}
void show_num(SDL_Renderer *ren, TTF_Font *font, int a, int b){
    TextObject num, num2;
    string str_val = std::to_string(a - b - 1);
    str_val += " vs ";
    str_val += std::to_string(b);
    num.setText(str_val);
    num.SetColor(TextObject:: BLACK_TEXT);
    num.LoadFromRenderText(font, ren);
    num.RenderText(ren, 63, 60);
    string dstr_val = std::to_string(a);
    dstr_val += "/5";
    num2.setText(dstr_val);
    num2.SetColor(TextObject::BLACK_TEXT);
    num2.LoadFromRenderText(font, ren);
    num2.RenderText(ren, 80, 15);
    num.Free(); num2.Free();
}
void Ex_come(MainCharacter &Explorer, SDL_Renderer *ren, bool &RunGame){
    LoadObject t; t.loadFromFile("GameHKI/Map1/talk.png", ren);
    bool Running = true, talk = false;
    Explorer.setX(545); Explorer.setY(590); Explorer.setSpeed(5);
    SDL_Event e;
    int cur = 0;
    SDL_Rect R1; R1.w = 35; R1.h = 50;
    Uint64 fpstime = SDL_GetTicks64(), movingtime = SDL_GetTicks64();
    while (Running){
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                RunGame = false;
                return;
            }
        }
        if (talk) Running = false;
        if (SDL_GetTicks64() - movingtime >= 50 && ! talk){
            if (Explorer.getY() != 275) Explorer.goUp(275, MainFrames, 0);
            else {
                if (Explorer.getX() != 680) Explorer.goRight(680, MainFrames, 3);
                else {Explorer.goLeft(670, MainFrames, 2); talk = true; }
                cur = 1;
            }
            movingtime = SDL_GetTicks64();
        }
        if (SDL_GetTicks64() - fpstime >= 33){
            SDL_RenderClear(ren);
            BackGround[1].render(0, 0, ren, NULL);
            R1.x = cur * 35; monk.render(415, 275, ren, &R1);
            Explorer.display(ren);
            if (talk) t.render(360, 150, ren, NULL);
            if (! Running) SDL_Delay(1500);
            SDL_RenderPresent(ren);
        }
    }
}
void LoadMap1(SDL_Renderer *ren){
    BackGround[1].loadFromFile("GameHKI/Map1/map1.png", ren);
    showNum.SetColor(TextObject::WHITE_TEXT);
    for (int i = 0; i < 3; i++){
        OTT[i].h = 120; OTT[i].w = 120; OTT[i].x = 0; OTT[i].y = 0;
    }
    pos[0] = {240, 20}; pos[1] = {490, 20}; pos[2] = {740, 20};
    RNumber = {0, 0, 80, 148};
    Explorer.loadCharacter(ren);
    result[0].loadFromFile("GameHKI/Map1/win.png", ren);
    result[1].loadFromFile("GameHKI/Map1/lose.png", ren);
    tg[0].loadFromFile("GameHKI/Map1/1.png", ren);
    tg[1].loadFromFile("GameHKI/Map1/2.png", ren);
    tg[2].loadFromFile("GameHKI/Map1/3.png", ren);
    choice[0].loadFromFile("GameHKI/Map1/rock.png", ren);
    choice[1].loadFromFile("GameHKI/Map1/paper.png", ren);
    choice[2].loadFromFile("GameHKI/Map1/scissors.png", ren);
    rock.loadFromFile("GameHKI/Map1/rock1.png", ren);
    paper.loadFromFile("GameHKI/Map1/paper1.png", ren);
    scissors.loadFromFile("GameHKI/Map1/scissors1.png", ren);
    monk.loadFromFile("GameHKI/Map1/monk.png", ren);
}
void CloseMap1(SDL_Renderer *ren){
    rock.free(); paper.free(); scissors.free(); monk.free(); mainp.free();
    for (int i = 0; i < 3; i++) {tg[i].free(); choice[i].free();}
    result[0].free(); result[1].free();
    showNum.Free(); Explorer.clean();
}
void Intro(SDL_Renderer *ren, bool &RunGame, int id){
    LoadObject intro;
    intro.loadFromFile("GameHKI/Intro/intro" +std::to_string(id) + ".png", ren);
    SDL_Event e; bool Running = true;
    while (Running) {
        while( SDL_PollEvent( &e ) != 0 ){
            //User requests quit
            if( e.type == SDL_QUIT ){
                Running = false;
                RunGame = false;
            }
            //Handle button events
            if (e.type == SDL_MOUSEBUTTONDOWN)
                Running = false;
        }
        intro.render(0, 0, ren, NULL);
        SDL_RenderPresent(ren);
    }
    intro.free();
}
bool RunMap1(SDL_Renderer *ren, TTF_Font *font, bool &RunGame){
    LoadMap1(ren);
    Intro(ren, RunGame, 1);
    Ex_come(Explorer, ren, RunGame);
    if (! RunGame) {
        CloseMap1(ren);
        return 0;
    }
    Uint64 StartTime = SDL_GetTicks64(), cnttime, showchoice;
    SDL_Rect R1; R1.w = 35; R1.h = 50; R1.x = 35;
    int click = 3, Running = 1, x,y, Pchoice = -1, COMchoice;
    int NumGames = 1, NumWins = 0;
    bool donecount = true, dowin = true;
    while (Running) {
        if (SDL_GetTicks64() - StartTime <= 30) continue;
        SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear(ren);
        BackGround[1].render(0, 0, ren, NULL);
        monk.render(415, 275, ren, &R1);
        Explorer.display(ren);
        rock.render(pos[0].x, pos[0].y, ren, &OTT[0]);
        paper.render(pos[1].x, pos[1].y, ren, &OTT[1]);
        scissors.render(pos[2].x, pos[2].y, ren, &OTT[2]);
        SDL_Event e;
        while( SDL_PollEvent( &e ) != 0 && Pchoice == -1){
            //User requests quit
            if( e.type == SDL_QUIT ){
                Running = false;
                RunGame = false;
            }
            //Handle button events
            if(click == 3 && (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)){
                SDL_GetMouseState(&x, &y);
                for (int i = 0; i < 3; i++){
                    bool inside = true;
                    if (x < pos[i].x || x > pos[i].x + OTT[i].w || y < pos[i].y || y > pos[i].y + OTT[i].h)
                        inside = false;
                    if (! inside) OTT[i].x = 0;
                    else {
                        if (e.type == SDL_MOUSEBUTTONDOWN) {
                                cnttime = SDL_GetTicks64(); Pchoice = i;
                                click = 0; OTT[i].x = 120; donecount = false;
                        }
                        if (e.type == SDL_MOUSEBUTTONUP || e.type == SDL_MOUSEMOTION) OTT[i].x = 0;
                    }
                }
            }
        }
        if (!Running) break;
        if (Pchoice != -1){
            if (! donecount){
                if (SDL_GetTicks64() - cnttime >= 1000 && click != 3) {
                        click++; cnttime = SDL_GetTicks64();
                        if (click == 3) {
                            donecount = true;
                            COMchoice = Rand(0, 1);
                            if (COMchoice) COMchoice = (Pchoice - 1 + 3) % 3;
                            else  COMchoice = (Pchoice + 1 + 3) % 3;
                            showchoice = SDL_GetTicks64();
                        }
                }
                if (click != 3)
                    tg[click].render(510, 260, ren, &RNumber);
            }
            if (donecount){
                if (SDL_GetTicks64() - showchoice <= 2000){
                    choice[Pchoice].render(600, 200, ren, NULL);
                    choice[COMchoice].render(470, 200, ren, NULL);
                }
                else {
                    if ((Pchoice == 0 && COMchoice == 2) || (Pchoice == 1 && COMchoice == 0)
                        || (Pchoice == 2 && COMchoice == 1))
                            NumWins++;
                    NumGames++;
                    Pchoice = -1;
                }
            }
        }
        StartTime = SDL_GetTicks64();
        if (NumWins == 3) {
            result[0].render(465, 175, ren, NULL);
            SDL_RenderPresent(ren);
            Running = false;
        }
        if (NumGames - NumWins > 3 ||(NumGames - 1 == 5 && NumWins < 3)) {
            dowin = false;
            result[1].render(440, 175, ren, NULL);
            SDL_RenderPresent(ren);
            Running = false;
        }
        show_frame(ren, font, 1);
        show_num(ren, font, NumGames, NumWins);
        SDL_RenderPresent(ren);
        if (! Running) SDL_Delay(2000);
    }
    // close
    CloseMap1(ren);
    return dowin;

}
