#include "Character.h"
Character::Character()
{
    Object();
    for (int i = 0; i < 4; i++){
        Rdir[i].h = 60; Rdir[i].w = 40;
        Rdir[i].x = 0; Rdir[i].y = 0;
        NumFrames[i] = 0;
    }
    id = 0;
}
Character::~Character()
{
    for (int i = 0; i < 4; i++){
        Rdir[i].h = 0; Rdir[i].w = 0;
        Rdir[i].x = 0; Rdir[i].y = 0;
        NumFrames[i] = 0;
        Dir[i].free();
    }
    id = 0;
}
void Character:: setId(int val){
    id = val;
}
int Character:: getId(){
    return id;
}
void Character:: setHp(int val){
    Hp = val;
}
int Character:: getHp(){
    return Hp;
}
int Character:: getNum(){
    return NumFrames[id];
}
void Character::setNumFrames(int val){
    NumFrames[id] = val;
}
void Character:: changeNumFrames(int Num){
    NumFrames[id]++;
    NumFrames[id] %= Num;
}
void Character:: goUp(int Lim, int Num){
    id = 0;
    changeNumFrames(Num);
    ty = std::max(Lim, ty - Speed);
}
void Character:: goDown(int Lim, int Num){
    id = 1;
    changeNumFrames(Num);
    ty = std::min(Lim, ty +  Speed);
}
void Character:: goLeft(int Lim, int Num){
    id = 2;
    changeNumFrames(Num);
    tx = std::max(Lim, tx -  Speed);
}
void Character:: goRight(int Lim, int Num){
    id = 3;
    changeNumFrames(Num);
    tx = std::min(Lim, tx +  Speed);
}

void Character:: display(SDL_Renderer *ren){
    Rdir[id].x = 64*NumFrames[id];
    Dir[id].render(tx, ty, ren, &Rdir[id]);
}
