#include "SparkleEffect.h"
#include <sprites\SpriteBatch.h>
#include "Constants.h"

// --------------------------------------------
// Sparkle effect
// --------------------------------------------
SparkleEffect::SparkleEffect() {
	_sparkles.initialize(4096);
}


SparkleEffect::~SparkleEffect() {
}

// --------------------------------------------
// start block
// --------------------------------------------
void SparkleEffect::start(int xp, int yp, const ds::Rect& r, int pieces) {
	int total = pieces * pieces;
	float dx = r.width() / pieces;
	float dy = r.height() / pieces;
	float gap = 2.0f;
	int start = _sparkles.countAlive;
	int end = start + total;
	v2 np = v2(START_X + xp * SQUARE_SIZE, START_Y + yp * SQUARE_SIZE);
	np.x -= (pieces - 1) * gap + SQUARE_SIZE / 4;
	np.y += (pieces - 1) * gap + SQUARE_SIZE / 4;
	if (end < _sparkles.count) {
		for (int i = 0; i < total; ++i) {
			_sparkles.wake(start + i);
		}
		int cnt = start;
		for (int y = 0; y < pieces; ++y) {
			for (int x = 0; x < pieces; ++x) {
				_sparkles.positions[cnt] = v2(np.x + (dx + gap) * x, np.y - (dy + gap) * y);
				_sparkles.timers[cnt] = 0.0f;
				_sparkles.textures[cnt] = ds::math::buildTexture(r.top + dy * y, r.left + dx * x, dx, dy);
				_sparkles.velocities[cnt] = ds::math::getRadialVelocity(PARTICLE_ANGLES[cnt - start], 20.0f + ds::math::random(-4.0f,4.0f));
				++cnt;
			}
		}
	}
}

// --------------------------------------------
// update
// --------------------------------------------
void SparkleEffect::update(float dt) {
	for (int i = 0; i < _sparkles.countAlive; ++i) {
		_sparkles.timers[i] += dt;
		if (_sparkles.timers[i] > 1.0f) {
			_sparkles.kill(i);
		}
	}
	for (int i = 0; i < _sparkles.countAlive; ++i) {
		_sparkles.positions[i] += _sparkles.velocities[i] * dt;
	}
}

// --------------------------------------------
// render
// --------------------------------------------
void SparkleEffect::render() {
	for (int i = 0; i < _sparkles.countAlive; ++i) {
		ds::sprites::draw(_sparkles.positions[i], _sparkles.textures[i]);
	}
}
