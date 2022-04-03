#include "Map5.h"

void CloseMap5(SDL_Renderer *ren){
    treasure.free();
    congra.free();
    bg.free();
}
void LoadMap5(SDL_Renderer *ren){
    treasure.loadFromFile("GameHKI/Map5/treasure.png", ren);
    congra.loadFromFile("GameHKI/Map5/congra.png", ren);
    bg.loadFromFile("GameHKI/Map5/bg.png", ren);
}
void RunMap5(SDL_Renderer *ren, bool &RunGame){
    LoadMap5(ren);
    bool Running = true;
    SDL_Event e;
    int frame = 0;
    SDL_Rect Rt; Rt.x = 0; Rt.y = 0; Rt.h = 468; Rt.w = 533;
    Uint64 fpstime = SDL_GetTicks64(), changeframe = SDL_GetTicks64();
    while (Running){
         while(SDL_PollEvent(&e) != 0){
            if (e.type == SDL_QUIT){
                RunGame = false;
                CloseMap5(ren);
                return;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN && frame == 6){
                CloseMap5(ren);
                return;
             }
        }
        if (SDL_GetTicks64() - changeframe >= 500){
            frame = min(6, frame + 1);
            changeframe = SDL_GetTicks64();
            Rt.x = frame  *533;
        }
        if (SDL_GetTicks64() - fpstime >= 33){
            SDL_RenderClear(ren);
            bg.render(0, 0, ren, NULL);
            congra.render(150, 20, ren, NULL);
            treasure.render(300, 150, ren, &Rt);
            SDL_RenderPresent(ren);
            fpstime = SDL_GetTicks64();
        }
    }
    CloseMap5(ren);
}
