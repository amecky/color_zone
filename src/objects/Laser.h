#pragma once
#include "..\Common.h"


struct LaserState {

	enum Enum {
		LS_RUNNING,
		LS_WARMING_UP,
		LS_IDLE,
		LS_MOVING
	};

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
	ds::vec4 _texture;
	LaserState::Enum _state;
};
