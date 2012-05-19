#include "Game.Player.h"

GamePlayer::GamePlayer() {
    memset(qVars, 0, sizeof(qVars));
}
/*
GamePlayer::~GamePlayer() {

}*/

void GamePlayer::handleInput(GameInput *gi, GameMap *gm) {
    int cX = 0, cY = 0;
    if (gi->GetKeyState(SDLK_UP)) {
        cY -= 1;
        dir = 0;
    }
    if (gi->GetKeyState(SDLK_RIGHT)) {
        cX += 1;
        dir = 3;
    }
    if (gi->GetKeyState(SDLK_DOWN)) {
        cY += 1;
        dir = 2;
    }
    if (gi->GetKeyState(SDLK_LEFT)) {
        cX -= 1;
        dir = 1;
    }

    if (gi->GetKeyState(SDLK_SPACE)) {
        ga.setAnim(8 + dir);
        updateFrame();
        cX = cY = 0;
    } else if (gi->GetKeyState(SDLK_a)) {
        ga.setAnim(12 + dir);
        updateFrame();
        cX = cY = 0;
    } else if (cX == 0 && cY == 0) {
        ga.setAnim(dir);
        updateFrame();
    } else {
        ga.setAnim(4+dir);
        updateFrame();
    }

    SDL_Rect rect;
    rect.x = x + (charSurface->clip_rect.w / 2) - 16;
    rect.y = y + (charSurface->clip_rect.h) - 32;
    rect.w = 32;
    rect.h = 32;

    rect.x += cX;
    if (!gm->isBlocking(rect))
        x += cX;
    else
        rect.x -= cX;

    rect.y += cY;
    if (!gm->isBlocking(rect))
        y += cY;

    SDL_Rect mapWarp = gm->isMapChange(rect);
    if (mapWarp.x >= 0)
        x = mapWarp.x;
    if (mapWarp.y >= 0)
        y = mapWarp.y;
}

void GamePlayer::tick(dword dt) {
    if (!loaded) return;
    if (ga.tick(dt)) {
        updateFrame();
    }
}

void GamePlayer::updateFrame() {
    if (!loaded) return;
    if (charSurface)
        SDL_FreeSurface(charSurface);
    charSurface = GameDraw::CreateBlankSurface(ga.getWidth(), ga.getHeight());
    GameDraw::RenderToSurface(0, 0, ga.getSurface(), charSurface, &charSurface->clip_rect);
}
