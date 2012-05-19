#ifndef GAMENPC_H
#define GAMENPC_H

#include <SDL/SDL.h>
#include "Game.Types.h"
#include "Game.Functions.h"
#include "Game.Dialog.h"
#include "Game.Draw.h"
#include "Game.Character.h"


class GameNPC
{
	public:
		GameNPC();
		virtual ~GameNPC();

		virtual bool Init(GameDialog* dialog, GameCharacter* player);
		virtual void Step(int dt);
		virtual void Collide(); // Player touched the NPC
		virtual void Remove();
		virtual SDL_Rect GetBlock() {return block;};
		virtual SDL_Surface* GetSurface() {return Surface;};

		int GetX() {return x;};
		int GetY() {return y;};
		int GetLayer() {return layer;};

		void SetX(int nX) {x = nX;};
		void SetY(int nY) {y = nY;};
		void SetLayer(int nL) {layer = nL;};


	protected:
        int x, y;
        int w, h;
        int layer;
        SDL_Rect block;

        SDL_Surface* screen;
        SDL_Surface* Surface;

        GameDraw gd;
        GameDialog* dialog;
        GameCharacter* player;

};


#endif // GAMENPC_H

