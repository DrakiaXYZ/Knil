#ifndef GAMEPLAYER_H
#define GAMEPLAYER_H

#include "Game.Character.h"
#include "Game.Map.h"
#include "Game.Input.h"



class GamePlayer : public GameCharacter
{
	public:
		GamePlayer();
		//~GamePlayer();

		void handleInput(GameInput *gi, GameMap *gm);
		virtual void tick(dword dt);

	private:
        void updateFrame();

        GameAnimation gaHead;
        GameAnimation gaBody;

};

#endif // GAMEPLAYER_H
