#pragma once
#include <renderer\render_types.h>
#include "..\utils\GameSettings.h"

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
	Laser(GameSettings* settings);
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
	ds::Texture _texture;
	GameSettings* _settings;
	LaserState _state;
};

