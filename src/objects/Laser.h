#pragma once
#include "..\Common.h"

enum LaserState {
	LS_RUNNING,
	LS_WARMING_UP,
	LS_IDLE
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
	LaserState getState() const;
private:
	GameContext* _ctx;
	ds::Color _color;
	float _timer;
	int _column;
	ds::vec4 _texture;
	LaserState _state;
};
