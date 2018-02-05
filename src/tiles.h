#pragma once
#include <stdint.h>

// forward declarations
struct Tile;
class SpriteBatchBuffer;
struct GameSettings;
namespace ds {
	struct Color;
}
struct Block;

bool copy_block(Tile* tiles, const Block* block);

uint32_t get_tiles_index(uint32_t x, uint32_t y);

int calculate_fill_rate(Tile* tiles);

void render_tiles(Tile* tiles, SpriteBatchBuffer* buffer, int squareSize, float scale, ds::Color* colors, GameSettings* settings);