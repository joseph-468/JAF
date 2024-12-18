#include "core.h"

#include <SDL.h>
#include <iostream>

namespace JAF {
    App::App() :
        initializedJAF(false),
        running(false),
        window(nullptr),
        renderer(nullptr),
        logLevel(spdlog::level::off),
        leftMousePressed(false),
        rightMousePressed(false),
        leftMouseDown(false),
        rightMouseDown(false),
        updatesPerSecond(-1),
        previousTickTime(0),
        deltaTime(-1),
        mouseX(-1),
        mouseY(-1),
        windowX(-1),
        windowY(-1),
        screenWidth(-1),
        screenHeight(-1),
        eventQueuePos(0) {
        initTime = SDL_GetPerformanceCounter();
        eventQueue.resize(MAX_WIDGET_EVENTS);
    }

    void App::run() {
        initJAF();
        running = initializedJAF;
        if (initializedJAF) {
            init();
        }

        while (running) {
            for (const auto &widget : widgets) {
                widget->update(this);
            }

            handleEvents();

            if (const double currentTickTime = getCurrentTime();
                currentTickTime - previousTickTime >= updatesPerSecond) {
                deltaTime = currentTickTime - previousTickTime;
                previousTickTime = currentTickTime;

                update();

                eventQueuePos = 0;
                for (auto &event : eventQueue) {
                    delete event;
                    event = nullptr;
                }
            }

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

    void App::handleEvents() {
        leftMousePressed = false;
        rightMousePressed = false;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            const auto appEvent = new AppEvent(AppEvent::SDL_EVENT);
            appEvent->event = event;
            appEvent->origin = this;
            appEvent->unseen = true;
            if (eventQueuePos < MAX_WIDGET_EVENTS) {
                eventQueue[eventQueuePos++] = appEvent;
            }

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
    }


    Event *App::getEvent(const void *const widget) const {
        Event *event;
        if (widget) {
            for (size_t i = 0; i < MAX_WIDGET_EVENTS; i++) {
                if (eventQueue[i] && eventQueue[i]->unseen && eventQueue[i]->origin == widget) {
                    event = eventQueue[i];
                    event->unseen = false;
                    return event;
                }
            }
        }
        else {
            for (size_t i = 0; i < MAX_WIDGET_EVENTS; i++) {
                if (eventQueue[i] && eventQueue[i]->unseen) {
                    event = eventQueue[i];
                    event->unseen = false;
                    return event;
                }
            }
        }
        return nullptr;
    }

    void App::initJAF() {
        spdlog::set_level(JAF_DEFAULT_LOG_LEVEL);
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
        if (value == 0) {
            updatesPerSecond = std::numeric_limits<double>::infinity();
        }
        else if (value > 0) {
            updatesPerSecond = 1000 / value / 1000;
        }
        else {
            updatesPerSecond = 0;
        }
    }

    void App::setRunning(const bool value) {
        running = value;
    }

    void setLogLevel(const spdlog::level::level_enum level) {
        spdlog::set_level(level);
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
