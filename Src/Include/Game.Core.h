#ifndef GAMECORE_H
#define GAMECORE_H

//#define DEBUG

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <iostream>
#include <time.h>
#include "Game.Draw.h"
#include "Game.Input.h"
#include "Game.Map.h"
#include "Game.Character.h"
#include "Game.Player.h"
#include "Game.Dialog.h"
#include "Game.Help.h"
#include "Game.Audio.h"
using namespace std;

/** \brief The core game class.
 *
 *  This class handles running the entire game. It will take control of all the
 *  other Game classes. Init() should be called, and will return when the game
 *  is complete.
 */

class GameCore {
    public:
        GameCore();
        ~GameCore();

        int Init(int width, int height);
    private:
        void Clean();
        bool RenderFunc();
        int CalcFPS();
        bool ErrorScreen();

        SDL_Surface *screen;
        GameDraw gd;
        GameInput gi;
        GameMap gm;
        GameAudio ga;
        bool quit;

        // Window variables
        int width;
        int height;
        int lastFrame;
        int cFPS;
        int FPS;

        int mapOffX, mapOffY;

};


#endif
