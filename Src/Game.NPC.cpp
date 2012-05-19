#include "Game.NPC.h"

GameNPC::GameNPC() {
    x = block.x = 0;
    y = block.y = 0;
    w = block.w = 16;
    h = block.h = 16;
    layer = 0;
    screen = GameDraw::screen;
    dialog = NULL;
    player = NULL;
    Surface = NULL;
}

GameNPC::~GameNPC() {}

bool GameNPC::Init(GameDialog* d, GameCharacter* p) {
    dialog = d;
    player = p;
    return true;
}

void GameNPC::Step(int dt) {}

void GameNPC::Collide() {}

void GameNPC::Remove() {}

