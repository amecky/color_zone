#include "Laser.h"
#include "..\TileMap.h"
#include <SpriteBatchBuffer.h>
#include "..\Common.h"
#include "..\GameSettings.h"




Laser::Laser(GameContext* ctx) : _ctx(ctx), _color(ds::Color(255,255,255,255)), _timer(0.0f), _column(-1), _state(LaserState::LS_IDLE) {
	_color = ds::Color(0, 210, 210, 255);
	for (int i = 0; i < MAX_Y; ++i) {
		p2i tmp = map::screen2grid(ds::vec2(0, i));
		p2i converted = map::grid2screen(p2i(0, i));
		_laser.sprites[i] = Sprite(ds::vec2(0,converted.y), ds::vec4(375, 0, 36, 36), ds::vec2(1.0f), 0.0f, ds::Color(0, 210, 210, 255));
	}
	_laser.active = false;
}

// -----------------------------------------------------------------
// reset
// -----------------------------------------------------------------
void Laser::reset() {
	_state = LaserState::LS_IDLE;
	_timer = _ctx->settings->laser.startDelay;
	_alphaTimer = 0.0f;
	_laser.active = false;
}

// -----------------------------------------------------------------
// move
// -----------------------------------------------------------------
bool Laser::move(float dt, int* column) {
	float stepDelay = _ctx->settings->laser.waitDelay;
	float moveDelay = _ctx->settings->laser.moveDelay;
	float startDelay = _ctx->settings->laser.startDelay;
	*column = _column;
	bool ret = false;

	if (_state == LaserState::LS_WAITING || _state == LaserState::LS_MOVING) {
		_alphaTimer += dt;
		float c = _ctx->settings->laser.minAlpha + abs(sin(_alphaTimer / _ctx->settings->laser.alphaTTL * ds::TWO_PI)) * (_ctx->settings->laser.maxAlpha - _ctx->settings->laser.minAlpha);
		if (_alphaTimer > _ctx->settings->laser.alphaTTL) {
			_alphaTimer -= _ctx->settings->laser.alphaTTL;
		}
		_color.a = c;
	}

	if (_state == LaserState::LS_WAITING) {
		_timer += dt;
		if (_timer > stepDelay) {
			_timer -= stepDelay;
			_state = LaserState::LS_MOVING;
		}
	}

	if (_state == LaserState::LS_MOVING) {
		_timer += dt;
		float vel = static_cast<float>(SQUARE_SIZE ) / moveDelay;
		_laser.xPos += vel * dt;
		if (_timer >= moveDelay) {
			++_column;
			if (_column >= 0 && _column < MAX_X) {
				*column = _column;
				ret = true;
			}
			_timer -= moveDelay;
			_state = LaserState::LS_WAITING;

			if (_column >= MAX_X) {
				_state = LaserState::LS_WARMING_UP;
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
	if (_state == LaserState::LS_WARMING_UP) {
		_timer -= dt;
		if (_timer <= 0.0f) {
			_timer = 0.0f;
			_state = LaserState::LS_MOVING;
			_column = -1;
			_laser.xPos = START_X;
			return true;
		}
	}
	return false;
}

// -----------------------------------------------------------------
// start
// -----------------------------------------------------------------
void Laser::start() {
	_state = LaserState::LS_WARMING_UP;
	_timer = _ctx->settings->laser.startDelay;
	_column = -1;
	_alphaTimer = 0.0f;
	_laser.active = true;
	_laser.xPos = START_X;
}

// -----------------------------------------------------------------
// render
// -----------------------------------------------------------------
void Laser::render() {
	if (_laser.active) {
		for (int i = 0; i < MAX_Y; ++i) {
			_laser.sprites[i].position.x = _laser.xPos;
			_laser.sprites[i].color = _color;
		}

		_ctx->buffer->add(_laser.sprites, MAX_Y);
	}
}

// -----------------------------------------------------------------
// is running
// -----------------------------------------------------------------
bool Laser::isRunning() const {
	return _laser.active;
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
LaserState::Enum Laser::getState() const {
	return _state;
}