#include "Game.Map.h"

GameMap::GameMap() {
    mapLoaded = false;
    tileset = NULL;
    mapFile = NULL;
    mapSurface = NULL;
    tilesetSurface = NULL;
    mapWidth = mapHeight = 0;
    screen = NULL;
    mapEdit = false;
    memset(&defTile, 0, sizeof(defTile));
    npcCount = linkCount = blockCount = 0;

    gp = NULL;
}

GameMap::~GameMap() {
    if (mapSurface)
        SDL_FreeSurface(mapSurface);
    if (tilesetSurface)
        SDL_FreeSurface(tilesetSurface);
    if (tileset)
        delete [] tileset;
    if (mapFile)
        delete [] mapFile;
    for (unsigned i = 0; i < mapNPC.size(); i++)
        delete mapNPC.at(i);
    for (unsigned i = 0; i < PH.size(); i++) {
        if (PH[i].surface)
            SDL_FreeSurface(PH[i].surface);
        delete [] PH[i].img;
    }
    PH.clear();
    mapNPC.clear();
    mapSurface = NULL;
    tilesetSurface = NULL;
    tileset = NULL;
}

void GameMap::Init(GamePlayer* player, GameDialog* d) {
    screen = GameDraw::screen;
    gp = player;
    dialog = d;
}

bool GameMap::LoadMap(string filename) {
    if (mapLoaded) {
        mapLoaded = false;
        // Free all the things used for THIS map to load a new map
        if (mapSurface)
            SDL_FreeSurface(mapSurface);
        if (tilesetSurface)
            SDL_FreeSurface(tilesetSurface);
        if (tileset)
            delete [] tileset;
        if (mapFile)
            delete [] mapFile;
        for (unsigned i = 0; i < mapNPC.size(); i++)
            delete mapNPC.at(i);
        for (unsigned i = 0; i < PH.size(); i++) {
            if (PH[i].surface)
                SDL_FreeSurface(PH[i].surface);
            delete [] PH[i].img;
        }
        mapSurface = NULL;
        tilesetSurface = NULL;
        tileset = NULL;
        mapFile = NULL;

        for (unsigned int i = 0; i < mapTiles.size(); i++)
            mapTiles.at(i).clear();
        PH.clear();
        mapTiles.clear();
        mapBlock.clear();
        blockCount = 0;
        mapLink.clear();
        linkCount = 0;
        mapNPC.clear();
        npcCount = 0;
        mapWidth = 0;
        mapHeight = 0;
        version = 0;
    }
    FILE* mapHandle = fopen(filename.c_str(), "rb");
    if (mapHandle == NULL) {
        printf("Can't open map file: %s", filename.c_str());
        return false;
    }
    mapFile = new char[filename.length() + 1];
    strcpy(mapFile, filename.c_str());
    word tstrlen;
    char cTmp[2];

    fread(mapHeader, 1, 3, mapHandle);
    if (strncmp(mapHeader, "DRM", 3)) {
        printf("This map wasn't designed for me. Header: %s\n", mapHeader);
        return false;
    }

    fread(&version, 1, 1, mapHandle);
    if (version != 6) {
        printf("Wrong map version. Version: %d\n", version);
        return false;
    }

    fread(&tstrlen, 2, 1, mapHandle);
    tileset = new char[tstrlen];
    fread(tileset, 1, tstrlen, mapHandle);

    mapTiles.clear();

    fread(&mapWidth, 1, 1, mapHandle);
    fread(&mapHeight, 1, 1, mapHandle);
    fread(&defTile.tX, 1, 1, mapHandle);
    fread(&defTile.tY, 1, 1, mapHandle);

    //printf("Map loaded: %s  Width: %d  Height: %d\n", filename.c_str(), mapWidth, mapHeight);
    int x = 0, y = 0;

    vector<Tile> tileRow;
    while ((y * mapWidth) + x < mapWidth * mapHeight) {
        fread(cTmp, 1, 2, mapHandle);
        Tile _tmpTile;

        _tmpTile.tX = (int)cTmp[0];
        _tmpTile.tY = (int)cTmp[1];
        _tmpTile.block = false;
        //_tmpTile.block = (int)cTmp[2];
        /*if (_tmpTile.block) {
            SDL_Rect rect;
            rect.x = x*16;
            rect.y = y*16;
            rect.w = rect.h = 16;
            mapBlock.push_back(rect);
        }*/

        _tmpTile.mX = x;
        _tmpTile.mY = y;
        tileRow.push_back(_tmpTile);
        x++;
        if (x >= mapWidth) {
            y++; x = 0;
            mapTiles.push_back(tileRow);
            tileRow.clear();
        }
    }

    char Sep[1];
    fread(Sep, 1, 1, mapHandle);
    if (*Sep != ':') printf("Error. Expected ':' Actual 0x%X\n", *Sep);

    //fread(&npcCount, 1, 1, mapHandle);
    int phcount = 0;
    fread(&phcount, 1, 1, mapHandle);
    for (int i = 0; i < phcount; i++) {
        // NPC Loading.
        placeholder ph;
        fread(&ph.r, sizeof(SDL_Rect), 1, mapHandle);
        int len = 0;
        fread(&len, 1, 1, mapHandle);
        ph.img = new char[len + 1];
        ph.img[len] = '\0';
        fread(ph.img, 1, len, mapHandle);
        if (mapEdit) {
            ph.surface = GameDraw::LoadImage(ph.img, SDL_MapRGB(screen->format, 0xFF, 0x00, 0xFF));
            ph.r.w = ph.surface->w;
            ph.r.h = ph.surface->h;
        } else {
            ph.surface = NULL;
        }
        PH.push_back(ph);
    }

    fread(Sep, 1, 1, mapHandle);
    if (*Sep != ':') printf("Error. Expected ':' Actual 0x%d\n", *Sep);

    fread(&linkCount, 1, 1, mapHandle);
    for (int i = 0; i < linkCount; i++) {
        // Link Loading
        signed char t;
        Link l;
        fread(&t, 1, 1, mapHandle);
        l.r.x = t * 16;
        fread(&t, 1, 1, mapHandle);
        l.r.y = t * 16;
        fread(&t, 1, 1, mapHandle);
        l.r.w = t * 16;
        fread(&t, 1, 1, mapHandle);
        l.r.h = t * 16;

        int mapLen = 0;
        fread(&mapLen, 2, 1, mapHandle);
        //printf("Maplen: %d\n", mapLen);
        char destMap[mapLen];
        memset(destMap, '\0', sizeof(destMap));
        fread(destMap, 1, mapLen, mapHandle);
        l.destination = string(destMap);

        fread(&t, 1, 1, mapHandle);
        l.destx = t * 16;
        fread(&t, 1, 1, mapHandle);
        l.desty = t * 16;

        mapLink.push_back(l);

    }

    fread(Sep, 1, 1, mapHandle);
    if (*Sep != ':') printf("Error. Expected ':' Actual 0x%d\n", *Sep);

    fread(&blockCount, 1, 1, mapHandle);
    for (int i = 0; i < blockCount; i++) {
        // Block List Loading.
        signed char t;
        SDL_Rect r;
        fread(&t, 1, 1, mapHandle);
        r.x = t * 16;
        fread(&t, 1, 1, mapHandle);
        r.y = t * 16;
        fread(&t, 1, 1, mapHandle);
        r.w = t * 16;
        fread(&t, 1, 1, mapHandle);
        r.h = t * 16;
        mapBlock.push_back(r);
    }

    fclose(mapHandle);
    #ifdef MAPEDITOR
    monkeuy
    #endif
    #ifndef MAPEDITOR
    if (!mapEdit) {
        /*** Hard coded NPCs based on map ***/
        if (!strcmp(mapFile, "Maps\\HouseMap.map")) {
            GameNPC* NPC1 = new GameNPC1();
            NPC1->Init(dialog, (GameCharacter*)gp);
            mapNPC.push_back(NPC1);
        } else if (!strcmp(mapFile, "Maps\\InHouse.map")) {
            GameNPC* NPCItem1 = new GameNPCItem1();
            NPCItem1->Init(dialog, (GameCharacter*)gp);
            mapNPC.push_back(NPCItem1);
        } else if (!strcmp(mapFile, "Maps\\Basement.map")) {
            GameNPC* NPCItem2 = new GameNPCItem2();
            NPCItem2->Init(dialog, (GameCharacter*)gp);
            mapNPC.push_back(NPCItem2);
        } else if (!strcmp(mapFile, "Maps\\Forest.map")) {
            int xs[23] = {64, 256, 400, 496, 592,
                          688, 112, 336, 448, 176,
                          272, 544, 176, 352, 464,
                          688, 80, 256, 544, 672,
                          32, 80, 416};
            int ys[23] = {64, 144, 80, 160, 144,
                          256, 272, 336, 304, 400,
                          448, 432, 544, 608, 560,
                          560, 784, 704, 688, 800,
                          496, 656, 800};
            for (int i = 0; i < 23; i++) {
                GameNPC* NPCTree = new GameNPCTree();
                NPCTree->Init(dialog, (GameCharacter*)gp);
                NPCTree->SetX(xs[i] - 64);
                NPCTree->SetY(ys[i] - 176);
                mapNPC.push_back(NPCTree);
            }
            GameNPC* NPCItem3 = new GameNPCItem3();
            NPCItem3->Init(dialog, (GameCharacter*)gp);
            mapNPC.push_back(NPCItem3);

        }
    }
    #endif

    mapLoaded = true;
    return true;
}

bool GameMap::SaveMap(string filename) {
    if (!mapLoaded) {
        return false;
    }
    FILE* mapHandle = fopen(filename.c_str(), "wb");
    if (mapHandle == NULL) {
        printf("Can't open map file for saving: %s", filename.c_str());
        return false;
    }

    // Print the header
    fprintf(mapHandle, "DRM\6");
    fprintf(mapHandle, "%c%c%s%c", strlen(tileset)+1, 0, tileset, 0);
    fprintf(mapHandle, "%c%c%c%c", mapWidth, mapHeight, defTile.tX, defTile.tY);

    // Dump tiles.
    for (unsigned int i = 0; i < mapTiles.size(); i++) {
        for (unsigned int j = 0; j < mapTiles[i].size(); j++) {
            fprintf(mapHandle, "%c%c", mapTiles[i][j].tX, mapTiles[i][j].tY);
        }
    }
    fprintf(mapHandle, ":");

    // Print a dummy NPC list.
    fprintf(mapHandle, "%c", PH.size());
    for (unsigned i = 0; i < PH.size(); i++) {
        fwrite(&PH[i].r, sizeof(PH[i].r), 1, mapHandle);
        fprintf(mapHandle, "%c%s", strlen(PH[i].img), PH[i].img);

    }
    fprintf(mapHandle, ":");

    // Print the link list.
    fprintf(mapHandle, "%c", mapLink.size());
    for (unsigned int i = 0; i < mapLink.size(); i++) {
        fprintf(mapHandle, "%c%c%c%c", mapLink[i].r.x / 16, mapLink[i].r.y / 16, mapLink[i].r.w / 16, mapLink[i].r.h / 16);
        fprintf(mapHandle, "%c%c%s%c", mapLink[i].destination.length() + 1, 0, mapLink[i].destination.c_str(), 0);
        fprintf(mapHandle, "%c%c", mapLink[i].destx / 16, mapLink[i].desty / 16);
    }
    fprintf(mapHandle, ":");

    // Print the block list.
    fprintf(mapHandle, "%c", mapBlock.size());
    for (unsigned int i = 0; i < mapBlock.size(); i++) {
        fprintf(mapHandle, "%c%c%c%c", mapBlock[i].x / 16, mapBlock[i].y / 16, mapBlock[i].w / 16, mapBlock[i].h / 16);
    }
    fprintf(mapHandle, ":");

    fclose(mapHandle);

    return true;
}

void GameMap::drawMap() {
    if (screen == NULL) return;
    if (mapSurface != NULL) SDL_FreeSurface(mapSurface);

    mapSurface = GameDraw::CreateBlankSurface(mapWidth * 16, mapHeight * 16);

    if (tilesetSurface == NULL)
        LoadTileSet(string(tileset));

    for (int i = 0; i < mapHeight; ++i) {
        for (int j = 0; j < mapWidth; j++) {
            Tile* _tmpTile = &mapTiles[i][j];
            SDL_Rect clip;
            clip.x = defTile.tX * 16;
            clip.y = defTile.tY * 16;
            clip.w = clip.h = 16;
            // Draw the default tile
            GameDraw::RenderToSurface(j*16, i*16, tilesetSurface, mapSurface, &clip);
            // Draw the actual tile overtop of it.
            clip.x = _tmpTile->tX * 16;
            clip.y = _tmpTile->tY * 16;
            GameDraw::RenderToSurface(j*16, i*16, tilesetSurface, mapSurface, &clip);
        }
    }

    if (mapEdit) {
        for (unsigned int i = 0; i < PH.size(); i++)
            GameDraw::RenderToSurface(PH[i].r.x, PH[i].r.y, PH[i].surface, mapSurface, &PH[i].surface->clip_rect);
        for (unsigned int i = 0; i < mapBlock.size(); i++)
            GameDraw::DrawSquare(mapBlock[i], mapSurface, SDL_MapRGB(mapSurface->format, 0xFF, 0, 0));
        for (unsigned int i = 0; i < mapLink.size(); i++)
            GameDraw::DrawSquare(mapLink[i].r, mapSurface, SDL_MapRGB(mapSurface->format, 0, 0xFF, 0xFF));
    }
}

void GameMap::SetTileset(string ts) {
    if (tileset)
        delete [] tileset;
    tileset = new char[ts.length() + 1];
    memcpy(tileset, ts.c_str(), ts.length());
    tileset[ts.length()] = '\0';
    LoadTileSet(tileset);
}

void GameMap::LoadTileSet(string filename) {
    if (screen == NULL) return;
    if (tilesetSurface != NULL) SDL_FreeSurface(tilesetSurface);
    tilesetSurface = GameDraw::LoadImage(filename.c_str(),
                                  SDL_MapRGB(screen->format, 0xFF, 0x00, 0xFF));
    if (tilesetSurface == NULL) printf("Empty tileset!\n");
}

bool GameMap::isBlocking(SDL_Rect rect) {
    if (rect.x < 0 || rect.y < 0 ||
        rect.x + rect.w > mapWidth * 16 ||
        rect.y + rect.h > mapHeight * 16) return true;
    for (unsigned int i = 0; i < mapNPC.size(); i++) {
        if (rectCollide(mapNPC.at(i)->GetBlock(), rect)) {
            mapNPC.at(i)->Collide();
            return true;
        }
    }
    for (unsigned int i = 0; i < mapBlock.size(); ++i) {
        if (rectCollide(mapBlock.at(i), rect)) return true;
    }
    return false;
}

SDL_Rect GameMap::isMapChange(SDL_Rect rect) {
    for (unsigned int i = 0; i < mapLink.size(); i++) {
        if (rectCollide(mapLink[i].r, rect)) {
//            printf("Loading new map %s (%d, %d)\n", mapLink[i].destination.c_str(), mapLink[i].destx, mapLink[i].desty);
            rect.x = mapLink[i].destx;
            rect.y = mapLink[i].desty;
            LoadMap(mapLink[i].destination);
            drawMap();
            return rect;
        }
    }
    rect.x = rect.y = -1;
    return rect;
}

// Basic blocking editor
void GameMap::handleInput(GameInput *gi) {

}

void GameMap::runScripts(int dt) {
    for (unsigned i = 0; i < mapNPC.size(); i++) {
        mapNPC.at(i)->Step(dt);
    }
}

void GameMap::setTile(int x, int y, int tilex, int tiley) {
    mapTiles[y][x].tX = tilex;
    mapTiles[y][x].tY = tiley;
}
Tile GameMap::getTile(int x, int y) {
    return mapTiles[y][x];
}

void GameMap::paintTile(int x, int y, SDL_Rect tile) {

}

void GameMap::clearTiles(SDL_Rect r) {
    for (int i = 0; i < r.w / 16; i++) {
        for (int j = 0; j < r.h / 16; j++) {
            mapTiles[j + r.y/16][i + r.x/16].tX = defTile.tX;
            mapTiles[j + r.y/16][i + r.x/16].tY = defTile.tY;
        }
    }
}

void GameMap::addBlock(SDL_Rect r) {
    // Check if the block already exists.
    for (unsigned int i = 0; i < mapBlock.size(); i++) {
        if (mapBlock[i].x == r.x && mapBlock[i].y == r.y &&
            mapBlock[i].w == r.w && mapBlock[i].h == r.h) {
            return;
        }
    }
    // Else, add the block.
    mapBlock.push_back(r);
}

void GameMap::delBlock(SDL_Rect r) {
    for (unsigned int i = 0; i < mapBlock.size(); i++) {
        if (mapBlock[i].x == r.x && mapBlock[i].y == r.y &&
            mapBlock[i].w == r.w && mapBlock[i].h == r.h) {
            mapBlock.erase(mapBlock.begin() + i);
            return;
        }
    }
}

void GameMap::addLink(Link l) {
    mapLink.push_back(l);
}

Link* GameMap::getLink(SDL_Rect r) {
    for (unsigned int i = 0; i < mapLink.size(); i++) {
        if (mapLink[i].r.x == r.x && mapLink[i].r.y == r.y &&
            mapLink[i].r.w == r.w && mapLink[i].r.h == r.h) {
                return &mapLink[i];
        }
    }
    return NULL;
}

void GameMap::delLink(SDL_Rect r) {
    for (unsigned int i = 0; i < mapLink.size(); i++) {
        if (mapLink[i].r.x == r.x && mapLink[i].r.y == r.y &&
            mapLink[i].r.w == r.w && mapLink[i].r.h == r.h) {
                mapLink.erase(mapLink.begin() + i);
                return;
        }
    }
}

void GameMap::clearBlock(SDL_Rect r) {
    // Clear all blocks that are encompassed in r
    unsigned int i = 0;
    while (i < mapBlock.size()) {
        if (mapBlock[i].x >= r.x && mapBlock[i].y >= r.y &&
            mapBlock[i].x + mapBlock[i].w <= r.x + r.w &&
            mapBlock[i].y + mapBlock[i].h <= r.y + r.h)
                mapBlock.erase(mapBlock.begin() + i);
            else
                i++;
    }
}

void GameMap::addPH(placeholder p) {
    PH.push_back(p);
}

void GameMap::delPH(SDL_Rect r) {
    for (unsigned i = 0; i < PH.size(); i++) {
        if (PH[i].r.x == r.x && PH[i].r.y == r.y) {
                delete [] PH[i].img;
                SDL_FreeSurface(PH[i].surface);
                PH.erase(PH.begin() + i);
                return;
            }
    }
}

placeholder* GameMap::getPH(SDL_Rect r) {
    for (unsigned i = 0; i < PH.size(); i++) {
        if (r.x > PH[i].r.x && r.y > PH[i].r.y &&
            r.x < PH[i].r.x + PH[i].surface->w &&
            r.y < PH[i].r.y + PH[i].surface->h) {
                return &PH[i];
            }
    }
    return NULL;
}
