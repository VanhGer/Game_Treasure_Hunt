#include "Weapon.h"

Weapon::Weapon()
{
    Object();
    num = 0;
    RArr = {0, 0, 32, 32};
}

Weapon::~Weapon()
{
    clean();
}
void Weapon::clean(){
    Arr.free();
    num = 0;
    RArr = {0, 0, 0, 0};
}
void Weapon::display(SDL_Renderer *ren){
    RArr.x += 32*num;
    Arr.render(tx, ty, ren, &RArr);
}
void Weapon::loadWeapon(SDL_Renderer *ren) {
    if (!Arr.loadFromFile("GameHKI/Arrow/Arrow.png", ren))
        logSDLError(std::cout, "Unable to load Arrow", 1);
}
