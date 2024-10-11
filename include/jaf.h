#pragma once

namespace JAF {
    class App {
    public:
        virtual ~App() = default;
        void run() {
            init();
            while (running) {
                update();
                render();
            }
            quit();
        }

    protected:
        virtual void init() = 0;
        virtual void update() = 0;
        virtual void render() = 0;
        virtual void quit() = 0;

        bool running = false;
    };
}