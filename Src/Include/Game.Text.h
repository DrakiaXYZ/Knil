#ifndef GAMETEXT_H
#define GAMETEXT_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "Game.Draw.h"
#include <string>

class GameText
{
	public:
		GameText();
		~GameText();
		void Init();
		bool LoadFont(string fontfile = "C:/windows/fonts/cour.ttf", int size = 14);
		SDL_Surface* DrawText(string text, SDL_Color color);
		int GetLineHeight();
		int GetCharWidth();
		int GetStrWidth(const char* str);

	private:

        bool loaded;
        TTF_Font* font;
        string fontfile;
        int fontsize;

        GameDraw gd;
        SDL_Surface* screen;

};

#endif // GAMETEXT_H
