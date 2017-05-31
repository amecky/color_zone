#include "Laser.h"
#include "..\TileMap.h"
#include <SpriteBatchBuffer.h>
#include "..\Common.h"
#include "..\GameSettings.h"

Laser::Laser(GameContext* ctx) : _ctx(ctx), _color(ds::Color(255,255,255,255)), _timer(0.0f), _column(-1), _texture(-1), _state(LS_IDLE) {
	_texture = ds::vec4(322, 0, 36, 36);
	_color = ds::Color(0, 210, 210);
}

// -----------------------------------------------------------------
// reset
// -----------------------------------------------------------------
void Laser::reset() {
	_state = LS_IDLE;
	_timer = _ctx->settings->laser.startDelay;
}

// -----------------------------------------------------------------
// move
// -----------------------------------------------------------------
bool Laser::move(float dt, int* column) {
	float stepDelay = _ctx->settings->laser.stepDelay;
	float startDelay = _ctx->settings->laser.startDelay;
	*column = -1;
	bool ret = false;
	if (_state == LS_RUNNING) {
		_timer += dt;
		float c = 0.7f + sin(_timer / stepDelay * ds::TWO_PI) * 0.3f;
		_color.a = c;
		if (_timer > stepDelay) {
			++_column;
			if (_column >= 0 && _column < MAX_X) {
				*column = _column;
				ret = true;
			}
			_timer = 0.0f;
			if (_column >= MAX_X) {
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
void Laser::start() {
	_state = LS_WARMING_UP;
	_timer = _ctx->settings->laser.startDelay;
	_column = -1;
}

// -----------------------------------------------------------------
// render
// -----------------------------------------------------------------
void Laser::render() {
	if (_state == LS_RUNNING) {
		for (int i = 0; i < MAX_Y; ++i) {
			p2i tmp = map::screen2grid(ds::vec2(_column, i));
			p2i converted = map::grid2screen(p2i(_column, i));
			_ctx->buffer->add(ds::vec2(converted.x,converted.y), _texture, ds::vec2(1, 1), 0.0f, _color);
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