#include "Game.Functions.h"

bool rectCollide(SDL_Rect a, SDL_Rect b) {
    // Either rect has a 0 side
    if (a.w == 0 || a.h == 0 || b.w == 0 || b.h == 0)
        return false;
    // a is left of b
    if ( ( (a.x + a.w) <= b.x ) ||
    // a is right of b
         ( a.x >= (b.x + b.w) ) ||
    // a is above b
         ( (a.y + a.h) <= b.y ) ||
    // a is below b
         ( a.y >= (b.y + b.h) ) )
         return false;
    return true;
};

string RelPath(string path) {
    if (path.length() == 0) return "";
    char cur[MAXPATHLEN];
    getcwd(cur, MAXPATHLEN);
    int len = (path.length() > strlen(cur)) ? strlen(cur) : path.length();
    if (strncmp(path.c_str(), cur, len)) {
        return path;
    }
    return path.substr(strlen(cur) + 1);
}
