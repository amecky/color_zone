#pragma once
#include <stdint.h>
#include "..\Common.h"
#include <ds_imgui.h>

class SpriteBatchBuffer;

class SparkleEffect {

public:
	SparkleEffect(GameContext* context);
	virtual ~SparkleEffect();
	void start(const p2i& pos, int color);
	virtual void update(float dt);
	virtual void render(SpriteBatchBuffer* buffer);
	void reset() {
		_sparklesCount = 0;
	}
protected:
	void allocate(int sz);
	GameContext* _ctx;
	ds::vec4 _texture;
	char* _sparkles;
	int _sparklesCapacity;
	int _sparklesCount;
	ds::vec2* _positions;
	ds::vec2* _velocities;
	ds::vec2* _accelerations;
	float* _timers;
	float* _rotations;
	ds::vec2* _scales;
	int* _colors;
};
