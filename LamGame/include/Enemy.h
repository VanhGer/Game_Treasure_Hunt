#ifndef ENEMY_H
#define ENEMY_H

#include "LoadObject.h"
#include "Character.h"

class Enemy: public Character
{
    public:
        Enemy();
        ~Enemy();
        void clean();
        void loadCharacter(SDL_Renderer *ren, std::string path);
        Uint64 getSpawnTime();
        void setSpawnTime(Uint64 val);
        void setSpawn(bool val);
        bool GetSpawn();
        void setTypes(int val);
        int GetTypes();
    protected:
        Uint64 SpawnTime;
        bool Spawn;
        int Types;

};

#endif // Enemy_H
