#pragma once
#include <renderer\render_types.h>
#include "..\Common.h"

enum LaserState {
	LS_RUNNING,
	LS_WARMING_UP,
	LS_IDLE
};

struct Laser {

	ds::Color color;
	float timer;
	int column;
	int texture;
	LaserState state;
	ds::SpriteSheet* spriteSheet;

	Laser() : color(ds::Color::WHITE), timer(0.0f), column(-1), texture(-1), state(LS_IDLE), spriteSheet(0) {}
};

namespace laser {

	void init(Laser* laser);

	void reset(Laser* laser, float startDelay);

	bool move(Laser* laser, float stepDelay, float startDelay, float dt, int* column);

	bool tick(Laser* laser, float dt);

	void start(Laser* laser, float startDelay);

	void render(Laser* laser);

}
