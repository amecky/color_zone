#include "Block.h"
#include <math.h>
#include "..\TileMap.h"

const float ROTATION_TIME = 0.2f;

const float STARTING_ANGLES[] = { DEGTORAD(225.0f), DEGTORAD(135.0f) , DEGTORAD(45.0f) , DEGTORAD(315.0f) };

namespace block {

	void shift_array(int* values, int num) {
		if (num > 1) {
			int tmp = values[0];
			for (int j = 0; j < num - 1; ++j) {
				values[j] = values[j + 1];
			}
			values[num - 1] = tmp;
		}
	}

	void copy_array(const int* src, int* dest, int num) {
		for (int i = 0; i < num; ++i) {
			dest[i] = src[i];
		}
	}

}

void pick_block_colors(Block* block) {
	int firstColor = ds::random(0, 3);
	int secondColor = ds::random(0, 3);
	block->colors[0] = firstColor;
	block->colors[1] = firstColor;
	block->colors[2] = secondColor;
	block->colors[3] = secondColor;
	int r = ds::random(0, 6);
	for (int i = 0; i < r; ++i) {
		block::shift_array(block->colors, 4);
	}
}

void initialise_block(Block* block) {
	block->position = p2i(512, 384);
	for (int i = 0; i < 4; ++i) {
		block->colors[i] = 0;
	}
	pick_block_colors(block);
	block->rotating = false;
	block->rotationRadius = length(ds::vec2(HALF_SQUARE_SIZE, HALF_SQUARE_SIZE));
	block->scale = ds::vec2(1, 1);
	block->flashing = false;
	block->flashTimer = 0.0f;
	block->rotationTimer = 0.0f;
}

void copy_block_colors(Block* current, Block* other) {
	for (int i = 0; i < 4; ++i) {
		current->colors[i] = other->colors[i];
	}
}

// -----------------------------------------------------------------
// render block
// -----------------------------------------------------------------
void render_block(Block* block, SpriteBatchBuffer* buffer, ds::Color* colors) {
	ds::vec2 p = ds::vec2(block->position.x, block->position.y) + ds::vec2(HALF_SQUARE_SIZE, HALF_SQUARE_SIZE);
	float norm = block->rotationTimer / ROTATION_TIME;
	ds::vec4 tex(36, 0, 36, 36);
	for (int i = 0; i < 4; ++i) {
		float angle = STARTING_ANGLES[i];
		if (block->rotating) {
			angle += norm * ds::PI * 0.5f;
		}
		ds::vec2 pp = ds::vec2(p.x + block->rotationRadius * cosf(angle), p.y + block->rotationRadius * sinf(angle));
		buffer->add(pp, tex, block->scale, angle + DEGTORAD(45.0f), colors[block->colors[i]]);
	}
}

// -----------------------------------------------------------------
// render block and a box surrounding it
// -----------------------------------------------------------------
void render_block_boxed(Block* block, SpriteBatchBuffer* buffer, ds::Color* colors) {
	float r = 0.0f;
	ds::vec2 p = ds::vec2(block->position.x, block->position.y) + ds::vec2(HALF_SQUARE_SIZE, HALF_SQUARE_SIZE);
	ds::vec4 t(220, 140, 78, 78);
	if (block->rotating) {
		float norm = block->rotationTimer / ROTATION_TIME;
		r = norm * ds::PI * 0.5f;
	}
	buffer->add(p, t, ds::vec2(1, 1), r, { 64,64,64,255 });
	render_block(block, buffer, colors);
}

void follow_mouse(Block* block) {
	ds::vec2 mp = ds::getMousePosition();
	p2i tmp = map::screen2grid(mp);
	if (tmp.x >= MAX_X - 1) {
		tmp.x = MAX_X - 2;
	}
	if (tmp.y >= MAX_Y - 1) {
		tmp.y = MAX_Y - 2;
	}
	block->position = map::grid2screen(tmp);

}

void flash_block_scale(Block* block, float dt, float flashTTL) {
	if (block->flashing) {
		block->flashTimer += dt;
		if (block->flashTimer >= flashTTL) {
			block->flashing = false;
		}
		else {
			float norm = 0.2f + block->flashTimer / flashTTL * 0.8f;
			block->scale = ds::vec2(norm, norm);
		}
	}
}

// -----------------------------------------------------------------
// update
// -----------------------------------------------------------------
void rotate_block(Block* block, float dt) {
	if (block->rotating) {
		block->rotationTimer += dt;
		float norm = block->rotationTimer / ROTATION_TIME;
		if (norm >= 1.0f) {
			block::shift_array(block->colors, 4);
			block->rotating = false;
		}
	}
}

