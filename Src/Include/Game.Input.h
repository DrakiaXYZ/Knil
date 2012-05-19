#ifndef GAMEINPUT_H
#define GAMEINPUT_H

#include <SDL/SDL.h>

class GameInput
{
	public:
		GameInput();
		~GameInput();
		void Update();
		bool GetKeyState(SDLKey key);
		bool GetQuit();
		bool GetMouseDown(int button);
		int GetMouseX() {return mouseX;};
		int GetMouseY() {return mouseY;};

	private:
        SDL_Event event;
        bool quit;
        Uint8 *keyState;
        Uint8 mouseState;
        int mouseX;
        int mouseY;

};


#endif // GAMEINPUT_H

