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

class GameContext;
class SpriteBatchBuffer;

class Laser {

public:
	Laser(GameContext* ctx);
	void reset();
	void start();
	void render(SpriteBatchBuffer* buffer);
	bool tick(float dt);
	bool move(float dt);
	void flash(float dt);
	int getIdleSeconds() const {
		return static_cast<int>(_timer) + 1;
	}
	int getColumn() const {
		return _column;
	}
private:
	LaserState::Enum _state;
	float _timer;
	float _xPos;
	bool _active;
	int _column;
	float _alphaTimer;
	int _idleSeconds;
	float _alpha;
	GameContext* _ctx;
};
