// main.cpp
#include "Game.h"

int SDL_main(int argc, char* argv[]) {
    auto game = std::make_unique<Game>(); // Smart pointer 

    if (!game->init("Space Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600)) {
        return 1;
    }

    while (game->running()) {
        game->handleEvents();
        game->update();
        game->render();
        SDL_Delay(16);  // Cap at ~60 FPS
    }

    return 0;
}
