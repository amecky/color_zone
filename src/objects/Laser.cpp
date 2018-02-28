#include "Laser.h"
#include <SpriteBatchBuffer.h>
#include "..\Common.h"
#include "..\GameSettings.h"
#include <ds_imgui.h>
#include "..\TileMap.h"

Laser::Laser(GameContext* ctx) {
	_ctx = ctx;
	_active = true;
	_alphaTimer = 0.0f;
	_column = 0;
	_state = LaserState::LS_IDLE;
	_timer = 0.0f;
	_xPos = 0;
	_alpha = 1.0f;
	_idleSeconds = 10;
}

// -----------------------------------------------------------------
// reset
// -----------------------------------------------------------------
void Laser::reset() {
	_state = LaserState::LS_IDLE;
	_timer = _ctx->settings->laser.startDelay;
	_alphaTimer = 0.0f;
	_active = false;
	_alpha = 1.0f;
}

void Laser::flash(float dt) {
	if (_state == LaserState::LS_WAITING || _state == LaserState::LS_MOVING) {
		_alphaTimer += dt;
		float c = _ctx->settings->laser.minAlpha + fabs(sin(_alphaTimer / _ctx->settings->laser.alphaTTL * ds::TWO_PI)) * (_ctx->settings->laser.maxAlpha - _ctx->settings->laser.minAlpha);
		if (_alphaTimer > _ctx->settings->laser.alphaTTL) {
			_alphaTimer -= _ctx->settings->laser.alphaTTL;
		}
		_alpha = c;
	}
}

bool Laser::move(float dt) {
	bool ret = false;
	if (_state == LaserState::LS_WAITING) {
		_timer += dt;
		if (_timer > _ctx->settings->laser.waitDelay) {
			_timer -= _ctx->settings->laser.waitDelay;
			_state = LaserState::LS_MOVING;
		}
	}

	if (_state == LaserState::LS_MOVING) {
		_timer += dt;
		float vel = static_cast<float>(SQUARE_SIZE ) / _ctx->settings->laser.moveDelay;
		_xPos += vel * dt;
		if (_timer >= _ctx->settings->laser.moveDelay) {
			++_column;
			if (_column >= 0 && _column < (MAX_X + 1)) {
				ret = true;
			}
			_timer -= _ctx->settings->laser.moveDelay;
			_state = LaserState::LS_WAITING;

			if (_column >= MAX_X) {
				_state = LaserState::LS_WARMING_UP;
				_timer = _ctx->settings->laser.startDelay;
				_active = false;
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
			_column = 0;
			_xPos = START_X;
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
	_column = 0;
	_alphaTimer = 0.0f;
	_active = true;
	_xPos = START_X;
}

// -----------------------------------------------------------------
// render
// -----------------------------------------------------------------
void Laser::render(SpriteBatchBuffer* buffer) {
	if (_state != LaserState::LS_WARMING_UP && _state != LaserState::LS_IDLE) {
		for (int i = 0; i < MAX_Y; ++i) {
			p2i converted = map::grid2screen(p2i(_column, i));
			ds::vec2 v((int)_xPos - 18, converted.y);
			int alpha = 255.0f * _alpha;
			buffer->add(v, ds::vec4(375, 0, 36, 36), ds::vec2(1.0f), 0.0f, ds::Color(0, 210, 210, alpha));
		}
	}
}
