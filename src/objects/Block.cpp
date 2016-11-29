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

}

Block::Block() {
	_position = p2i(512, 384);
	ds::SpriteSheet* spriteSheet = ds::res::getSpriteSheet("spritesheet");
	_texture = spriteSheet->findIndex(SID("block"));
	_boxTexture = spriteSheet->findIndex(SID("block_box"));
	for (int i = 0; i < 4; ++i) {
		_colors[i] = 0;
	}
	pick_colors();
	_rotating = false;
	_rotationRadius = length(v2(HALF_SQUARE_SIZE, HALF_SQUARE_SIZE));
	_scale = v2(1, 1);
	_flashing = false;
}
// -----------------------------------------------------------------
// pick new colors
// -----------------------------------------------------------------
void Block::pick_colors() {
	int firstColor = math::random(0, 3);
	int secondColor = math::random(0, 3);
	_colors[0] = firstColor;
	_colors[1] = firstColor;
	_colors[2] = secondColor;
	_colors[3] = secondColor;
	int r = math::random(0, 6);
	for (int i = 0; i < r; ++i) {
		block::shift_array(_colors, 4);
	}
}

int Block::getColor(int idx) const {
	return _colors[idx];
}
// -----------------------------------------------------------------
// copy colors from source block to destination block
// -----------------------------------------------------------------
void Block::copy_colors(const Block* src) {
	for (int i = 0; i < 4; ++i) {
		_colors[i] = src->getColor(i);
	}
}

// -----------------------------------------------------------------
// flash scale
// -----------------------------------------------------------------
void Block::flash_scale(float dt, float flashTTL) {
	if (_flashing) {
		_flashTimer += dt;
		if (_flashTimer >= flashTTL) {
			_flashing = false;
		}
		else {
			float norm = 0.2f + _flashTimer / flashTTL * 0.8f;
			_scale = v2(norm, norm);
		}
	}
}

// -----------------------------------------------------------------
// render block
// -----------------------------------------------------------------
void Block::render(ds::Color* colors) {
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	ds::SpriteSheet* spriteSheet = ds::res::getSpriteSheet("spritesheet");
	v2 p = v2(_position.x, _position.y) + v2(HALF_SQUARE_SIZE, HALF_SQUARE_SIZE);
	float norm = _rotationTimer / ROTATION_TIME;
	ds::Texture t = spriteSheet->get(_texture);
	for (int i = 0; i < 4; ++i) {
		float angle = STARTING_ANGLES[i];
		if (_rotating) {
			angle += norm * HALF_PI;
		}
		v2 pp = ds::math::getDistantPosition(p, angle, _rotationRadius);
		sprites->draw(pp, t, angle + DEGTORAD(45.0f), _scale, colors[_colors[i]]);
	}
}

// -----------------------------------------------------------------
// render block and a box surrounding it
// -----------------------------------------------------------------
void Block::render_boxed(ds::Color* colors) {
	render(colors);
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	float r = 0.0f;
	ds::SpriteSheet* spriteSheet = ds::res::getSpriteSheet("spritesheet");
	v2 p = v2(_position.x, _position.y) + v2(HALF_SQUARE_SIZE, HALF_SQUARE_SIZE);
	ds::Texture bt = spriteSheet->get(_boxTexture);
	if (_rotating) {
		float norm = _rotationTimer / ROTATION_TIME;
		r = norm * HALF_PI;
	}
	sprites->draw(p, bt, r);
}

// -----------------------------------------------------------------
// start rotating
// -----------------------------------------------------------------
void Block::start_rotating() {
	if (!_rotating) {
		_rotating = true;
		_rotationTimer = 0.0f;
	}
}

// -----------------------------------------------------------------
// update
// -----------------------------------------------------------------
void Block::update(float dt) {
	if (_rotating) {
		_rotationTimer += dt;
		float norm = _rotationTimer / ROTATION_TIME;
		if (norm >= 1.0f) {
			block::shift_array(_colors, 4);
			_rotating = false;
		}
	}
}

// -----------------------------------------------------------------
// follow mouse
// -----------------------------------------------------------------
void Block::follow_mouse() {
	v2 mp = ds::input::getMousePosition();
	p2i tmp = map::screen2grid(mp);
	if (tmp.x >= MAX_X - 1) {
		tmp.x = MAX_X - 2;
	}
	if (tmp.y >= MAX_Y - 1) {
		tmp.y = MAX_Y - 2;
	}
	_position = map::grid2screen(tmp);
}

void Block::setPosition(const p2i& pos) {
	_position = pos;
}

void Block::startFlashing() {
	_flashing = true;
	_flashTimer = 0.0f;
}