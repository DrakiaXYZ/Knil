#ifndef GAMENPCITEM3_h
#define GAMENPCITEM3_h

#include "Game.NPC.h"

class GameNPCItem3 : public GameNPC
{
	public:
		GameNPCItem3() {
            x = block.x = 320;
            y = block.y = 608;
            block.w = block.h = 40;
		}
		~GameNPCItem3() {
            if (Surface)
                SDL_FreeSurface(Surface);
		}
		void Collide() {
		    if (!player->getQVar(20)) {
                player->setQVar(20, 1);
                dialog->clearPages();
                dialog->addPage("You found GoodGuy's [Item3].\n\nReturn to GoodGuy to see what to do next!");
                dialog->ShowPage(0);
                if (Surface) {
                    SDL_FreeSurface(Surface);
                    Surface = NULL;
                }
                x = block.x = y = block.y = block.w = block.h = 0;
		    }
		}
		void Step(int dt) {
		    if (!Surface && !player->getQVar(20))
                Surface = GameDraw::LoadImage("Images/Apple.png", SDL_MapRGB(screen->format, 0xFF, 0x00, 0xFF));
            if (player->getQVar(20)) {
                x = block.x = y = block.y = block.w = block.h = 0;
            }
		}
};




#endif // GAMENPCITEM3_h

