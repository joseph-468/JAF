#include "jaf.h"

#include <cassert>
#include <iostream>

constexpr auto SCREEN_WIDTH = 512;
constexpr auto SCREEN_HEIGHT = 512;
constexpr auto WINDOW_TITLE = "Example - Falling Sand";


enum class CellType {
    AIR,
    SAND,
    WATER,
};

struct Cell {
    CellType type = CellType::AIR;
    JAF::Color color = { 0, 0, 0};
};

class World {
public:
    explicit World(const Sint32 width, const Sint32 height) : width(width), height(height) {
        grid.clear();
        grid.resize(width * height);

        texture.clear();
        texture.resize(width * height);
    }

    void setCell(const Sint32 x, const Sint32 y, const Cell &cell) {
        const Sint32 dst = x + y * width;
        grid[dst] = cell;
    }

    void updateTexture() {
        for (Sint32 i = 0; i < grid.size(); i++) {
            texture[i] = grid[i].color.data();
        }
    }

    Sint32 width{};
    Sint32 height{};
    std::vector<Cell> grid;
    std::vector<Uint32> texture;
};

class App final : public JAF::App {
protected:
    void init() override {
        createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

        canvas = new JAF::Canvas(this);
        canvas->x = 0;
        canvas->y = 0;
        canvas->w = 512;
        canvas->h = 512;
        canvas->addTexture(world.width, world.height);
    }

    void update() override {
        const auto currentTicks = static_cast<Sint64>(getCurrentTime() / (1000.0f / static_cast<float>(tickRate) / 1000.0f));
        if (currentTicks <= ticks) {
            return;
        }
        ticks = currentTicks;

        if (canvas->pressed) {
            const Cell newCell = { CellType::SAND, { 203, 189, 147 } };
            world.setCell(canvas->pressedX, canvas->pressedY, newCell);
        }

        for (Sint32 y = world.height - 1; y > 0; y--) {
            for (Sint32 x = 0; x < world.width; x++) {
                const Sint32 dst = x + y * world.width;
                if (world.grid[dst].type == CellType::AIR && world.grid[dst - world.width].type == CellType::SAND) {
                    world.grid[dst] = world.grid[dst - world.width];
                    world.grid[dst - world.width].type = CellType::AIR;
                    world.grid[dst - world.width].color = { 0, 0, 0, 0 };
                }
            }
        }
    }

    void render() override {
        world.updateTexture();
        canvas->updateTexture(world.texture);
        renderWidgets();
    }

    void quit() override {}

    static constexpr auto DEFAULT_WORLD_WIDTH = 128;
    static constexpr auto DEFAULT_WORLD_HEIGHT = 128;
    static constexpr auto DEFAULT_TICK_RATE = 64;


    Sint64 ticks{};
    Sint32 tickRate = DEFAULT_TICK_RATE;
    World world{ DEFAULT_WORLD_WIDTH, DEFAULT_WORLD_HEIGHT };
    JAF::Canvas *canvas{};
};

int main(int argc, char *argv[]) {
    App app;
    app.run();
    return 0;
}
