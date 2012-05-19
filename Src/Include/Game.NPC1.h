#ifndef GAMENPC1_H
#define GAMENPC1_H

#include "Game.NPC.h"


#define DIALOGCOW "\
         )__(\n\
         (oo)\n\
  *-------\\/\n\
 / |     ||\n\
/  ||----||\n\
   vv    vv"


class GameNPC1 : public GameNPC
{
	public:
		GameNPC1();
		~GameNPC1();
		void Collide();
		bool Init(GameDialog* dialog, GameCharacter* player);
		void Step(int dt);

	private:

};


#endif // GAMENPC1_H

