#include "Map2.h"
void movingin(MainCharacter &Explorer, SDL_Renderer *ren, bool &RunGame){
    Explorer.setX(700); Explorer.setY(570);
    Rsphinx.w = 160; Rsphinx.h = 140;
    bool Running = true, done = false;  int cur = 7, x = -160, y = 260;
    Uint64 fpstime = SDL_GetTicks64(); Uint64 changetime = SDL_GetTicks64();
    SDL_Event e;
    while (Running) {
         while( SDL_PollEvent(&e) != 0){
            //User requests quit
            if( e.type == SDL_QUIT ){
                RunGame = false;
                return;
            }
        }
        if (SDL_GetTicks64() - changetime >= 80 && !done){
            cur = (cur - 1); if (cur < 0) cur += 8;
            changetime = SDL_GetTicks64();
            Rsphinx.x = cur * 160;
            if (Explorer.getY() != 350) Explorer.goUp(350, MainFrames, 0);
            else if (Explorer.getX() != 670) Explorer.goLeft(670, MainFrames, 2);
            x = min(320, x + 7);

        }
        if (SDL_GetTicks64() - fpstime >= 33){
            SDL_RenderClear(ren);
            BackGround[2].render(0, 0, ren, NULL);
            Explorer.display(ren);
            Sphinx.render(x, y, ren, &Rsphinx);
            SDL_RenderPresent(ren);
            if (x == 320) {Running = false;}
        }
        //if (!Running) SDL_Delay(300);
        //if (x == 320) Running = false;
    }
}
void talk(MainCharacter &Explorer, SDL_Renderer *ren, bool &RunGame){
    Sphinx.loadFromFile("GameHKI/Map2/sphinx33.png", ren);
    int talking = 0, cur = 7;
    Uint64 fpstime = SDL_GetTicks64(), movingtime = SDL_GetTicks64();
    while (talking != 5){
        SDL_Event e;
        while( SDL_PollEvent(&e) != 0){
            //User requests quit
            if( e.type == SDL_QUIT ){
                RunGame = false;
                return;
            }
            //Handle button events
            if(e.type == SDL_MOUSEBUTTONDOWN) {
                talking++;
                break;
            }
        }
        if (SDL_GetTicks64() - movingtime >= 150){
            cur--; if (cur < 0) cur += 8; Rsphinx.x = cur * 160;
            movingtime = SDL_GetTicks64();
        }
        if (SDL_GetTicks64() - fpstime >= 33){
            SDL_RenderClear(ren);
            BackGround[2].render(0, 0, ren, NULL);
            Sphinx.render(320, 260, ren, &Rsphinx);
            Explorer.display(ren);
            Rtalk.x = 200 * talking;
            Talk.render(470, 200, ren, &Rtalk);
            SDL_RenderPresent(ren);
        }
    }
}
void LoadMap2(SDL_Renderer *ren){
    BackGround[2].loadFromFile("GameHKI/Map2/Map22.png", ren);
    Explorer.loadCharacter(ren); Explorer.setSpeed(5);
    Sphinx.loadFromFile("GameHKI/Map2/sphinx22.png", ren);
    Talk.loadFromFile("GameHKI/Map2/Talk/talkkk.png", ren);
    result[1].loadFromFile("GameHKI/Map2/Talk/win.png", ren);
    result[0].loadFromFile("GameHKI/Map2/Talk/lose.png", ren);
    guess.loadFromFile("GameHKI/Map2/Talk/guess.png", ren);
    gframe.loadFromFile("GameHKI/Map2/gframe.png", ren);
    Rtalk = {0, 0, 200, 120}; Rgframe = {0, 0, 200, 150};
    Rguess = {0, 0,200, 120};
    TextGuess.SetColor(TextObject::BLACK_TEXT);
    showNum.SetColor(TextObject::BLACK_TEXT);
}
void CloseMap2(SDL_Renderer *ren){
    BackGround[2].free(); Explorer.clean();
    Sphinx.free(); Talk.free();
    result[0].free(); result[1].free();
    guess.free(); TextGuess.Free(); showNum.Free(); gframe.free();
}
int to_num(string s){
    int p = 0;
    for (int i = 0; i < s.size(); i++)
        p = p * 10 + s[i] - '0';
    return p;
}
bool RunMap2(SDL_Renderer *ren, TTF_Font *font, bool &RunGame){
    LoadMap2(ren);
    Intro(ren, RunGame, 2);
    if (! RunGame) {CloseMap2(ren); return 0;}
    movingin(Explorer, ren, RunGame);
    if (! RunGame) {CloseMap2(ren); return 0;}
    talk(Explorer, ren, RunGame);
    if (! RunGame) {CloseMap2(ren); return 0;}
    string inputText = "";
    int res = Rand(1, 99), gtimes = 8, x, y, state = 0, cur = 7;
    bool win = false, lose = false, guessing = false, Running  = true, input = false;
    SDL_Event e; Uint64 fpstime = SDL_GetTicks64(), movingtime = SDL_GetTicks64();
    while (gtimes) {
        SDL_RenderClear(ren);
        if (! guessing){
            SDL_StartTextInput();
            while( SDL_PollEvent(&e) != 0){
                if( e.type == SDL_QUIT ){
                    RunGame = false;
                    CloseMap2(ren);
                    return 0;
                }
                if(e.type == SDL_MOUSEBUTTONDOWN) {
                    SDL_GetMouseState(&x, &y);
                    if (x >= 528 && x <= 627 && y >= 527 && y <= 560) input = true;
                    else if (x >= 563 && x <= 596 && y >= 567 && y <= 591) {
                        input = false;
                        guessing = true;
                        gtimes--;
                    }
                }
                if(input &&e.type == SDL_KEYDOWN){
                    if(e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)
                        inputText.pop_back();
                }
                else if(input && e.type == SDL_TEXTINPUT && inputText.size() < 3){
                    if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) ) ){
                        inputText += e.text.text;
                    }
                }
            }
            SDL_StopTextInput();
        }
        else {
            while( SDL_PollEvent(&e) != 0){
                if(e.type == SDL_QUIT) {
                    RunGame = false;
                    CloseMap2(ren);
                    return 0;
                }
                if(e.type == SDL_MOUSEBUTTONDOWN) {
                    guessing = false;
                    SDL_GetMouseState(&x, &y);
                    if (x >= 528 && x <= 627 && y >= 527 && y <= 560) input = true;
                }
            }
            if (!RunGame) break;
            if (win) break;
        }
        if (SDL_GetTicks64() - movingtime >= 150){
            cur--; if (cur < 0) cur += 8; Rsphinx.x = cur * 160;
            movingtime = SDL_GetTicks64();
        }
        if (SDL_GetTicks64() - fpstime >= 33){
            SDL_RenderClear(ren);
            BackGround[2].render(0, 0, ren, NULL);
            Sphinx.render(320, 260, ren, &Rsphinx);
            Explorer.display(ren);
            if (guessing){
                int d = to_num(inputText);
                if (d > res) Rguess.x = 0;
                else if (d < res) Rguess.x = 200;
                else win = true;
                if (gtimes == 1 && ! win) lose = true;
                if (! win) guess.render(470, 200, ren, &Rguess);
                Rgframe.x = 200;
            }
            else Rgframe.x = 0;
            gframe.render(478, 465, ren, &Rgframe);
            show_frame(ren, font, 2);
            showNum.setText(std::to_string(9 - gtimes) + " / 7");
            if (inputText.size()) TextGuess.setText(inputText.c_str());
            else TextGuess.setText(" ");
            TextGuess.LoadFromRenderText(font, ren);
            showNum.LoadFromRenderText(font, ren);
            TextGuess.RenderText(ren, 560, 525);
            showNum.RenderText(ren, 50, 30);
            if (win) result[1].render(470, 200, ren, NULL);
            if (lose) result[0].render(470, 200, ren, NULL);
            SDL_RenderPresent(ren);
            fpstime = SDL_GetTicks64();
        }
        if (win || lose) {
            SDL_Delay(3000);
            break;
        }
    }
    CloseMap2(ren);
    return win;
}
