#pragma once
#include "..\Common.h"
#include <SpriteBatchBuffer.h>

struct LaserState {

	enum Enum {
		LS_RUNNING,
		LS_WARMING_UP,
		LS_IDLE,
		LS_MOVING,
		LS_WAITING
	};

};

struct MyLaser {
	LaserState::Enum state;
	Sprite sprites[MAX_Y];
	float timer;
	float xPos;
	bool active;
};

class Laser {

public:
	Laser(GameContext* ctx);
	~Laser() {}
	void reset();
	bool move(float dt, int* column);
	bool tick(float dt);
	void start();
	void render();
	bool isRunning() const;
	int getIdleSeconds() const;
	LaserState::Enum getState() const;
private:
	GameContext* _ctx;
	ds::Color _color;
	float _timer;
	float _alphaTimer;
	int _column;
	LaserState::Enum _state;

	MyLaser _laser;
};
