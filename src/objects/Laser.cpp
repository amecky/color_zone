#include "Laser.h"
#include "..\TileMap.h"
#include <core\log\Log.h>
#include <resources\ResourceContainer.h>

Laser::Laser() : _color(ds::Color::WHITE), _timer(0.0f), _column(-1), _texture(-1), _state(LS_IDLE), _spriteSheet(0) {
	_spriteSheet = ds::res::getSpriteSheet("spritesheet");
	_texture = _spriteSheet->findIndex(SID("laser"));
	_color = ds::Color(0, 210, 210);
}

// -----------------------------------------------------------------
// reset
// -----------------------------------------------------------------
void Laser::reset(float startDelay) {
	LOG << "Laser reset";
	_state = LS_IDLE;
	_timer = startDelay;
}

// -----------------------------------------------------------------
// move
// -----------------------------------------------------------------
bool Laser::move(float stepDelay, float startDelay, float dt, int* column) {
	*column = -1;
	bool ret = false;
	if (_state == LS_RUNNING) {
		_timer += dt;
		float c = 0.7f + sin(_timer / stepDelay * TWO_PI) * 0.3f;
		_color.a = c;
		if (_timer > stepDelay) {
			++_column;
			if (_column >= 0 && _column < MAX_X) {
				*column = _column;
				ret = true;
			}
			_timer = 0.0f;
			if (_column >= MAX_X) {
				LOG << "Laser state set to warming up";
				_state = LS_WARMING_UP;
				_timer = startDelay;
			}
		}
	}
	return ret;
}

// -----------------------------------------------------------------
// tick
// -----------------------------------------------------------------
bool Laser::tick(float dt) {
	if (_state == LS_WARMING_UP) {
		_timer -= dt;
		if (_timer <= 0.0f) {
			LOG << "Laser running";
			_timer = 0.0f;
			_state = LS_RUNNING;
			_column = -1;
			return true;
		}
	}
	return false;
}

// -----------------------------------------------------------------
// start
// -----------------------------------------------------------------
void Laser::start(float startDelay) {
	LOG << "starting laser";
	_state = LS_WARMING_UP;
	_timer = startDelay;
	_column = -1;
}

// -----------------------------------------------------------------
// render
// -----------------------------------------------------------------
void Laser::render() {
	if (_state == LS_RUNNING) {
		ds::Texture t = _spriteSheet->get(_texture);
		ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
		for (int i = 0; i < MAX_Y; ++i) {
			p2i tmp = map::screen2grid(v2(_column, i));
			p2i converted = map::grid2screen(p2i(_column, i));
			sprites->draw(converted, t, 0.0f, v2(1, 1), _color);
		}
	}
}

// -----------------------------------------------------------------
// is running
// -----------------------------------------------------------------
bool Laser::isRunning() const {
	return _state == LS_RUNNING;
}

// -----------------------------------------------------------------
// get idle seconds
// -----------------------------------------------------------------
int Laser::getIdleSeconds() const {
	return static_cast<int>(_timer);
}

// -----------------------------------------------------------------
// get state
// -----------------------------------------------------------------
LaserState Laser::getState() const {
	return _state;
}