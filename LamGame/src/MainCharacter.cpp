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
        NumFrames[i] = 0;
    }
}
void MainCharacter:: loadCharacter(SDL_Renderer * ren) {
    if (!Dir[0].loadFromFile("GameHKI/Map4/Explorer/up1.png", ren))
        std::cout << "Unable to load Character" << '\n';
    if (!Dir[1].loadFromFile("GameHKI/Map4/Explorer/down.png", ren))
        std::cout << "Unable to load Character" << '\n';
    if (!Dir[2].loadFromFile("GameHKI/Map4/Explorer/left.png", ren))
        std::cout << "Unable to load Character" << '\n';
    if (!Dir[3].loadFromFile("GameHKI/Map4/Explorer/right.png", ren))
        std::cout << "Unable to load Character" << '\n';
}
