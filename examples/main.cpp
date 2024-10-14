#include "jaf.h"

#include <iostream>

class App final : public JAF::App {
protected:
    void init() override {
        std::cout << "Init" << std::endl;
        running = true;
    }
    void update() override {
        std::cout << "Updating" << std::endl;
    }
    void render() override {
        std::cout << "Rendering" << std::endl;
    }
    void quit() override {
        std::cout << "Quitting" << std::endl;
    }
};

int main(int argc, char *argv[]) {
    App app;
    app.run();
    return 0;
}
