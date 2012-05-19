#ifndef GAMEDRAW_H
#define GAMEDRAW_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>
using namespace std;


class GameDraw
{
	public:
        static SDL_Surface *CreateBlankSurface(int width, int height);
        static SDL_Surface *LoadImage(string filename, Uint32 transparency);
        static void RenderToSurface(int x, int y, SDL_Surface *source, SDL_Surface *dest, SDL_Rect* clip);
        static void SurfaceClear(SDL_Surface *surface, Uint32 colorkey);
        static void FlipToScreen(SDL_Surface *surface);
        static void DrawSquare(SDL_Rect rect, SDL_Surface *surface, Uint32 colorkey);
        static void DrawCircle(int radius, SDL_Surface *surface, Uint32 colorkey);

        static SDL_Surface* screen;

};

#endif // GAMEDRAW_H

