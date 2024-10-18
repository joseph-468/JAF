#include "jaf.h"

class App final : public JAF::App {
protected:
    void init() override {
        createWindow(854, 480, "Example - Main");
    }

    void update() override {
    }

    void render() override {
    }

    void quit() override {
    }
};

int main(int argc, char *argv[]) {
    App app;
    app.run();
    return 0;
}
