#ifndef GAMENPCITEM2_h
#define GAMENPCITEM2_h

#include "Game.NPC.h"

class GameNPCItem2 : public GameNPC
{
	public:
		GameNPCItem2() {
            x = block.x = 128;
            y = block.y = 368;
            block.w = block.h = 40;
		}
		~GameNPCItem2() {
            if (Surface)
                SDL_FreeSurface(Surface);
		}
		void Collide() {
		    if (!player->getQVar(10)) {
                player->setQVar(10, 1);
                dialog->clearPages();
                dialog->addPage("You found GoodGuy's [Item2].\n\nReturn to GoodGuy to see what to do next!");
                dialog->ShowPage(0);
                if (Surface) {
                    SDL_FreeSurface(Surface);
                    Surface = NULL;
                }
                x = block.x = y = block.y = block.w = block.h = 0;
		    }
		}
		void Step(int dt) {
		    if (!Surface && !player->getQVar(10))
                Surface = GameDraw::LoadImage("Images/Apple.png", SDL_MapRGB(screen->format, 0xFF, 0x00, 0xFF));
            if (player->getQVar(10)) {
                x = block.x = y = block.y = block.w = block.h = 0;
            }
		}
};




#endif // GAMENPCITEM2_h

