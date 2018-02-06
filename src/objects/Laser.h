#pragma once

struct LaserState {

	enum Enum {
		LS_RUNNING,
		LS_WARMING_UP,
		LS_IDLE,
		LS_MOVING,
		LS_WAITING
	};

};

struct Laser {
	LaserState::Enum state;
	float timer;
	float xPos;
	bool active;
	int column;
	float alphaTimer;
	int idleSeconds;
	float alpha;
};

class SpriteBatchBuffer;

void initialize_laser(Laser* laser);

void start_laser(Laser* laser, float startDelay);

void reset_laser(Laser* laser, float startDelay);

bool tick_laser(Laser* laser, float dt);

bool move_laser(Laser* laser, float dt, float startDelay, float stepDelay, float moveDelay);

void flash_laser(Laser* laser, float dt, float minAlpha, float maxAlpha, float ttl);

void render_laser(Laser* laser, SpriteBatchBuffer* buffer);

/*
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
*/