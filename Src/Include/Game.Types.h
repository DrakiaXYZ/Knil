#ifndef GAMETYPES_H
#define GAMETYPES_H

#include <SDL/SDL.h>
#include <string>
using std::string;
#define DEBUG

typedef unsigned __int8 byte;
typedef unsigned __int16 word;
typedef unsigned __int32 dword;
typedef unsigned __int64 qword;

struct coord {
    coord (float _x, float _y) {
        x = _x;
        y = _y;
    };
    coord () {x = 0; y = 0;};
    float x, y;
};

struct Tile {
    int mX, mY;
    int tX, tY;
    //bool block;
    int block;
    void clear() {
        mX = 0; mY = 0; tX = 0; tY = 0; block = false;
    };
};

struct Link {
    SDL_Rect r;
    string destination;
    int destx;
    int desty;
};

struct placeholder {
    SDL_Rect r;
    char* img;
    SDL_Surface* surface;
};

struct Rect {
    int x, y, x2, y2;
    Rect(int _x, int _y, int _x2, int _y2) {
        x = _x;
        y = _y;
        x2 = _x2;
        y2 = _y2;
    }
    bool Intersect(Rect *_tRect) {
        if (_tRect->x > x2 || _tRect->x2 < x)
            return false;
        if (_tRect->y > y2 || _tRect->y2 < y)
            return false;
        return true;
    }
};

struct KCAFrame {
    word time;
    word nextFrame;
    word imageX;
    word imageY;
    word imageW;
    word imageH;
};

struct KCAAnim {
    word frameCount;

    KCAFrame** frames;
};

struct KCAFile {
    byte header[3];
    byte version;
    word imageLen;
    char* image;
    word aniCount;

    KCAAnim** animations;
};
#endif
