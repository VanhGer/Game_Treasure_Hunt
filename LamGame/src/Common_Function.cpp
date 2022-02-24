#include "Common_Function.h"


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
