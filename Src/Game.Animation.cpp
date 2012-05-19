#include "Game.Animation.h"

GameAnimation::GameAnimation() {
    filename = "";
    aniSurface = NULL;
    aniImage = NULL;
    screen = GameDraw::screen;
    anim = 0;
    frame = 0;
    curFrameTime = 0;
    loaded = false;

    kca.image = NULL;
    kca.aniCount = 0;
    kca.animations = NULL;
}

GameAnimation::~GameAnimation() {
    if (!loaded) return;

    freeKCA();
}

bool GameAnimation::loadKCA(string file) {
    if (!filename.compare(file)) return true;
    // Don't load another KCA if we already have one loaded.
    if (loaded) freeKCA();
    if (screen == NULL) return false;

    FILE* kcafp = fopen(file.c_str(), "rb");
    if (kcafp == NULL) return false;

    // Check that this is a valid KCA file.
    fread(&kca.header, 1, 3, kcafp);
    // If this isn't 0, then we don't have a valid KCA file.
    if (memcmp(kca.header, "KCA", 3)) {
        fclose(kcafp);
        return false;
    }

    // Check the version
    fread(&kca.version, 1, 1, kcafp);
    if (kca.version != 1) {
        fclose(kcafp);
        return false;
    }

    // Get the image file.
    fread(&kca.imageLen, 2, 1, kcafp);
    kca.image = (char*)calloc(1, kca.imageLen);
    fread(kca.image, 1, kca.imageLen, kcafp);
    // Get the amount of animations.
    fread(&kca.aniCount, 2, 1, kcafp);
    kca.animations = new KCAAnim*[kca.aniCount];

    for (int i = 0; i < kca.aniCount; ++i) {
        KCAAnim* anim = new KCAAnim;
        kca.animations[i] = anim;
        fread(&anim->frameCount, 2, 1, kcafp);
        anim->frames = new KCAFrame*[anim->frameCount];
        for (int j = 0; j < anim->frameCount; ++j) {
            KCAFrame* frame = new KCAFrame;
            anim->frames[j] = frame;

            fread(frame, 1, sizeof(KCAFrame), kcafp);
        }
    }

    fclose(kcafp);

    // TODO: Load image into surface.
    aniImage = GameDraw::LoadImage(kca.image,
                                  SDL_MapRGB(screen->format, 0xFF, 0x00, 0xFF));
    if (aniImage == NULL) {
        printf("Error loading animation image %s!\n", kca.image);
        freeKCA();
        return false;
    }
    updateFrame();
    filename = file;
    loaded = true;
    return true;

}


// Return true if updateFrame is called.
bool GameAnimation::tick(dword dt) {
    bool updated = false;
    if (!loaded) return false;
    curFrameTime += dt;
    while (curFrameTime >= kca.animations[anim]->frames[frame]->time) {
        curFrameTime -= kca.animations[anim]->frames[frame]->time;
        frame = kca.animations[anim]->frames[frame]->nextFrame;
        if (frame >= kca.animations[anim]->frameCount) frame = 0;
        updateFrame();
        updated = true;
    }
    return updated;
}

void GameAnimation::updateFrame() {
    if (aniSurface)
        SDL_FreeSurface(aniSurface);

    aniSurface = GameDraw::CreateBlankSurface(kca.animations[anim]->frames[frame]->imageW,
                                       kca.animations[anim]->frames[frame]->imageH);
    SDL_Rect clipRect;
    clipRect.x = kca.animations[anim]->frames[frame]->imageX;
    clipRect.y = kca.animations[anim]->frames[frame]->imageY;
    clipRect.w = kca.animations[anim]->frames[frame]->imageW;
    clipRect.h = kca.animations[anim]->frames[frame]->imageH;
    GameDraw::RenderToSurface(0, 0, aniImage, aniSurface, &clipRect);
}

void GameAnimation::freeKCA() {
    //if (!loaded) return;

    if (kca.image)
        free(kca.image);
    kca.image = NULL;

    for (int i = 0; i < kca.aniCount; ++i) {
        for (int j = 0; j < kca.animations[i]->frameCount; ++j) {
            delete(kca.animations[i]->frames[j]);
        }
        delete[](kca.animations[i]->frames);
        delete(kca.animations[i]);
    }
    if (kca.animations)
        delete[](kca.animations);
    kca.animations = NULL;

    if (aniSurface)
        SDL_FreeSurface(aniSurface);
    aniSurface = NULL;

    if (aniImage)
        SDL_FreeSurface(aniImage);
    aniImage = NULL;

    frame = 0;
    anim = 0;
    curFrameTime = 0;
    loaded = false;
    kca.aniCount = 0;
    filename = "";
}

int GameAnimation::getWidth() {
    if (!loaded) return 0;
    return kca.animations[anim]->frames[frame]->imageW;
}

int GameAnimation::getHeight() {
    if (!loaded) return 0;
    return kca.animations[anim]->frames[frame]->imageH;
}

void GameAnimation::setAnim(int a) {
    if (!loaded) return;
    if (a == anim) return;
    if (a < kca.aniCount)
        anim = a;
    else
        anim = 0;
    frame = 0;
    curFrameTime = 0;
    updateFrame();
}
