#include "Game.Input.h"

GameInput::GameInput() {
    keyState = SDL_GetKeyState(NULL);
    quit = false;
}

GameInput::~GameInput() {

}

void GameInput::Update() {
    SDL_PumpEvents();
    mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            quit = true;
    }
}

bool GameInput::GetKeyState(SDLKey key) {
    return keyState[key];
}

bool GameInput::GetMouseDown(int button) {
    mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    return mouseState & SDL_BUTTON(button);
}

bool GameInput::GetQuit() {
    return quit;
}
