#pragma once
#include <renderer\render_types.h>
#include "..\utils\DynamicGameSettings.h"

class Laser {

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
	bool _warmUp;
	bool _active;
	float _timer;
	int _column;
	ds::Texture _texture;
	GameSettings* _settings;
};

