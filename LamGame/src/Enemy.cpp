#include "Enemy.h"

Enemy::Enemy()
{
     Character();
}

Enemy::~Enemy()
{
    clean();
}
void Enemy::clean() {
    for (int i = 0; i < 4; i++){
        Dir[i].free();
        Rdir[i].h = 0; Rdir[i].w = 0;
        Rdir[i].x = 0; Rdir[i].y = 0;
        NumFrames[i] = 0;
    }
}
void Enemy:: loadCharacter(SDL_Renderer * ren, std::string path) {
    if (!Dir[0].loadFromFile(path + "/up.png", ren))
        logSDLError(std::cout, "Unable to load Character", 1);
    if (!Dir[1].loadFromFile(path + "/down.png", ren))
        logSDLError(std::cout, "Unable to load Character", 1);
    if (!Dir[2].loadFromFile(path + "/left.png", ren))
        logSDLError(std::cout, "Unable to load Character", 1);
    if (!Dir[3].loadFromFile(path + "/right.png", ren))
        logSDLError(std::cout, "Unable to load Character", 1);
}
