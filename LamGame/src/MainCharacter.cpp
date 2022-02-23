#include "MainCharacter.h"

MainCharacter:: MainCharacter(){
    Character();
}
MainCharacter:: ~MainCharacter(){
    clean();
}
void MainCharacter:: clean(){
    for (int i = 0; i < 4; i++){
        Dir[i].free();
        Rdir[i].h = 0; Rdir[i].w = 0;
        Rdir[i].x = 0; Rdir[i].y = 0;
        NumAnimation[i] = 0;
    }
}
void MainCharacter:: loadCharacter(SDL_Renderer * ren) {
    if (!Dir[0].loadFromFile("GameHKI/Explorer/up.png", ren))
        logSDLError(std::cout, "Unable to load Character", 1);
    if (!Dir[1].loadFromFile("GameHKI/Explorer/down.png", ren))
        logSDLError(std::cout, "Unable to load Character", 1);
    if (!Dir[2].loadFromFile("GameHKI/Explorer/left.png", ren))
        logSDLError(std::cout, "Unable to load Character", 1);
    if (!Dir[3].loadFromFile("GameHKI/Explorer/right.png", ren))
        logSDLError(std::cout, "Unable to load Character", 1);
}