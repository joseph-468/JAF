#include "jaf.h"

#include <cassert>
#include <iostream>

constexpr auto SCREEN_WIDTH = 512;
constexpr auto SCREEN_HEIGHT = 512;
constexpr auto WINDOW_TITLE = "Example - Falling Sand";


class App final : public JAF::App {
protected:
    void init() override {
        createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

        grid.resize(128 * 128);

        canvas = new JAF::Canvas(this);
        canvas->x = 0;
        canvas->y = 0;
        canvas->w = 512;
        canvas->h = 512;
        canvas->addTexture(128, 128);
    }

    void update() override {
        if (canvas->pressed) {
            grid[canvas->pressedX + canvas->pressedY * canvas->textureWidth] = 0xFF00FFFF;
        }
    }

    void render() override {
        canvas->updateTexture(grid);
        renderWidgets();
    }

    void quit() override {}

    Uint32 color = 0xFF0000FF;
    std::vector<Uint32> grid;
    JAF::Canvas *canvas{};
};

int main(int argc, char *argv[]) {
    App app;
    app.run();
    return 0;
}
