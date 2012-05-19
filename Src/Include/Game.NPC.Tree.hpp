#ifndef GAMENPCTREE_h
#define GAMENPCTREE_h

#include "Game.NPC.h"

class GameNPCTree : public GameNPC
{
	public:
		GameNPCTree() {
            block.w = block.h = 0;
            layer = 1;
		}
		~GameNPCTree() {
            if (Surface)
                SDL_FreeSurface(Surface);
		}
		void Step(int dt) {
		    if (!Surface)
                Surface = GameDraw::LoadImage("Images/Tree.png", SDL_MapRGB(screen->format, 0xFF, 0x00, 0xFF));
		}
};




#endif // GAMENPCTREE_h

