#pragma once
#include <Vector.h>
#include <renderer\render_types.h>
#include "Common.h"

struct SparkleArray {

	v2* positions;
	v2* velocities;
	v2* scales;
	float* rotations;
	float* timers;
	ds::Texture* textures;
	char* buffer;

	int count;
	int countAlive;

	SparkleArray() : count(0), countAlive(0), buffer(0) {}

	~SparkleArray() {
		if (buffer != 0) {
			delete[] buffer;
		}
	}

	void initialize(unsigned int maxParticles) {
		int size = maxParticles * (sizeof(v2) * 3 + sizeof(float) * 2 + sizeof(ds::Texture));
		buffer = new char[size];
		positions = (v2*)(buffer);
		velocities = (v2*)(positions + maxParticles);		
		scales = (v2*)(velocities + maxParticles);
		rotations = (float*)(scales + maxParticles);
		timers = (float*)(rotations + maxParticles);		
		textures = (ds::Texture*)(timers + maxParticles);
		count = maxParticles;
		countAlive = 0;
	}

	void swapData(uint32 a, uint32 b) {
		if (a != b) {
			positions[a] = positions[b];
			velocities[a] = velocities[b];
			scales[a] = scales[b];
			rotations[a] = rotations[b];
			timers[a] = timers[b];
			textures[a] = textures[b];
		}
	}

	void kill(uint32 id) {
		if (countAlive > 0) {
			swapData(id, countAlive - 1);
			--countAlive;
		}
	}

	void wake(uint32 id) {
		if (countAlive < count)	{
			swapData(id, countAlive);
			++countAlive;
		}
	}

};

class SparkleEffect {

public:
	SparkleEffect(GameContext* context);
	~SparkleEffect();
	void start(int x, int y, const ds::Rect& r,int pieces);
	void update(float dt);
	void render();
	void reset() {
		_sparkles.countAlive = 0;
	}
private:
	GameContext* _context;
	SparkleArray _sparkles;
	int _startX;
	int _startY;
};
