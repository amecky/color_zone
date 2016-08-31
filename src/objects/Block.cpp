#include "Block.h"
#include <core\math\GameMath.h>
#include "..\Constants.h"
#include <core\math\math.h>
#include <renderer\graphics.h>
#include <renderer\sprites.h>
#include <resources\ResourceContainer.h>
#include <base\InputSystem.h>
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

	// -----------------------------------------------------------------
	// pick new colors
	// -----------------------------------------------------------------
	void pick_colors(Block* block) {
		int firstColor = math::random(0, 3);
		int secondColor = math::random(0, 3);
		block->colors[0] = firstColor;
		block->colors[1] = firstColor;
		block->colors[2] = secondColor;
		block->colors[3] = secondColor;
		int r = math::random(0, 6);
		for (int i = 0; i < r; ++i) {
			shift_array(block->colors, 4);
		}
	}

	// -----------------------------------------------------------------
	// init
	// -----------------------------------------------------------------
	void init(Block* block) {
		block->position = p2i(512, 384);
		ds::SpriteSheet* spriteSheet = ds::res::getSpriteSheet("spritesheet");
		block->texture = spriteSheet->findIndex(SID("block"));
		block->boxTexture = spriteSheet->findIndex(SID("block_box"));
		for (int i = 0; i < 4; ++i) {
			block->colors[i] = 0;
		}
		pick_colors(block);
		block->rotating = false;
		block->rotationRadius = length(v2(HALF_SQUARE_SIZE, HALF_SQUARE_SIZE));
		block->scale = v2(1, 1);
		block->flashing = false;
	}

	// -----------------------------------------------------------------
	// copy colors from source block to destination block
	// -----------------------------------------------------------------
	void copy_colors(Block* dest, const Block* src) {
		copy_array(src->colors, dest->colors, 4);
	}

	// -----------------------------------------------------------------
	// flash scale
	// -----------------------------------------------------------------
	void flash_scale(Block* block, float dt, float flashTTL) {
		if (block->flashing) {
			block->flashTimer += dt;
			if (block->flashTimer >= flashTTL) {
				block->flashing = false;
			}
			else {
				float norm = 0.2f + block->flashTimer / flashTTL * 0.8f;
				block->scale = v2(norm, norm);
			}
		}
	}

	// -----------------------------------------------------------------
	// render block
	// -----------------------------------------------------------------
	void render(Block* block, ds::Color* colors) {
		ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
		ds::SpriteSheet* spriteSheet = ds::res::getSpriteSheet("spritesheet");
		v2 p = v2(block->position.x, block->position.y) + v2(HALF_SQUARE_SIZE, HALF_SQUARE_SIZE);
		float norm = block->rotationTimer / ROTATION_TIME;
		ds::Texture t = spriteSheet->get(block->texture);
		for (int i = 0; i < 4; ++i) {
			float angle = STARTING_ANGLES[i];
			if (block->rotating) {
				angle += norm * HALF_PI;
			}
			v2 pp = ds::math::getDistantPosition(p, angle, block->rotationRadius);
			sprites->draw(pp, t, angle + DEGTORAD(45.0f), block->scale, colors[block->colors[i]]);
		}
	}

	// -----------------------------------------------------------------
	// render block and a box surrounding it
	// -----------------------------------------------------------------
	void render_boxed(Block* block, ds::Color* colors) {
		render(block, colors);
		ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
		float r = 0.0f;
		ds::SpriteSheet* spriteSheet = ds::res::getSpriteSheet("spritesheet");
		v2 p = v2(block->position.x, block->position.y) + v2(HALF_SQUARE_SIZE, HALF_SQUARE_SIZE);
		ds::Texture bt = spriteSheet->get(block->boxTexture);
		if (block->rotating) {
			float norm = block->rotationTimer / ROTATION_TIME;
			r = norm * HALF_PI;
		}
		sprites->draw(p, bt, r);
	}

	// -----------------------------------------------------------------
	// start rotating
	// -----------------------------------------------------------------
	void start_rotating(Block* block) {
		if (!block->rotating) {
			block->rotating = true;
			block->rotationTimer = 0.0f;
		}
	}

	// -----------------------------------------------------------------
	// update
	// -----------------------------------------------------------------
	void update(Block* block, float dt) {
		if (block->rotating) {
			block->rotationTimer += dt;
			float norm = block->rotationTimer / ROTATION_TIME;
			if (norm >= 1.0f) {
				block::shift_array(block->colors, 4);
				block->rotating = false;
			}
		}
	}

	// -----------------------------------------------------------------
	// follow mouse
	// -----------------------------------------------------------------
	void follow_mouse(Block* block) {
		v2 mp = ds::input::getMousePosition();
		p2i tmp = map::screen2grid(mp);
		if (tmp.x >= MAX_X - 1) {
			tmp.x = MAX_X - 2;
		}
		if (tmp.y >= MAX_Y - 1) {
			tmp.y = MAX_Y - 2;
		}
		block->position = map::grid2screen(tmp);
	}

}
