#include "Game.Dialog.h"

GameDialog::GameDialog() {
    Surface = NULL;
    Active = false;
    Page = 0;
    screen = GameDraw::screen;
    enterDown = false;

    gt.Init();
    gt.LoadFont();
}

GameDialog::~GameDialog() {
    if (Surface)
        SDL_FreeSurface(Surface);
}

// Add page to end of list containing text [text]
// Returns the page number.
int GameDialog::addPage(string text) {
    Pages.push_back(text);
    return Pages.size() - 1;
}
// Removes page [i] if i < pageCount. 0-indexed.
void GameDialog::removePage(int p) {
    if ((unsigned)p < Pages.size())
        Pages.erase(Pages.begin() + p);
}
// Remove all pages
void GameDialog::clearPages() {
    Pages.clear();
}
// Return page count
int GameDialog::pageCount() {
    return Pages.size();
}
// Draws page to Surface, activates dialog.
void GameDialog::ShowPage(int p) {
    if ((unsigned)p >= Pages.size()) return;
    Page = p;
    if (Surface)
        SDL_FreeSurface(Surface);
    Surface = GameDraw::LoadImage("Images/Dialog.png", SDL_MapRGB(screen->format, 0xff, 0x00, 0xff));
    // Handle newlines in dialogs.
    string txt = Pages[p];
    char buf[txt.length()];
    buf[0] = '\0';
    int offY = 0; int offbuf = 0;
    for (unsigned int i = 0; i < txt.length(); i++) {
        if (txt[i] != '\n') {
            buf[offbuf] = txt[i];
            offbuf++;
            buf[offbuf] = '\0';
        }
        if (txt[i] == '\n' || i == txt.length() - 1) {
            SDL_Surface* text = gt.DrawText(buf, (SDL_Color){0, 0, 0});
            GameDraw::RenderToSurface(DIALOGPAD, DIALOGPAD + offY, text, Surface, &text->clip_rect);
            SDL_FreeSurface(text);
            offbuf = 0; buf[0] = '\0';
            offY += gt.GetLineHeight();
        }
    }

    SDL_Surface* entertext = gt.DrawText("Press Enter to continue", (SDL_Color){0, 0, 0});
    GameDraw::RenderToSurface(DIALOGMSGOFFSET, DIALOGHEIGHT - DIALOGMSGSPACE, entertext, Surface, &entertext->clip_rect);
    SDL_FreeSurface(entertext);
    entertext = gt.DrawText("Enter", (SDL_Color){0, 0, 0xff});
    GameDraw::RenderToSurface(DIALOGMSGOFFSET + gt.GetStrWidth("Press "), DIALOGHEIGHT - DIALOGMSGSPACE, entertext, Surface, &entertext->clip_rect);
    SDL_FreeSurface(entertext);

    // Draw Page to Surface
    Active = true;
}
// Return whether dialog is active.
bool GameDialog::isActive() {
    return Active;
}

void GameDialog::handleInput(GameInput* gi) {
    if (gi->GetKeyState(SDLK_RETURN) && !enterDown) {
        if ((unsigned)Page < Pages.size())
            ShowPage(++Page);
        enterDown = true;
    }
    if (!gi->GetKeyState(SDLK_RETURN)) enterDown = false;
    if ((unsigned)Page >= Pages.size()) Active = false;
}

void GameDialog::Close() {
    Active = false;
}
