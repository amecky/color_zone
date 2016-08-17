#pragma once
#include <renderer\render_types.h>
#include "..\Common.h"

// -----------------------------------------------------------------
//
// -----------------------------------------------------------------
class Laser {

enum LaserState {
	LS_RUNNING,
	LS_WARMING_UP,
	LS_IDLE
};

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
};

