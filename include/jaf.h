#pragma once

#include <SDL.h>

namespace JAF {
    class App {
    public:
        virtual ~App() = default;
        void run();

    protected:
        virtual void init() = 0;
        virtual void update() = 0;
        virtual void render() = 0;
        virtual void quit() = 0;

        bool running = false;

    private:
        void initJAF();
        void quitJAF();

        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;
    };
}