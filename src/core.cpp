#include "core.h"

#include <SDL.h>

namespace JAF {
    App::App() :
        initializedJAF(false),
        running(false),
        window(nullptr),
        renderer(nullptr),
        leftMousePressed(false),
        rightMousePressed(false),
        leftMouseDown(false),
        rightMouseDown(false),
        updatesPerSecond(-1),
        previousTickTime(-1),
        mouseX(-1),
        mouseY(-1),
        windowX(-1),
        windowY(-1),
        screenWidth(-1),
        screenHeight(-1) {
        initTime = SDL_GetPerformanceCounter();
    }

    void App::run() {
        initJAF();
        running = initializedJAF;
        if (initializedJAF) {
            init();
        }

        while (running) {
            if (const double currentTickTime = getCurrentTime();
                currentTickTime - previousTickTime >= updatesPerSecond) {
                previousTickTime = currentTickTime;
            }
            else {
                continue;
            }

            leftMousePressed = false;
            rightMousePressed = false;

            for (const auto &widget : widgets) {
                widget->update(this);
            }

            // TODO
            // With low update rates events aren't polled frequently enough to be caught.
            // Should implement a way to poll for events and then process them the next update.
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT: {
                        running = false;
                    } break;

                    case SDL_WINDOWEVENT: {
                        if (event.window.event == SDL_WINDOWEVENT_MOVED) {
                            windowX = event.window.data1;
                            windowY = event.window.data2;
                        }
                    } break;

                    case SDL_MOUSEBUTTONDOWN:
                    case SDL_MOUSEBUTTONUP: {
                        if (event.button.button == 1) {
                            if (event.button.state == SDL_PRESSED && !leftMouseDown) {
                                leftMousePressed = true;
                            }
                            leftMouseDown = event.button.state;
                        }
                        else if (event.button.button == 3) {
                            rightMousePressed = false;
                            if (event.button.state == SDL_PRESSED && !rightMouseDown) {
                                rightMousePressed = true;
                            }
                            rightMouseDown = event.button.state;
                        }
                    } break;
                }
                for (const auto &widget : widgets) {
                    widget->handleEvent(this, event);
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

        window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN
        );
        JAF_ASSERT(window != nullptr);

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        JAF_ASSERT(renderer != nullptr);

        screenWidth = width;
        screenHeight = height;
        SDL_GetWindowPosition(window, &windowX, &windowY);
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

    double App::getCurrentTime() const {
        return static_cast<double>(SDL_GetPerformanceCounter() - initTime) / static_cast<double>(SDL_GetPerformanceFrequency());
    }

    Sint32 App::getMouseX() const {
        Sint32 mouseX;
        SDL_GetGlobalMouseState(&mouseX, nullptr);
        return mouseX - windowX;
    }

    Sint32 App::getMouseY() const {
        Sint32 mouseY;
        SDL_GetGlobalMouseState(nullptr, &mouseY);
        return mouseY - windowY;
    }

    void App::setUpdatesPerSecond(const double value) {
        updatesPerSecond = 1000 / value / 1000;
    }

    void App::setRunning(const bool value) {
        running = value;
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
