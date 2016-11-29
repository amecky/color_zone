#pragma once
#include <renderer\render_types.h>
#include "..\Common.h"

enum LaserState {
	LS_RUNNING,
	LS_WARMING_UP,
	LS_IDLE
};

class Laser {

public:
	Laser();
	~Laser() {}
	void reset(float startDelay);
	bool move(float stepDelay, float startDelay, float dt, int* column);
	bool tick(float dt);
	void start(float startDelay);
	void render();
	bool isRunning() const;
	int getIdleSeconds() const;
	LaserState getState() const;
private:
	ds::Color _color;
	float _timer;
	int _column;
	int _texture;
	LaserState _state;
	ds::SpriteSheet* _spriteSheet;
};
