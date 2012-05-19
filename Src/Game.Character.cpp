#include "Game.Character.h"

GameCharacter::GameCharacter() {
    name = "Drake";
    loaded = false;
    screen = GameDraw::screen;
    charSurface = NULL;
    x = y = dir = 0;
    speed = 5;
}
/*
GameCharacter::~GameCharacter() {

}*/

bool GameCharacter::loadKCA(string file) {
    if (ga.loadKCA(file)) {
        loaded = true;
        updateFrame();
        return true;
    }
    return false;
}

void GameCharacter::setAnim(int a) {
    ga.setAnim(a);
}

void GameCharacter::tick(dword dt) {
    if (!loaded) return;
    if (ga.tick(dt)) {
        updateFrame();
    }
}

void GameCharacter::updateFrame() {
    if (!loaded) return;
    if (charSurface)
        SDL_FreeSurface(charSurface);
    charSurface = GameDraw::CreateBlankSurface(ga.getWidth(), ga.getHeight());
    GameDraw::RenderToSurface(0, 0, ga.getSurface(), charSurface, &charSurface->clip_rect);
}

void GameCharacter::setName(string newName) {
    name = newName;
}

void GameCharacter::setQVar(int var, byte val) {
    qVars[var] = val;
}

byte GameCharacter::getQVar(int var) {
    return qVars[var];
}
