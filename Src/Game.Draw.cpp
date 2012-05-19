#include "Game.Draw.h"

SDL_Surface *GameDraw::screen = NULL;

SDL_Surface *GameDraw::CreateBlankSurface(int width, int height) {
    if (screen == NULL) return NULL;
    SDL_Surface *surface = NULL;
    Uint32 colorkey;

    surface = SDL_CreateRGBSurface( screen->flags, width, height,
                screen->format->BitsPerPixel,
                screen->format->Rmask,
                screen->format->Gmask,
                screen->format->Bmask,
                screen->format->Amask);

    if (surface != NULL) {
        colorkey = SDL_MapRGB(surface->format, 0xFF, 0x00, 0xFF); // Create a purple color for transperancy
        SDL_FillRect(surface, NULL, colorkey);
        SDL_SetColorKey(surface, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey);
    }

    return surface;
}

SDL_Surface *GameDraw::LoadImage(string filename, Uint32 colorkey) {
    SDL_Surface *loadedImage = NULL;
    SDL_Surface *optimizedImage = NULL;

    loadedImage = IMG_Load(filename.c_str());
    if (loadedImage != NULL) {
        optimizedImage = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);
        if (optimizedImage != NULL)
            SDL_SetColorKey(optimizedImage, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey);
    }
    return optimizedImage;
}

void GameDraw::RenderToSurface(int x, int y, SDL_Surface *source, SDL_Surface *dest, SDL_Rect *clip) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( source, clip, dest, &offset);
}

void GameDraw::SurfaceClear(SDL_Surface *surface, Uint32 colorkey) {
    SDL_FillRect(surface, NULL, colorkey);
    SDL_SetColorKey(surface, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey);
}

void GameDraw::FlipToScreen(SDL_Surface *surface) {
    SDL_Flip(surface);
}

void GameDraw::DrawSquare(SDL_Rect rect, SDL_Surface *surface, Uint32 colorkey) {
    SDL_Rect top; SDL_Rect right; SDL_Rect bottom; SDL_Rect left;
    top.x = rect.x; top.y = rect.y; top.w = rect.w; top.h = 1;
    right.x = rect.x + rect.w - 1; right.y = rect.y; right.w = 1; right.h = rect.h;
    bottom.x = rect.x; bottom.y = rect.y + rect.h - 1; bottom.w = rect.w; bottom.h = 1;
    left.x = rect.x; left.y = rect.y; left.w = 1; left.h = rect.h;

    SDL_FillRect(surface, &top, colorkey);
    SDL_FillRect(surface, &right, colorkey);
    SDL_FillRect(surface, &bottom, colorkey);
    SDL_FillRect(surface, &left, colorkey);
}
