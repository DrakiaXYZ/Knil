#ifndef GAMENPCITEM1_h
#define GAMENPCITEM1_h

#include "Game.NPC.h"

class GameNPCItem1 : public GameNPC
{
	public:
		GameNPCItem1() {
            x = block.x = 304;
            y = block.y = 256;
            block.w = block.h = 40;
		}
		~GameNPCItem1() {
            if (Surface)
                SDL_FreeSurface(Surface);
		}
		void Collide() {
		    if (!player->getQVar(0)) {
                player->setQVar(0, 1);
                dialog->clearPages();
                dialog->addPage("You found GoodGuy's [Item].\n\nReturn to GoodGuy to see what to do next!");
                dialog->ShowPage(0);
                if (Surface) {
                    SDL_FreeSurface(Surface);
                    Surface = NULL;
                }
                x = block.x = y = block.y = block.w = block.h = 0;
		    }
		}
		void Step(int dt) {
		    if (!Surface && !player->getQVar(0))
                Surface = GameDraw::LoadImage("Images/Apple.png", SDL_MapRGB(screen->format, 0xFF, 0x00, 0xFF));
            if (player->getQVar(0)) {
                x = block.x = y = block.y = block.w = block.h = 0;
            }
		}
};




#endif // GAMENPCITEM1_h
