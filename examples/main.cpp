#include "jaf.h"

#include <cassert>

constexpr auto SCREEN_WIDTH = 854;
constexpr auto SCREEN_HEIGHT = 480;
constexpr auto WINDOW_TITLE = "Example - Main";

class App final : public JAF::App {
protected:
    void init() override {
        createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

        auto const but = new JAF::Button();
        addWidget(but);
        but->w = getScreenWidth() / 4;
        but->h = getScreenHeight() / 4;
        but->x = getScreenWidth() / 2 - but->w / 2;
        but->y = getScreenHeight() / 2 - but->h / 2;
        but->color = { 255, 0, 0, 255 };
        widgets.push_back(but);
    }

    void update() override {
        const auto but = dynamic_cast<JAF::Button *>(widgets[0]);
        if (but->pressed) {
            but->color = { 0, 255, 0, 255 };
        }
        else {
            but->color = { 255, 0, 0, 255 };
        }
    }

    void render() override {
        renderWidgets();
    }

    void quit() override {}

    std::vector<JAF::Widget *> widgets;
};

int main(int argc, char *argv[]) {
    App app;
    app.run();
    return 0;
}
