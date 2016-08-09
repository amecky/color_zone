#include "Laser.h"
#include <math\math.h>
#include "..\Constants.h"
#include <renderer\sprites.h>
#include "..\TileMap.h"

Laser::Laser(GameSettings* settings) : _settings(settings) {
	_active = false;
	_timer = 0.0f;
	_column = 0;
	_texture = math::buildTexture(132, 220, 36, 36);
}

Laser::~Laser() {
}

void Laser::reset() {
	_active = false;
	_timer = 1.0f; // FIXME: get from settings
	_warmUp = true;
}

bool Laser::move(float dt, int* column) {
	*column = -1;
	bool ret = false;
	if (_active) {
		_timer += dt;
		float c = 0.7f + sin(_timer / 0.4f * TWO_PI * 1.0f) * 0.3f;
		_color.r = 0.0f;
		_color.g = c;
		_color.b = 0.0f;
		// FIXME: color should be pulsing
		// FIXME: take this one from settings
		if (_timer > 0.4f) {
			++_column;
			if (_column >= 0 && _column < MAX_X) {
				*column = _column;
				ret = true;
			}
			_timer = 0.0f;
			if (_column >= MAX_X) {
				_active = false;
				_timer = 5.0f;
			}
		}
	}
	return ret;
}

void Laser::tick(float dt) {
	if ( !_active) {
		_timer -= dt;
		if (_timer <= 0.0f) {
			start();
		}
	}
}

void Laser::start() {
	_warmUp = true;
	_active = true;
	_timer = 0.0f;
	_column = -1;
}

void Laser::render() {
	if (_active) {
		ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
		if (_active) {
			for (int i = 0; i < MAX_Y; ++i) {
				sprites->draw(map::convert(_column, i), _texture,0.0f,v2(1,1),_color);
			}
		}
	}
}