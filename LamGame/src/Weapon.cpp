#include "Weapon.h"

Weapon::Weapon()
{
    Object();
    num = 0; damn = 0;
    Vx = 0; Vy = 0;
    tx = 0; ty = 0;
}
Weapon::~Weapon(){
    clean();
}
void Weapon:: clean(){
    Obj.free();
    num = 0;
    add = 0;
    RObj = {0, 0, 0, 0};
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
void Weapon::setAdd(int val){
    add = val;
}
void Weapon::setRObj(int x, int y, int w, int h){
    RObj.x = x; RObj.y = y; RObj.w = w; RObj.h = h;
}
void Weapon::display(SDL_Renderer *ren){
    RObj.x = add*num;
    Obj.render(tx, ty, ren, &RObj);
}
void Weapon::setDamn(int val){
    damn = val;
}
int Weapon::getDamn(){
    return damn;
}
void Weapon::loadWeapon(SDL_Renderer *ren, std::string path) {
    if (!Obj.loadFromFile(path.c_str(), ren))
        std::cout << "Unable to load Weapon" << '\n';
}
