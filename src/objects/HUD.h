#pragma once
#include <renderer\render_types.h>
#include "..\utils\GameSettings.h"
#include <utils\GameTimer.h>

struct NumberDef {
	int width;
	int offset;

	NumberDef() : width(0), offset(0) {}
	NumberDef(int o, int w) : width(w), offset(o) {}
};

class HUD {

public:
	HUD(GameSettings* settings);
	~HUD();
	void tick(float dt);
	void render();
	const ds::GameTimer& getTimer() const {
		return _timer;
	}
	void start();
	void setScore(int scr);
	void setCoverage(int fillRate);
private:
	void convert(int value, int* array, int length);
	void tickTimer(float dt);
	ds::GameTimer _timer;
	int _score;
	int _score_digits[6];
	ds::Texture _textures[10];
	GameSettings* _settings;
	int _coverage_digits[3];
	int _coverage;
	ds::Texture _perTexture;
	ds::Texture _delimiterTexture;
	int _time_digits[4];
};

