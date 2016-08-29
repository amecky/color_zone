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
	void init(Laser* laser) {
		laser->state = LS_IDLE;
		laser->timer = 0.0f;
		laser->column = 0;
		laser->spriteSheet = ds::res::getSpriteSheet("spritesheet");
		laser->texture = laser->spriteSheet->findIndex(SID("laser"));
		laser->color = ds::Color(0, 210, 210);
	}

	// -----------------------------------------------------------------
	// reset
	// -----------------------------------------------------------------
	void reset(Laser* laser, float startDelay) {
		LOG << "Laser reset";
		laser->state = LS_IDLE;
		laser->timer = startDelay;
	}

	// -----------------------------------------------------------------
	// move
	// -----------------------------------------------------------------
	bool move(Laser* laser, float stepDelay, float startDelay, float dt, int* column) {
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
	bool tick(Laser* laser, float dt) {
		if (laser->state == LS_WARMING_UP) {
			laser->timer -= dt;
			if (laser->timer <= 0.0f) {
				LOG << "Laser running";
				laser->timer = 0.0f;
				laser->state = LS_RUNNING;
				laser->column = -1;
				return true;
			}
		}
		return false;
	}

	// -----------------------------------------------------------------
	// start
	// -----------------------------------------------------------------
	void start(Laser* laser, float startDelay) {
		LOG << "starting laser";
		laser->state = LS_WARMING_UP;
		laser->timer = startDelay;
		laser->column = -1;
	}

	// -----------------------------------------------------------------
	// render
	// -----------------------------------------------------------------
	void render(Laser* laser) {
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
