#include "widgets.h"

namespace JAF {
    void Button::display(App *const app) {
        app->drawRectangle(x, y, w, h, color);
    }

    void Button::handleEvent(App *const app, const SDL_Event &event) {
        const SDL_Point mousePos = { app->getMouseX(), app->getMouseY() };
        const SDL_Rect buttonRect = { x, y, w, h };
        if (!SDL_PointInRect(&mousePos, &buttonRect)) return;

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            pressed = true;
        }
        else if (event.type == SDL_MOUSEBUTTONUP) {
            pressed = false;
        }
    }
}
