#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <SDL/SDL.h>
#include "Game.Dialog.h"
#include "Game.Draw.h"
#include "Game.Input.h"
#include "Game.Types.h"
#include "Game.Functions.h"
// NPCs
#include "Game.NPC.h"
#include "Game.NPC1.h"
#include "Game.NPC.Item1.hpp"
#include "Game.NPC.Item2.hpp"
#include "Game.NPC.Item3.hpp"
#include "Game.NPC.Tree.hpp"
// END NPCs
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

class GamePlayer;

class GameMap
{
	public:
		GameMap();
		~GameMap();

		int GetWidth() {return mapWidth;};
		int GetHeight() {return mapHeight;};

        bool LoadMap(string filename);
        bool SaveMap(string filename);
        void Init(GamePlayer* player, GameDialog* dialog);
        void setMapEdit(bool e) {mapEdit = e;};
        void handleInput(GameInput *gi);
        void runScripts(int dt);
        void drawMap();
        vector<GameNPC*>* getNPCList() {return &mapNPC;};


        SDL_Surface *GetMapBottom() {return mapSurface;};

        bool isBlocking(SDL_Rect rect);
        SDL_Rect isMapChange(SDL_Rect rect);

        // Map Editing Functions.
        void setTile(int x, int y, int tilex, int tiley);
        Tile getTile(int x, int y);
        void SetTileset(string tileset);
        void paintTile(int x, int y, SDL_Rect tile);
        void setDefTile(Tile t) {defTile = t;};
        void clearTiles(SDL_Rect r);
        void addPH(placeholder p);
        void delPH(SDL_Rect r);
        placeholder* getPH(SDL_Rect r);
        void addBlock(SDL_Rect r);
        void delBlock(SDL_Rect r);
        void addLink(Link link);
        Link* getLink(SDL_Rect r);
        void delLink(SDL_Rect r);
        void clearBlock(SDL_Rect r);
        SDL_Surface* getTilesetSurface() {return tilesetSurface;};
        int getTilesetWidth() {return (tilesetSurface) ? tilesetSurface->clip_rect.w : 0;};
        int getTilesetHeight() {return (tilesetSurface) ? tilesetSurface->clip_rect.h : 0;};



	private:
        bool mapLoaded;
        bool mapEdit;

        // Map info
        char* mapFile;
        char mapHeader[3];
        byte version;
        char* tileset;
        byte mapWidth, mapHeight;
        Tile defTile;
        vector< vector< Tile > > mapTiles;

        byte blockCount;
        vector< SDL_Rect > mapBlock;

        byte linkCount;
        vector< Link > mapLink;

        byte npcCount;
        vector< GameNPC* > mapNPC;

        vector< placeholder > PH;

        SDL_Surface *mapSurface;
        SDL_Surface *tilesetSurface;
        SDL_Surface *screen;

        GameDraw gd;
        GamePlayer* gp;
        GameDialog* dialog;

        void LoadTileSet(string filename);


};


#endif // GAMEMAP_H

