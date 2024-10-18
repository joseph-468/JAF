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
        running = true;
    }

    void App::quitJAF() {
        destroyWindow();
        SDL_Quit();
    }

    void App::createWindow(const int width, const int height, const char *const title) {
        destroyWindow();
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }

    void App::destroyWindow() {
        if (renderer) {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }
        if (window) {
            SDL_DestroyWindow(window);
            window = nullptr;
        }
    }
}
