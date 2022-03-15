#include "Weapon.h"

Weapon::Weapon()
{
    Object();
    num = 0;
    RArr = {0, 0, 32, 32};
    Vx = 0; Vy = 0;
    tx = 0; ty = 0;
}

Weapon::~Weapon()
{
    clean();
}
void Weapon:: clean(){
    Arr.free();
    num = 0;

    RArr = {0, 0, 0, 0};
}
int Weapon::getVx(){
    return Vx;
}
int Weapon::getVy(){
    return Vy;
}
void Weapon::setVx(int val) {
    Vx = val;
}
void Weapon::setVy(int val) {
    Vy = val;
}
void Weapon::setNum(int val){
    num = val;
}
void Weapon::display(SDL_Renderer *ren){
    RArr.x = 32*num;
    Arr.render(tx, ty, ren, &RArr);
}
void Weapon::loadWeapon(SDL_Renderer *ren, std::string path) {
    if (!Arr.loadFromFile("GameHKI/Map4/Arrow/" + path, ren))
        std::cout << "Unable to load Arrow";
}
