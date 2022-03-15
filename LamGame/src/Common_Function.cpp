#include "Common_Function.h"
#include "Object.h"

void logSDLError(std::ostream& os, const std::string &msg, bool fatal){
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}
int getDistance(int x, int y, int u, int v) {
   return (x - u) * (x - u) + (y - v) * (y - v);
}
bool Intersect(int x, int y, int h, int w, int x2, int y2, int h2, int w2){
    if (x <= x2 && x2 <= x + w){
        if (y <= y2 && y2 <= (y + h)) return 1;
        if (y <= (y2 + h2) && (y2 + h2) <= (y + h)) return 1;
    }
    if (x <= (x2 + w2) && (x2 + w2) <= (x + w)){
        if (y <= y2 && y2 <= (y + h)) return 1;
        if (y <= (y2 + h2) && (y2 + h2) <= (y + h)) return 1;
    }
    return 0;
}
bool LoadBackGround(int id, string path, SDL_Renderer *ren){
    if (! BackGround[id].loadFromFile(path.c_str(), ren)){
        logSDLError(cout, "Unable to load sheet texture", 1);
        return false;
    }
    return true;
}
long long Rand(long long l, long long r)
{
    return uniform_int_distribution<long long> (l, r) (rng);
}
