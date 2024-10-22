#pragma once

#include <SDL.h>
#include <vector>

namespace JAF {
    class App;

    struct Color {
        Uint8 r;
        Uint8 g;
        Uint8 b;
        Uint8 a;
    };

    class Widget {
    public:
        virtual ~Widget() = default;
        virtual void display(App *app) = 0;
        virtual void handleEvent(App *app, const SDL_Event &event) = 0;
    };

    class Button final : public Widget {
    public:
        Button() = default;
        ~Button() override = default;
        void display(App *app) override;
        void handleEvent(App *app, const SDL_Event &event) override;

        int x{}, y{};
        int w{}, h{};
        bool pressed{};
        Color color{};
    };

    class App {
    public:
        App();
        virtual ~App() = default;
        void run();

        virtual void init() = 0;
        virtual void update() = 0;
        virtual void quit() = 0;

        void createWindow(int width, int height, const char *title);
        void destroyWindow();
        void drawRectangle(int x, int y, int w, int h, Color color) const;
        void addWidget(Widget *widget);
        void removeWidget(const Widget *widget);

        [[nodiscard]] int getMouseX() const noexcept { return mouseX; }
        [[nodiscard]] int getMouseY() const noexcept { return mouseY; }
        [[nodiscard]] int getScreenWidth() const noexcept { return screenWidth; }
        [[nodiscard]] int getScreenHeight() const noexcept { return screenHeight; }
        [[nodiscard]] int getRunning() const noexcept { return running; }

        void setRunning(const bool value) noexcept { running = value; }

    private:
        void initJAF();
        void quitJAF();
        void renderWidgets();

        bool running;
        SDL_Window *window;
        SDL_Renderer *renderer;
        int mouseX;
        int mouseY;
        int screenWidth;
        int screenHeight;
        std::vector<Widget *> widgets;
    };
}
