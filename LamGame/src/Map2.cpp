#include "Map2.h"

void LoadMap2(SDL_Renderer *ren){
    BackGround[2].loadFromFile("GameHKI/Map2/Map22.png", ren);
    mainp.loadFromFile("GameHKI/Map2/explorer.png", ren);
    Sphinx.loadFromFile("GameHKI/Map2/sphinx.png", ren);
    Talk.loadFromFile("GameHKI/Map2/Talk/talkspritesheet.png", ren);
    result[1].loadFromFile("GameHKI/Map2/Talk/win.png", ren);
    result[0].loadFromFile("GameHKI/Map2/Talk/lose.png", ren);
    guess.loadFromFile("GameHKI/Map2/Talk/guess.png", ren);
    button.loadFromFile("GameHKI/Map2/guess.png", ren);
    blank.loadFromFile("GameHKI/Map2/rect.png", ren);
    Rtalk = {0, 0, 400, 213};
    Rguess = {0, 0,200, 120};
    Rbutton = {0, 0, 128, 64};
    Rblank = {0, 0, 120, 45};
    TextGuess.SetColor(TextObject::BLACK_TEXT);
    showNum.SetColor(TextObject::BLACK_TEXT);

}
void CloseMap2(SDL_Renderer *ren){
    BackGround[2].free();
    mainp.free(); Sphinx.free(); Talk.free();
    result[0].free(); result[1].free(); button.free(); blank.free();
    guess.free(); TextGuess.Free();
}
void Intro2(SDL_Renderer *ren){
    LoadObject intro1;
    intro1.loadFromFile("GameHKI/Intro/intro2.png", ren);
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
        BackGround[2].render(0, 0, ren, &RMap);
        intro1.render(0, 0, ren, NULL);
        SDL_RenderPresent(ren);
    }
    intro1.free();
}
int to_num(string s){
    int p = 0;
    for (int i = 0; i < s.size(); i++)
        p = p * 10 + s[i] - '0';
    return p;
}
bool RunMap2(SDL_Renderer *ren, TTF_Font *font){
    LoadMap2(ren);
    Intro2(ren);
    string inputText = "";
    int res = Rand(1, 99);
    cout << res << '\n';
    int talking = 0;
    Uint64 st = SDL_GetTicks64();
    while (talking != 4){
        SDL_Event e;
        while( SDL_PollEvent(&e) != 0){

            //User requests quit
            if( e.type == SDL_QUIT ){
                return 0;
            }
            //Handle button events
            if(e.type == SDL_MOUSEBUTTONDOWN) {
                talking++;
                break;
            }
        }
        SDL_RenderClear(ren);
        BackGround[2].render(0, 0, ren, NULL);
        mainp.render(920, 360, ren, NULL);
        Sphinx.render(0, 170, ren, NULL);
        Rtalk.x = 400 * talking;
        if (SDL_GetTicks64() - st >= 800)
            Talk.render(400, 100, ren, &Rtalk);
        SDL_RenderPresent(ren);
    }
    int gtimes = 7, x, y, state = 0;
    bool win = false, guessing = false, Running  = true;
    while (gtimes){
        if (win) {
            SDL_Delay(1000);
            break;
        }
        SDL_RenderClear(ren);
        SDL_Event e;
        BackGround[2].render(0, 0, ren, NULL);
        mainp.render(920, 360, ren, NULL);
        Sphinx.render(0, 170, ren, NULL);
        if (! guessing){
            SDL_StartTextInput();
            while( SDL_PollEvent(&e) != 0){
                if( e.type == SDL_QUIT )return 0;
                if(e.type == SDL_MOUSEBUTTONDOWN) {
                    SDL_GetMouseState(&x, &y);
                    if (x >= 530 && x <= 650 && y >= 480 && y <= 525) Rblank.x = 120;
                    else Rblank.x = 0;
                    if (x >= 530 && x <= 658 && y >= 550 && y <= 614) {
                        Rbutton.x = 128; gtimes--;
                        guessing = true;
                        int d = to_num(inputText);
                        //cout << d << '\n';
                        if (d > res) state = 1;
                        else if (d == res) win = 3;
                        else state = 2;
                        if (d != res && gtimes == 0) state = 4;
                    }
                    else Rbutton.x = 0;
                    break;
                }
               else if( Rblank.x == 120 &&e.type == SDL_KEYDOWN ){
                    if( e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
                        inputText.pop_back();
                }
                else if(Rblank.x == 120 && e.type == SDL_TEXTINPUT && inputText.size() < 3){
                    if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) ) ){
                        inputText += e.text.text;
                    }
                }
            }
            SDL_StopTextInput();
        }
        else {
            while( SDL_PollEvent(&e) != 0){
                if( e.type == SDL_QUIT )return 0;
                if(e.type == SDL_MOUSEBUTTONDOWN) {
                    guessing = false;
                    state = 0;
                    break;
                }
            }
            if (state == 1)  {// bigger
                Rguess.x = 0;
                guess.render(400, 100, ren, &Rguess);
            }
            if (state == 2) {//smaller
                Rguess.x = 200;
                guess.render(400, 100, ren, &Rguess);
            }
            if (win) break;
        }
        if (Rbutton.x == 120) {Rblank.x = 0;}
        blank.render(530, 480, ren, &Rblank);
        button.render(530, 550, ren, &Rbutton);
        if (inputText.size()) TextGuess.setText(inputText.c_str());
        else TextGuess.setText(" ");
        TextGuess.LoadFromRenderText(font, ren);
        TextGuess.RenderText(ren, 530, 480);
        string str_val = "Times: ";
        str_val += to_string(gtimes);
        showNum.setText(str_val.c_str());
        showNum.LoadFromRenderText(font, ren);
        showNum.RenderText(ren, 0, 0);
        SDL_RenderPresent(ren);
    }
    while (Running) {
        SDL_Event e;
         while( SDL_PollEvent(&e) != 0){
                if( e.type == SDL_QUIT )return 0;
                if(e.type == SDL_MOUSEBUTTONDOWN) {
                    Running = false;
                }
        }
        SDL_RenderClear(ren);
        BackGround[2].render(0, 0, ren, NULL);
        mainp.render(920, 360, ren, NULL);
        Sphinx.render(0, 170, ren, NULL);
        result[win].render(400, 100, ren, NULL);
        SDL_RenderPresent(ren);
    }
    CloseMap2(ren);
    return win;
}
