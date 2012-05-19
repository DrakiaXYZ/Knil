#ifndef GAMECHARACTER_H
#define GAMECHARACTER_H

#include <SDL/SDL.h>
#include "Game.Draw.h"
#include "Game.Animation.h"
#include "Game.Types.h"
#include "Game.Audio.h"
#include <iostream>
#include <time.h>
using namespace std;

class GameCharacter
{
	public:
		GameCharacter();
        virtual ~GameCharacter(){};

		SDL_Surface* getSurface() {return charSurface;};

		void setName(string newName);
		virtual void tick(dword dt);
		virtual bool loadKCA(string file);

        void setDir(int d) {dir = d;};
        void setSpeed(int s) {speed = s;};
        void setPos(int nX, int nY) {x = nX; y = nY;};
        void updatePos(int dX, int dY) {x += dX; y += dY;};

        int getDir() {return dir;};
        int getSpeed() {return speed;};
        int getX() {return x;};
        int getY() {return y;};
        void setAnim(int anim);
		void setQVar(int var, byte val);
		byte getQVar(int var);


	protected:

        bool loaded;

        int dir;
        string name;

        // Pixels per second
        int speed;

        int x, y;
        byte qVars[100];

        GameDraw gd;
        GameAudio audio;
        SDL_Surface *screen;
        SDL_Surface *charSurface;
        GameAnimation ga;

        virtual void updateFrame();



};


#endif // GAMEMAP_H


