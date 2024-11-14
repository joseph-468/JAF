#include "jaf.h"

#include <cassert>
#include <iostream>

constexpr auto SCREEN_WIDTH = 512;
constexpr auto SCREEN_HEIGHT = 512;
constexpr auto WINDOW_TITLE = "Example - Falling Sand";


enum class CellType {
    AIR,
    WALL,
    SAND,
    WATER,
};

struct Cell {
    CellType type = CellType::AIR;
    JAF::Color color = { 0, 0, 0};

    static const Cell EMPTY;
};
const Cell Cell::EMPTY = { CellType::AIR, { 0, 0, 0 }};

class World {
public:
    explicit World(const Sint32 width, const Sint32 height, const CellType borderType) : width(width), height(height), borderType(borderType) {
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
    CellType borderType;
};

class App final : public JAF::App {
protected:
    void init() override {
        createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
        setUpdatesPerSecond(tickRate);

        canvas = new JAF::Canvas(this);
        canvas->x = 0;
        canvas->y = 0;
        canvas->w = 512;
        canvas->h = 512;
        canvas->addTexture(world.width, world.height);

        borderTypeButton = new JAF::Button(this);
        borderTypeButton->color = { 255, 0, 0 };
        borderTypeButton->x = 0;
        borderTypeButton->y = 0;
        borderTypeButton->w = 30;
        borderTypeButton->h = 10;
    }

    void update() override {
        if (borderTypeButton->isPressed()) {
            world.borderType = (world.borderType == CellType::AIR) ? CellType::WALL : CellType::AIR;
        }

        for (Sint32 y = world.height - 1; y > -1; y--) {
            for (Sint32 x = 0; x < world.width; x++) {
                const Sint32 dst = x + y * world.width;
                if (world.grid[dst].type == CellType::SAND) {
                    if (y == world.height - 1) {
                        if (world.borderType == CellType::AIR) {
                            world.grid[dst] = Cell::EMPTY;
                        }
                    }
                    if (world.grid[dst + world.width].type == CellType::AIR) {
                        world.grid[dst + world.width] = world.grid[dst];
                        world.grid[dst] = Cell::EMPTY;
                    }
                }
            }
        }
        if (canvas->pressed) {
            const Cell newCell = { CellType::SAND, { 203, 189, 147 } };
            world.setCell(canvas->pressedX, canvas->pressedY, newCell);
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
    World world{ DEFAULT_WORLD_WIDTH, DEFAULT_WORLD_HEIGHT, CellType::AIR };
    JAF::Button *borderTypeButton{};
    JAF::Canvas *canvas{};
};

int main(int argc, char *argv[]) {
    App app;
    app.run();
    return 0;
}
