#ifndef GAMEANIMATION_H
#define GAMEANIMATION_H

#include <SDL/SDL.h>
#include "Game.Draw.h"
#include "Game.Types.h"
#include <iostream>

using namespace std;

class GameAnimation
{
	public:
		GameAnimation();
		~GameAnimation();

		bool loadKCA(string file);
		SDL_Surface *getSurface() {return aniSurface;};
		bool tick(dword dt);
		int getWidth();
		int getHeight();
		void setAnim(int anim);
		int getFrame() {return frame;};
	protected:

	private:
        string filename;

        SDL_Surface *aniSurface;
        SDL_Surface *aniImage;
        SDL_Surface *screen;

        dword curFrameTime;
        word anim;
        word frame;
        KCAFile kca;
        GameDraw gd;

        bool loaded;

        void updateFrame();
        void freeKCA();

};

#endif // GAMEANIMATION_H
