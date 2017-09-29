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

Block::Block() {
	_position = p2i(512, 384);
	for (int i = 0; i < 4; ++i) {
		_colors[i] = 0;
	}
	pick_colors();
	_rotating = false;
	_rotationRadius = length(ds::vec2(HALF_SQUARE_SIZE, HALF_SQUARE_SIZE));
	_scale = ds::vec2(1, 1);
	_flashing = false;
}
// -----------------------------------------------------------------
// pick new colors
// -----------------------------------------------------------------
void Block::pick_colors() {
	int firstColor = ds::random(0, 3);
	int secondColor = ds::random(0, 3);
	_colors[0] = firstColor;
	_colors[1] = firstColor;
	_colors[2] = secondColor;
	_colors[3] = secondColor;
	int r = ds::random(0, 6);
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
			_scale = ds::vec2(norm, norm);
		}
	}
}

// -----------------------------------------------------------------
// render block
// -----------------------------------------------------------------
void Block::render(SpriteBatchBuffer* buffer,ds::Color* colors) {
	ds::vec2 p = ds::vec2(_position.x, _position.y) + ds::vec2(HALF_SQUARE_SIZE, HALF_SQUARE_SIZE);
	float norm = _rotationTimer / ROTATION_TIME;
	ds::vec4 tex(36, 0, 36, 36);
	for (int i = 0; i < 4; ++i) {
		float angle = STARTING_ANGLES[i];
		if (_rotating) {
			angle += norm * ds::PI * 0.5f;
		}
		ds::vec2 pp = ds::vec2(p.x + _rotationRadius * cos(angle), p.y + _rotationRadius * sin(angle));
		buffer->add(pp, tex, _scale, angle + DEGTORAD(45.0f), colors[_colors[i]]);
	}
}

// -----------------------------------------------------------------
// render block and a box surrounding it
// -----------------------------------------------------------------
void Block::render_boxed(SpriteBatchBuffer* buffer, ds::Color* colors) {
	render(buffer, colors);
	float r = 0.0f;
	ds::vec2 p = ds::vec2(_position.x, _position.y) + ds::vec2(HALF_SQUARE_SIZE, HALF_SQUARE_SIZE);
	ds::vec4 t(220, 140, 78, 78);
	if (_rotating) {
		float norm = _rotationTimer / ROTATION_TIME;
		r = norm * ds::PI * 0.5f;
	}
	buffer->add(p, t, ds::vec2(1, 1), r, { 50,208,253,255 });
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
	ds::vec2 mp = ds::getMousePosition();
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