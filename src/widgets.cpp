#include "widgets.h"

#include <string>

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

    void Canvas::addTexture(const Sint32 textureWidth, const Sint32 textureHeight) {
        texture = SDL_CreateTexture(app->getRenderer(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        void *pixels;
        int pitch;
        SDL_LockTexture(texture, nullptr, &pixels, &pitch);
        std::memset(pixels, 0, textureHeight * pitch);
        SDL_UnlockTexture(texture);
    }

    void Canvas::display(App *const app) {
        const SDL_Rect dst = { x, y, w, h };
        SDL_RenderCopy(app->getRenderer(), texture, nullptr, &dst);
    }

    void Canvas::handleEvent(App *const app, const SDL_Event &event) {
    }
}
