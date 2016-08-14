#pragma once
#include <Vector.h>
#include <renderer\render_types.h>
#include "Common.h"
#include <stdint.h>
#include <lib\BlockArray.h>

class SparkleEffect {

public:
	SparkleEffect(GameContext* context);
	virtual ~SparkleEffect();
	void start(const p2i& pos, int color);	
	virtual void update(float dt);
	virtual void render();
	void reset() {
		_sparkles.size = 0;
	}
protected:
	void allocate(int sz);
	GameContext* _context;
	ds::Texture _texture;
	BlockArray _sparkles;
	v2* _positions;
	v2* _velocities;
	v2* _accelerations;
	float* _timers;
	float* _rotations;
	v2* _scales;
	int* _colors;
};


class IntroEffect : public SparkleEffect {

public:
	IntroEffect(GameContext* context);
	~IntroEffect() {}
	void start(int x, int y, const ds::Rect& r, int stepX, int stepY, float gap);
	virtual void update(float dt);
	bool isRunning() {
		return _running;
	}
	virtual void render();
private:
	ds::Texture _texture;
	float _warmUpTimer;
	bool _running;
};
