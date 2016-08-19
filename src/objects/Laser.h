#pragma once
#include <renderer\render_types.h>
#include "..\Common.h"

enum LaserState {
	LS_RUNNING,
	LS_WARMING_UP,
	LS_IDLE
};

struct MyLaser {

	ds::Color color;
	float timer;
	int column;
	int texture;
	LaserState state;
	ds::SpriteSheet* spriteSheet;

	MyLaser() : color(ds::Color::WHITE), timer(0.0f), column(-1), texture(-1), state(LS_IDLE), spriteSheet(0) {}
};

namespace laser {

	void init(MyLaser* laser);

	void reset(MyLaser* laser, float startDelay);

	bool move(MyLaser* laser, float stepDelay, float startDelay, float dt, int* column);

	void tick(MyLaser* laser, float dt);

	void start(MyLaser* laser, float startDelay);

	void render(MyLaser* laser);

}
// -----------------------------------------------------------------
//
// -----------------------------------------------------------------
class Laser {



	

public:
	Laser(GameContext* context);
	~Laser();
	void reset();
	bool move(float dt,int* column);
	void tick(float dt);
	void render();
	void start();
private:
	ds::Color _color;
	float _timer;
	int _column;
	int _texture;
	GameContext* _context;
	LaserState _state;
	ds::SpriteSheet* _spriteSheet;
};

