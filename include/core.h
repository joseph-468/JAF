#pragma once

#include "color.h"
#include "widgets.h"

#include <SDL.h>
#include <spdlog/spdlog.h>
#include <vector>
#include <cassert>

#define JAF_ASSERT(expression) { \
    assert(expression); \
}

#if NDEBUG == true
    #define JAF_DEFAULT_LOG_LEVEL spdlog::level::off
#else
    #define JAF_DEFAULT_LOG_LEVEL spdlog::level::trace
#endif

namespace JAF {
    class Widget;
    struct Event;

    class App {
    public:
        friend class Widget;

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
        void drawRectangle(Sint32 x, Sint32 y, Sint32 w, Sint32 h, Color color) const;

        [[nodiscard]] Event *getEvent(const void *widget = nullptr) const;
        [[nodiscard]] double getCurrentTime() const;
        [[nodiscard]] Sint32 getMouseX() const;
        [[nodiscard]] Sint32 getMouseY() const;
        [[nodiscard]] double getDeltaTime() const { return deltaTime; }
        [[nodiscard]] Sint32 getWindowX() const { return windowX; }
        [[nodiscard]] Sint32 getWindowY() const { return windowY; }
        [[nodiscard]] Sint32 getScreenWidth() const { return screenWidth; }
        [[nodiscard]] Sint32 getScreenHeight() const { return screenHeight; }
        [[nodiscard]] Sint32 getRunning() const { return running; }
        [[nodiscard]] SDL_Renderer *getRenderer() const { return renderer; }
        [[nodiscard]] bool isLeftMouseDown() const { return leftMouseDown; }
        [[nodiscard]] bool isRightMouseDown() const { return rightMouseDown; }
        [[nodiscard]] bool isLeftMousePressed() const { return leftMousePressed; }
        [[nodiscard]] bool isRightMousePressed() const { return rightMousePressed; }

        void setUpdatesPerSecond(double value);
        void setRunning(bool value);
        void setLogLevel(spdlog::level::level_enum level);

    private:
        void initJAF();
        void quitJAF();
        void addWidget(Widget *widget);
        void removeWidget(const Widget *widget);
        void handleEvents();

        static constexpr Uint64 MAX_WIDGET_EVENTS = 1024;
        bool initializedJAF;
        bool running;
        SDL_Window *window;
        SDL_Renderer *renderer;
        spdlog::level::level_enum logLevel;
        bool leftMousePressed;
        bool rightMousePressed;
        bool leftMouseDown;
        bool rightMouseDown;
        double updatesPerSecond;
        double previousTickTime;
        double deltaTime;
        Sint32 mouseX;
        Sint32 mouseY;
        Sint32 windowX;
        Sint32 windowY;
        Sint32 screenWidth;
        Sint32 screenHeight;
        Uint64 initTime;
        Uint64 eventQueuePos;
        std::vector<Event *> eventQueue;
        std::vector<Widget *> widgets;
    };
}
