#include "Laser.h"
#include <math\math.h>
#include "..\Constants.h"
#include <renderer\sprites.h>
#include "..\TileMap.h"
#include <utils\Log.h>
#include <resources\ResourceContainer.h>

namespace laser {

	// -----------------------------------------------------------------
	// intialize
	// -----------------------------------------------------------------
	void init(MyLaser* laser) {
		laser->state = LS_IDLE;
		laser->timer = 0.0f;
		laser->column = 0;
		laser->spriteSheet = ds::res::getSpriteSheet("spritesheet");
		laser->texture = laser->spriteSheet->findIndex("laser");
		laser->color = ds::Color(0, 210, 210);
	}

	// -----------------------------------------------------------------
	// reset
	// -----------------------------------------------------------------
	void reset(MyLaser* laser, float startDelay) {
		LOG << "Laser reset";
		laser->state = LS_IDLE;
		laser->timer = startDelay;
	}

	// -----------------------------------------------------------------
	// move
	// -----------------------------------------------------------------
	bool move(MyLaser* laser, float stepDelay, float startDelay, float dt, int* column) {
		*column = -1;
		bool ret = false;
		if (laser->state == LS_RUNNING) {
			laser->timer += dt;
			float c = 0.7f + sin(laser->timer / stepDelay * TWO_PI) * 0.3f;
			//_color.r = 0.0f;
			laser->color.a = c;
			//_color.b = 0.0f;
			if (laser->timer > stepDelay) {
				++laser->column;
				if (laser->column >= 0 && laser->column < MAX_X) {
					*column = laser->column;
					ret = true;
				}
				laser->timer = 0.0f;
				if (laser->column >= MAX_X) {
					LOG << "Laser state set to warming up";
					laser->state = LS_WARMING_UP;
					laser->timer = startDelay;
				}
			}
		}
		return ret;
	}

	// -----------------------------------------------------------------
	// tick
	// -----------------------------------------------------------------
	void tick(MyLaser* laser, float dt) {
		if (laser->state == LS_WARMING_UP) {
			laser->timer -= dt;
			if (laser->timer <= 0.0f) {
				LOG << "Laser running";
				laser->timer = 0.0f;
				laser->state = LS_RUNNING;
				laser->column = -1;
			}
		}
	}

	// -----------------------------------------------------------------
	// start
	// -----------------------------------------------------------------
	void start(MyLaser* laser, float startDelay) {
		LOG << "starting laser";
		laser->state = LS_WARMING_UP;
		laser->timer = startDelay;
		laser->column = -1;
	}

	// -----------------------------------------------------------------
	// render
	// -----------------------------------------------------------------
	void render(MyLaser* laser) {
		if (laser->state == LS_RUNNING) {
			ds::Texture t = laser->spriteSheet->get(laser->texture);
			ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
			for (int i = 0; i < MAX_Y; ++i) {
				p2i tmp = map::screen2grid(v2(laser->column, i));
				p2i converted = map::grid2screen(p2i(laser->column, i));
				sprites->draw(converted, t, 0.0f, v2(1, 1), laser->color);
			}
		}
	}

}

Laser::Laser(GameContext* context) : _context(context) {
	_state = LS_IDLE;
	_timer = 0.0f;
	_column = 0;
	_spriteSheet = ds::res::getSpriteSheet("spritesheet");
	_texture = _spriteSheet->findIndex("laser");
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
	_timer = _context->settings->laserStartDelay;
}

// -----------------------------------------------------------------
// move
// -----------------------------------------------------------------
bool Laser::move(float dt, int* column) {
	*column = -1;
	bool ret = false;
	if (_state == LS_RUNNING) {
		_timer += dt;
		float c = 0.7f + sin(_timer / _context->settings->laserStepDelay * TWO_PI) * 0.3f;
		//_color.r = 0.0f;
		_color.a = c;
		//_color.b = 0.0f;
		if (_timer > _context->settings->laserStepDelay) {
			++_column;
			if (_column >= 0 && _column < MAX_X) {
				*column = _column;
				ret = true;
			}
			_timer = 0.0f;
			if (_column >= MAX_X) {
				LOG << "Laser state set to warming up";
				_state = LS_WARMING_UP;
				_timer = _context->settings->laserStartDelay;
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
	_timer = _context->settings->laserStartDelay;
	_column = -1;
}

// -----------------------------------------------------------------
// render
// -----------------------------------------------------------------
void Laser::render() {
	if (_state == LS_RUNNING) {
		ds::Texture t = _context->spriteSheet->get(_texture);
		ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
		for (int i = 0; i < MAX_Y; ++i) {
			p2i tmp = map::screen2grid(v2(_column,i));
			p2i converted = map::grid2screen(p2i(_column, i));
			sprites->draw(converted, t, 0.0f, v2(1, 1), _color);
		}
	}
}