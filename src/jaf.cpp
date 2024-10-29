#include "jaf.h"

#include <SDL.h>

namespace JAF {
    void Button::display(App *const app) {
        app->drawRectangle(x, y, w, h, color);
    }

    void Button::handleEvent(App *const app, const SDL_Event &event) {
        const SDL_Point mousePos = { app->getMouseX(), app->getMouseY() };
        const SDL_Rect buttonRect = { x, y, w, h };
        if (!SDL_PointInRect(&mousePos, &buttonRect)) return;

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            pressed = true;
        }
        else if (event.type == SDL_MOUSEBUTTONUP) {
            pressed = false;
        }
    }

    App::App() :
        running(false),
        window(nullptr),
        renderer(nullptr),
        mouseX(-1),
        mouseY(-1),
        screenWidth(-1),
        screenHeight(-1) {}

    void App::run() {
        initJAF();
        init();

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

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            render();
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

    void App::createWindow(const Sint32 width, const Sint32 height, const char *const title) {
        destroyWindow();

        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height, SDL_WINDOW_SHOWN);
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
