#include "jaf.h"

#include <iostream>

class App final : public JAF::App {
protected:
    void init() override {
        std::cout << "Init" << std::endl;
        running = true;
    }
    void update() override {
        std::cout << "Updating x: " << x << std::endl;
        if (x++ == 10) {
            running = false;
        }
    }
    void render() override {
        std::cout << "Rendering" << std::endl;
    }
    void quit() override {
        std::cout << "Quitting" << std::endl;
    }

    int x = 1;
};

int main() {
    App app;
    app.run();
}
