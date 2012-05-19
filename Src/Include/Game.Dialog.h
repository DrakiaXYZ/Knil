#ifndef GAMEDIALOG_H
#define GAMEDIALOG_H

#include "Game.Input.h"
#include "Game.Draw.h"
#include "Game.Text.h"
#include <vector>
#include <string>
using std::string;
using std::vector;

class GameDialog
{
	public:
		GameDialog();
		~GameDialog();

        // Add page to end of list containing text [text]
        // Returns the page number.
		int addPage(string text);
		// Removes page [i] if i < pageCount. 0-indexed.
		void removePage(int page);
		// Remove all pages
		void clearPages();
		// Return page count
		int pageCount();
		// Draws page to Surface, activates dialog. 0-indexed
		void ShowPage(int page);
		// Return whether dialog is active.
		bool isActive();
		// Close the dialog
		void Close();

		void handleInput(GameInput* gi);
		SDL_Surface* getSurface() {return Surface;};


        // Some magic number goodness
        static const int DIALOGWIDTH = 600;
        static const int DIALOGHEIGHT = 200;
        static const int DIALOGMSGSPACE = 30;
        static const int DIALOGMSGOFFSET = 390;
        static const int DIALOGPAD = 20;

	private:
        SDL_Surface* screen;
        SDL_Surface* Surface;
        vector<string> Pages;
        GameDraw gd;
        GameText gt;

        bool Active;
        int Page;
        bool enterDown;


};


#endif // GAMEDIALOG_H

