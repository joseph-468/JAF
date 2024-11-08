#include "widgets.h"

#include <string>
#include <iostream>

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

    void Canvas::display(App *const app) {
        const SDL_Rect dst = { x, y, w, h };
        SDL_RenderCopy(app->getRenderer(), texture, nullptr, &dst);
    }

    void Canvas::handleEvent(App *const app, const SDL_Event &event) {}

    void Canvas::addTexture(const Sint32 textureWidth, const Sint32 textureHeight) {
        this->textureWidth = textureWidth;
        this->textureHeight = textureHeight;
        texture = SDL_CreateTexture(
            app->getRenderer(),
            SDL_PIXELFORMAT_RGBA32,
            SDL_TEXTUREACCESS_STREAMING,
            textureWidth, textureHeight
        );
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        void *pixels;
        Sint32 pitch;
        SDL_LockTexture(texture, nullptr, &pixels, &pitch);
        std::memset(pixels, 0, textureHeight * pitch);
        SDL_UnlockTexture(texture);
    }

    void Canvas::updateTexture(const std::vector<Uint32> &data) const { JAF_ASSERT(texture != nullptr);
        JAF_ASSERT(data.size() == textureWidth * textureHeight);

        Uint32 *pixels;
        Sint32 pitch;
        SDL_LockTexture(texture, nullptr, reinterpret_cast<void **>(&pixels), &pitch);

        for (Sint32 y = 0; y < textureHeight; y++) {
            std::memcpy(
                pixels + y * (pitch / sizeof(Uint32)),
                data.data() + y * textureWidth,
                textureWidth * sizeof(Uint32)
            );
        }

        SDL_UnlockTexture(texture);
    }
}
