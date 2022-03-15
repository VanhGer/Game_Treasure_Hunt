#include "Map1.h"

void LoadMap1(SDL_Renderer *ren){
    BackGround[1].loadFromFile("GameHKI/Map1/map1.png", ren);
    showNum.SetColor(TextObject::WHITE_TEXT);
    for (int i = 0; i < 3; i++){
        OTT[i].h = 120; OTT[i].w = 120; OTT[i].x = 0; OTT[i].y = 0;
    }
    pos[0] = {240, 20}; pos[1] = {490, 20}; pos[2] = {740, 20};
    RNumber = {0, 0, 80, 148};
    mainp.loadFromFile("GameHKI/Map1/explorer.png", ren);
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
    showNum.Free();
}
void Intro1(SDL_Renderer *ren){
    LoadObject intro1;
    intro1.loadFromFile("GameHKI/Intro/intro1.png", ren);
    SDL_Event e; bool Running = true;
    while (Running) {
        while( SDL_PollEvent( &e ) != 0 ){
            //User requests quit
            if( e.type == SDL_QUIT ){
                Running = false;
            }
            //Handle button events
            if (e.type == SDL_MOUSEBUTTONDOWN)
                Running = false;
        }
        BackGround[1].render(0, 0, ren, NULL);
        intro1.render(0, 0, ren, NULL);
        SDL_RenderPresent(ren);
    }
    intro1.free();
}
bool RunMap1(SDL_Renderer *ren, TTF_Font *font){
    LoadMap1(ren);
    Intro1(ren);
    Uint64 StartTime = SDL_GetTicks64(), cnttime, showchoice;
    int click = 3, Running = 1, x,y, Pchoice = -1, COMchoice;
    int NumGames = 1, NumWins = 0;
    bool donecount = true, dowin = true;
    while (Running) {
        if (SDL_GetTicks64() - StartTime <= 30) continue;
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor( ren, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear(ren);
        BackGround[1].render(0, 0, ren, NULL);
        monk.render(0, SCREEN_HEIGHT - 480, ren, NULL);
        mainp.render(778, SCREEN_HEIGHT - 480, ren, NULL);
        rock.render(pos[0].x, pos[0].y, ren, &OTT[0]);
        paper.render(pos[1].x, pos[1].y, ren, &OTT[1]);
        scissors.render(pos[2].x, pos[2].y, ren, &OTT[2]);
        SDL_Event e;
        while( SDL_PollEvent( &e ) != 0 && Pchoice == -1){
            //User requests quit
            if( e.type == SDL_QUIT ){
                Running = false;
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
                    tg[click].render(520, 260, ren, &RNumber);
            }
            if (donecount){
                if (SDL_GetTicks64() - showchoice <= 1500){
                    choice[Pchoice].render(700, 300, ren, NULL);
                    choice[COMchoice].render(300, 300, ren, NULL);
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
        string str_val = "Win :";
        str_val += std::to_string(NumWins);
        str_val += "  Games : ";
        str_val += std::to_string(NumGames);
        str_val += " /5";
        showNum.setText(str_val);
        showNum.LoadFromRenderText(font, ren);
        showNum.RenderText(ren, 0, 0);
        StartTime = SDL_GetTicks64();
        if (NumWins == 3) {
            result[0].render(485, 200, ren, NULL);
            SDL_RenderPresent(ren);
            SDL_Delay(1000);
            break;
        }
        if (NumGames - NumWins > 3 ||(NumGames - 1 == 5 && NumWins < 3)) {
            dowin = false;
            result[1].render(485, 200, ren, NULL);
            SDL_RenderPresent(ren);
            SDL_Delay(1000);
            break;
        }
        SDL_RenderPresent(ren);

    }
    // close
    CloseMap1(ren);
    return dowin;

}
