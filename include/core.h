#pragma once

#include "color.h"
#include "widgets.h"

#include <SDL.h>
#include <vector>
#include <cassert>

#define JAF_ASSERT(expression) { \
    assert(expression); \
}

namespace JAF {
    class Widget;

    class App {
    public:
        App();
        virtual ~App() = default;
        void run();

        virtual void init() = 0;
        virtual void update() = 0;
        virtual void render() = 0;
        virtual void quit() = 0;

        void createWindow(Sint32 width, Sint32 height, const char *title);
        void destroyWindow();
        void renderWidgets();
        void addWidget(Widget *widget);
        void removeWidget(const Widget *widget);

        void drawRectangle(Sint32 x, Sint32 y, Sint32 w, Sint32 h, Color color) const;

        [[nodiscard]] Sint32 getMouseX() const noexcept { return mouseX; }
        [[nodiscard]] Sint32 getMouseY() const noexcept { return mouseY; }
        [[nodiscard]] Sint32 getScreenWidth() const noexcept { return screenWidth; }
        [[nodiscard]] Sint32 getScreenHeight() const noexcept { return screenHeight; }
        [[nodiscard]] Sint32 getRunning() const noexcept { return running; }
        [[nodiscard]] SDL_Renderer *getRenderer() const noexcept { return renderer; }

        void setRunning(const bool value) noexcept { running = value; }

    private:
        void initJAF();
        void quitJAF();

        bool initializedJAF;
        bool running;
        SDL_Window *window;
        SDL_Renderer *renderer;
        Sint32 mouseX;
        Sint32 mouseY;
        Sint32 screenWidth;
        Sint32 screenHeight;
        std::vector<Widget *> widgets;
    };
}
