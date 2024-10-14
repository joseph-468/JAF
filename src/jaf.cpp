#include "jaf.h"

#include <SDL.h>
#include <iostream>

namespace JAF {
    void App::run() {
        initJAF();

        init();
        while (running) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }
            }

            update();
            render();

            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
        }
        quit();

        quitJAF();
    }

    void App::initJAF() {
        SDL_Init(SDL_INIT_EVERYTHING);
        window = SDL_CreateWindow("JAF Application", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }

    void App::quitJAF() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        renderer = nullptr;
        window = nullptr;
        SDL_Quit();
    }
}