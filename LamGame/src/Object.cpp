#include "Object.h"

Object::Object(){
    tx = ty = Speed = 0;
}

Object::~Object(){
    tx = ty = Speed = 0;
}

int Object::getSpeed() {
    return Speed;
}
int Object::getX() {
    return tx;
}
int Object::getY() {
    return ty;
}
void Object::setSpeed(int val) {
    Speed = val;
}
void Object::setX(int val) {
    tx = val;
}
void Object::setY(int val) {
    ty = val;
}
