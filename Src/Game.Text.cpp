#include "Game.Text.h"

GameText::GameText() {
    font = NULL;
    fontfile = "";
    fontsize = 0;
    loaded = false;
    screen = NULL;
}

GameText::~GameText() {
    if (font)
        TTF_CloseFont(font);
    TTF_Quit();
}

void GameText::Init() {
    screen = GameDraw::screen;
}

bool GameText::LoadFont(string f, int s) {
    if (!screen) return false;
    if (loaded) {
        loaded = false;
        if (font)
            TTF_CloseFont(font);
    }
    fontfile = f;
    fontsize = s;
    if (TTF_Init() == -1) {
        printf("Error loading TTF_Init\n");
        return false;
    }
    font = TTF_OpenFont(fontfile.c_str(), fontsize);
    if (!font) {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        TTF_Quit();
        return false;
    }
    loaded = true;
    return true;
}

SDL_Surface* GameText::DrawText(string text, SDL_Color color) {
    if (!loaded) return NULL;
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    return surface;
}

int GameText::GetLineHeight() {
    if (!loaded) return 0;
    return TTF_FontLineSkip(font);
}

int GameText::GetCharWidth() {
    if (!TTF_FontFaceIsFixedWidth(font)) return -1;
    int width = 0;
    TTF_GlyphMetrics(font, 'a', NULL, NULL, NULL, NULL, &width);
    return width;
}

int GameText::GetStrWidth(const char* str) {
    int w = 0;
    TTF_SizeText(font, str, &w, NULL);
    return w;
}
