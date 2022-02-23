#include "Skeleton.h"

Skeleton::Skeleton()
{
     Character();
}

Skeleton::~Skeleton()
{
    clean();
}

void Skeleton::clean() {
    for (int i = 0; i < 4; i++){
        Dir[i].free();
        Rdir[i].h = 0; Rdir[i].w = 0;
        Rdir[i].x = 0; Rdir[i].y = 0;
        NumAnimation[i] = 0;
    }
}
void Skeleton:: loadCharacter(SDL_Renderer * ren) {
    if (!Dir[0].loadFromFile("GameHKI/Skeleton/up.png", ren))
        logSDLError(std::cout, "Unable to load Character", 1);
    if (!Dir[1].loadFromFile("GameHKI/Skeleton/down.png", ren))
        logSDLError(std::cout, "Unable to load Character", 1);
    if (!Dir[2].loadFromFile("GameHKI/Skeleton/left.png", ren))
        logSDLError(std::cout, "Unable to load Character", 1);
    if (!Dir[3].loadFromFile("GameHKI/Skeleton/right.png", ren))
        logSDLError(std::cout, "Unable to load Character", 1);
}
