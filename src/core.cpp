#include "core.h"

#include <SDL.h>

namespace JAF {
    App::App() :
        initializedJAF(false),
        running(false),
        window(nullptr),
        renderer(nullptr),
        mouseX(-1),
        mouseY(-1),
        screenWidth(-1),
        screenHeight(-1) {}

    void App::run() {
        initJAF();
        running = initializedJAF;
        if (initializedJAF) {
            init();
        }

        while (running) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }
                else if (event.type == SDL_MOUSEMOTION) {
                    mouseX = event.motion.x;
                    mouseY = event.motion.y;
                }
                else {
                    for (const auto &widget : widgets) {
                        widget->handleEvent(this, event);
                    }
                }
            }

            update();

            JAF_ASSERT(renderer != nullptr);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            render();

            JAF_ASSERT(renderer != nullptr);
            SDL_RenderPresent(renderer);
        }

        if (initializedJAF) {
            quit();
        }
        quitJAF();
    }

    void App::initJAF() {
        initializedJAF = true;
        const bool initializedSDL = SDL_Init(SDL_INIT_EVERYTHING) == 0;
        if (!initializedSDL) {
            initializedJAF = false;
        }
    }

    void App::quitJAF() {
        destroyWindow();
        SDL_Quit();
    }

    void App::createWindow(const Sint32 width, const Sint32 height, const char *const title) {
        destroyWindow();

        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        screenWidth = width;
        screenHeight = height;
    }

    void App::destroyWindow() {
        if (renderer) {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }
        if (window) {
            SDL_DestroyWindow(window);
            window = nullptr;
            screenWidth = -1;
            screenHeight = -1;
        }
    }

    void App::drawRectangle(const Sint32 x, const Sint32 y, const Sint32 w, const Sint32 h, const Color color) const {
        JAF_ASSERT(renderer != nullptr);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        const SDL_Rect dst = { x, y, w, h };
        SDL_RenderFillRect(renderer, &dst);
    }

    void App::renderWidgets() {
        for (const auto &widget : widgets) {
            widget->display(this);
        }
    }

    void App::addWidget(Widget *const widget) {
        widget->app = this;
        widgets.push_back(widget);
    }

    void App::removeWidget(const Widget *const widget) {
        for (Sint32 i = 0; i < widgets.size(); i++) {
            if (widgets[i] == widget) {
                widgets.erase(widgets.begin() + i);
            }
        }
    }
}
