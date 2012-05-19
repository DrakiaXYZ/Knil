#ifndef GAMEFUNCTIONS_H
#define GAMEFUNCTIONS_H

#include <SDL/SDL.h>
#include <unistd.h>
#include <sys/param.h>
#include <string>
using namespace std;

bool rectCollide(SDL_Rect a, SDL_Rect b);
string RelPath(string path);

#endif // GAMEFUNCTIONS_H
