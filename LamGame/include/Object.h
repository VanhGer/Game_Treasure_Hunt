#ifndef OBJECT_H
#define OBJECT_H


class Object
{
    public:
        Object();
        ~Object();
        int getSpeed();
        int getX();
        int getY();
        void setSpeed(int val);
        void setX(int val);
        void setY(int val);
    protected:
        int Speed, tx, ty;
};

#endif // OBJECT_H
