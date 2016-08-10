#include "Laser.h"
#include <math\math.h>
#include "..\Constants.h"
#include <renderer\sprites.h>
#include "..\TileMap.h"
#include <utils\Log.h>

Laser::Laser(GameSettings* settings) : _settings(settings) {
	_state = LS_IDLE;
	_timer = 0.0f;
	_column = 0;
	_texture = math::buildTexture(132, 260, 36, 36);
	_color = ds::Color(0, 210, 210);
}

Laser::~Laser() {
}

// -----------------------------------------------------------------
// reset
// -----------------------------------------------------------------
void Laser::reset() {
	LOG << "Laser reset";
	_state = LS_IDLE;
	_timer = _settings->laserStartDelay;
}

// -----------------------------------------------------------------
// move
// -----------------------------------------------------------------
bool Laser::move(float dt, int* column) {
	*column = -1;
	bool ret = false;
	if (_state == LS_RUNNING) {
		_timer += dt;
		float c = 0.7f + sin(_timer / _settings->laserStepDelay * TWO_PI) * 0.3f;
		//_color.r = 0.0f;
		_color.a = c;
		//_color.b = 0.0f;
		if (_timer > _settings->laserStepDelay) {
			++_column;
			if (_column >= 0 && _column < MAX_X) {
				*column = _column;
				ret = true;
			}
			_timer = 0.0f;
			if (_column >= MAX_X) {
				LOG << "Laser state set to warming up";
				_state = LS_WARMING_UP;
				_timer = _settings->laserStartDelay;
			}
		}
	}
	return ret;
}

// -----------------------------------------------------------------
// tick
// -----------------------------------------------------------------
void Laser::tick(float dt) {
	if (_state == LS_WARMING_UP) {
		_timer -= dt;
		if (_timer <= 0.0f) {
			LOG << "Laser running";
			_timer = 0.0f;
			_state = LS_RUNNING;
			_column = -1;
		}
	}	
}

// -----------------------------------------------------------------
// start
// -----------------------------------------------------------------
void Laser::start() {
	LOG << "starting laser";
	_state = LS_WARMING_UP;
	_timer = _settings->laserStartDelay;
	_column = -1;
}

// -----------------------------------------------------------------
// render
// -----------------------------------------------------------------
void Laser::render() {
	if (_state == LS_RUNNING) {
		ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
		for (int i = 0; i < MAX_Y; ++i) {
			sprites->draw(map::convert(_column, i), _texture,0.0f,v2(1,1),_color);
		}
	}
}