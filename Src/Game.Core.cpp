#include "Game.Core.h"


GameCore::GameCore() {
    quit = false;
    lastFrame = 0;
    cFPS = 0;
    FPS = 0;
    mapOffX = mapOffY = 0;
}

GameCore::~GameCore() {
    ga.Destroy();
    SDL_Quit();
}

/** \brief Initialize the game core.
 *
 *  Initialize SDL, load required files, start the main game loop.
 */
int GameCore::Init(int w, int h) {
    bool Escape = false;
    width = w; height = h;
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) return 1;

    // TODO: This might need to be changed to SDL_SWSURFACE for speed.
    screen = SDL_SetVideoMode(w, h, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (screen == NULL) return 1;

    SDL_WM_SetCaption("The Adventures Of Knil", NULL);

    GameDraw::screen = screen;

    ga.Init();
    ga.PlayMusic("Sounds/Tank Battle.mp3");

    GamePlayer gp;
    gp.loadKCA("CharAnis\\modernbody.kca");
    gp.setPos(368, 528);
    GameDialog dialog;
    // Test load a map
    gm.Init(&gp, &dialog);
    if (!gm.LoadMap("Maps\\HouseMap.map")) {
        return ErrorScreen();
    }
    gm.drawMap();

    while (!quit) {
        // Update GameInput. Check if we should quit.
        gi.Update();
        if (gi.GetKeyState(SDLK_ESCAPE)) {
            if (!Escape && dialog.isActive()) {
                Escape = true;
                dialog.Close();
            }
        } else {
            Escape = false;
        }
        if (gi.GetQuit() || (gi.GetKeyState(SDLK_ESCAPE) && !Escape)) {
            quit = true;
            break;
        }

        if (!dialog.isActive() && gi.GetKeyState(SDLK_F1)) {
            dialog.clearPages();
            dialog.addPage(HELPTEXT1);
            dialog.addPage(HELPTEXT2);
            dialog.addPage(HELPTEXT3);
            dialog.addPage(HELPTEXT4);
            dialog.ShowPage(0);
        }

        // Calculate FPS and frame time
        int dt = CalcFPS();

        gp.tick(dt);
        if (!dialog.isActive())
            gp.handleInput(&gi, &gm);
        gm.handleInput(&gi);
        gm.runScripts(dt);
        dialog.handleInput(&gi);

        // Move screen with player
        if (gp.getX() > (width / 2)) {
            mapOffX = gp.getX() - (width / 2);
            if ((mapOffX + width) > (gm.GetWidth() * 16) ) {
                mapOffX = gm.GetWidth() * 16 - width;
            }
        } else
            mapOffX = 0;
        if (gp.getY() > (height / 2)) {
            mapOffY = gp.getY() - (height / 2);
            if ((mapOffY + height) > (gm.GetHeight() * 16) ) {
                mapOffY = gm.GetHeight() * 16 - height;
            }
        } else
            mapOffY = 0;

        SDL_Rect clip;
        clip.x = mapOffX;
        clip.y = mapOffY;
        clip.w = width;
        clip.h = height;

        GameDraw::SurfaceClear(screen, SDL_MapRGB(screen->format, 0, 0, 0));

        GameDraw::RenderToSurface(0, 0, gm.GetMapBottom(), screen, &clip);
        // Render NPCs on Layer 0
        vector<GameNPC*>* mapNPC = gm.getNPCList();
        for (unsigned int i = 0; i < mapNPC->size(); i++) {
            if (mapNPC->at(i)->GetLayer()) continue;
            if (!mapNPC->at(i)->GetSurface()) continue;
            SDL_Rect r = mapNPC->at(i)->GetBlock();
            r.x -= mapOffX; r.y -= mapOffY;
            int npcx = mapNPC->at(i)->GetX() - mapOffX;
            int npcy = mapNPC->at(i)->GetY() - mapOffY;
            GameDraw::RenderToSurface(npcx, npcy, mapNPC->at(i)->GetSurface(), screen, &mapNPC->at(i)->GetSurface()->clip_rect);
        }

        // Render Player
        GameDraw::RenderToSurface(gp.getX() - mapOffX, gp.getY() - mapOffY, gp.getSurface(), screen, &gp.getSurface()->clip_rect);

        // Render NPCs on Layer != 0
        for (unsigned int i = 0; i < mapNPC->size(); i++) {
            if (!mapNPC->at(i)->GetLayer()) continue;
            if (!mapNPC->at(i)->GetSurface()) continue;
            SDL_Rect r = mapNPC->at(i)->GetBlock();
            r.x -= mapOffX; r.y -= mapOffY;
            int npcx = mapNPC->at(i)->GetX() - mapOffX;
            int npcy = mapNPC->at(i)->GetY() - mapOffY;
            GameDraw::RenderToSurface(npcx, npcy, mapNPC->at(i)->GetSurface(), screen, &mapNPC->at(i)->GetSurface()->clip_rect);
        }

        // If there is an active dialog, draw it in the middle of the screen, at the bottom.
        if (dialog.isActive())
            GameDraw::RenderToSurface(width / 2 - GameDialog::DIALOGWIDTH / 2, height - GameDialog::DIALOGHEIGHT - 30, dialog.getSurface(), screen, &dialog.getSurface()->clip_rect);
        GameDraw::FlipToScreen(screen);
    }

    return 0;
}

int GameCore::CalcFPS() {
    int frameStart = SDL_GetTicks();
    int timeDif = frameStart - lastFrame;
    lastFrame = frameStart;

    if (cFPS > 1000) {
        char buf[1024]; // Buffer should always be large enough.
        sprintf(buf, "The Adventures of Knil - FPS: %d", FPS);
        FPS = 0;
        cFPS = 0;
        SDL_WM_SetCaption(buf, NULL);
    } else {
        FPS++;
        cFPS += timeDif;
    }
    return timeDif;
}

bool GameCore::ErrorScreen() {
    if (!screen) return false;
    bool alive = true;
    SDL_Event event;
    SDL_Surface* error = GameDraw::LoadImage("Images/Error.png", SDL_MapRGB(screen->format, 0xFF, 0x00, 0xFF));
    while (alive) {
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        GameDraw::RenderToSurface(width / 2 - error->w / 2, height / 2 - error->h / 2, error, screen, &error->clip_rect);
        GameDraw::FlipToScreen(screen);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                alive = false;
            if (event.type == SDL_KEYDOWN)
                alive = false;
        }
    }
    return false;
}
