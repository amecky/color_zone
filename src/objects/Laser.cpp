#include "Laser.h"
#include <SpriteBatchBuffer.h>
#include "..\Common.h"
#include "..\GameSettings.h"
#include <ds_imgui.h>
#include "..\TileMap.h"

void initialize_laser(Laser* laser) {
	laser->active = true;
	laser->alphaTimer = 0.0f;
	laser->column = 0;
	laser->state = LaserState::LS_IDLE;
	laser->timer = 0.0f;
	laser->xPos = 0;
	laser->alpha = 1.0f;
	laser->idleSeconds = 10;
}

// -----------------------------------------------------------------
// reset
// -----------------------------------------------------------------
void reset_laser(Laser* laser, float startDelay) {
	laser->state = LaserState::LS_IDLE;
	laser->timer = startDelay;
	laser->alphaTimer = 0.0f;
	laser->active = false;
	laser->alpha = 1.0f;
}

void flash_laser(Laser* laser, float dt, float minAlpha, float maxAlpha, float ttl) {
	if (laser->state == LaserState::LS_WAITING || laser->state == LaserState::LS_MOVING) {
		laser->alphaTimer += dt;
		float c = minAlpha + abs(sin(laser->alphaTimer / ttl * ds::TWO_PI)) * (maxAlpha - minAlpha);
		if (laser->alphaTimer > ttl) {
			laser->alphaTimer -= ttl;
		}
		laser->alpha = c;
	}
}

bool move_laser(Laser* laser, float dt, float startDelay, float stepDelay, float moveDelay) {
	bool ret = false;
	if (laser->state == LaserState::LS_WAITING) {
		laser->timer += dt;
		if (laser->timer > stepDelay) {
			laser->timer -= stepDelay;
			laser->state = LaserState::LS_MOVING;
		}
	}

	if (laser->state == LaserState::LS_MOVING) {
		laser->timer += dt;
		float vel = static_cast<float>(SQUARE_SIZE ) / moveDelay;
		laser->xPos += vel * dt;
		if (laser->timer >= moveDelay) {
			++laser->column;
			if (laser->column >= 0 && laser->column < MAX_X) {
				ret = true;
			}
			laser->timer -= moveDelay;
			laser->state = LaserState::LS_WAITING;

			if (laser->column >= MAX_X) {
				laser->state = LaserState::LS_WARMING_UP;
				laser->timer = startDelay;
				laser->active = false;
			}
		}
	}
	return ret;
}

// -----------------------------------------------------------------
// tick
// -----------------------------------------------------------------
bool tick_laser(Laser* laser, float dt) {
	if (laser->state == LaserState::LS_WARMING_UP) {
		laser->timer -= dt;
		if (laser->timer <= 0.0f) {
			laser->timer = 0.0f;
			laser->state = LaserState::LS_MOVING;
			laser->column = 0;
			laser->xPos = START_X;
			return true;
		}
	}
	return false;
}

// -----------------------------------------------------------------
// start
// -----------------------------------------------------------------
void start_laser(Laser* laser, float startDelay) {
	laser->state = LaserState::LS_WARMING_UP;
	laser->timer = startDelay;
	laser->column = 0;
	laser->alphaTimer = 0.0f;
	laser->active = true;
	laser->xPos = START_X;
}

// -----------------------------------------------------------------
// render
// -----------------------------------------------------------------
void render_laser(Laser* laser, SpriteBatchBuffer* buffer) {
	if (laser->active) {
		for (int i = 0; i < MAX_Y; ++i) {
			p2i converted = map::grid2screen(p2i(laser->column, i));
			ds::vec2 v((int)laser->xPos - 18, converted.y);
			int alpha = 255.0f * laser->alpha;
			buffer->add(v, ds::vec4(375, 0, 36, 36), ds::vec2(1.0f), 0.0f, ds::Color(0, 210, 210, alpha));
		}
	}
}

