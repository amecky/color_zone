#include "SparkleEffect.h"
#include <sprites\SpriteBatch.h>
#include "Constants.h"

// --------------------------------------------
// Sparkle effect
// --------------------------------------------
SparkleEffect::SparkleEffect(GameContext* context) : _context(context) {
	_sparkles.initialize(4096);
	_startX = (1024 - SQUARE_SIZE * MAX_X) / 2;
	_startY = (768 - SQUARE_SIZE * MAX_Y) / 2;
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
	float gap = _context->settings->sparkleGap;
	int start = _sparkles.countAlive;
	int end = start + total;
	v2 np = v2(_startX + xp * SQUARE_SIZE, _startY + yp * SQUARE_SIZE);
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
				float v = _context->settings->sparkleVelocity + ds::math::random(-_context->settings->sparkleVelocityVariance, _context->settings->sparkleVelocityVariance);
				_sparkles.velocities[cnt] = ds::math::getRadialVelocity(PARTICLE_ANGLES[cnt - start], v);
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
		if (_sparkles.timers[i] > _context->settings->sparkleTTL ) {
			_sparkles.kill(i);
		}
	}
	for (int i = 0; i < _sparkles.countAlive; ++i) {
		_sparkles.positions[i] += _sparkles.velocities[i] * dt;
		_sparkles.scales[i] = lerp(v2(_context->settings->sparkleStartScale, _context->settings->sparkleStartScale), v2(_context->settings->sparkleEndScale, _context->settings->sparkleEndScale),_sparkles.timers[i]/_context->settings->sparkleTTL);
	}
}

// --------------------------------------------
// render
// --------------------------------------------
void SparkleEffect::render() {
	for (int i = 0; i < _sparkles.countAlive; ++i) {
		ds::sprites::draw(_sparkles.positions[i], _sparkles.textures[i],0.0f,_sparkles.scales[i].x,_sparkles.scales[i].y);
	}
}

// --------------------------------------------
// Intro effect
// --------------------------------------------
IntroEffect::IntroEffect(GameContext* context) : SparkleEffect(context) , _warmUpTimer(0.0f) , _running(false) {}

// --------------------------------------------
// start
// --------------------------------------------
void IntroEffect::start(int xp, int yp, const ds::Rect& r, int stepX, int stepY, float gap) {
	_texture = ds::math::buildTexture(r);
	int total = stepX * stepY;
	float dx = r.width() / stepX;
	float dy = r.height() / stepY;
	int start = _sparkles.countAlive;
	int end = start + total;
	int xpos = (1024 - r.width()) / 2;
	int ypos = (768 - r.height()) / 2 + r.height();
	v2 np = v2(xpos, ypos);
	if (end < _sparkles.count) {
		for (int i = 0; i < total; ++i) {
			_sparkles.wake(start + i);
		}
		int cnt = start;
		for (int y = 0; y < stepY; ++y) {
			for (int x = 0; x < stepX; ++x) {
				_sparkles.positions[cnt] = v2(np.x + (dx + gap) * x, np.y - (dy + gap) * y);
				_sparkles.timers[cnt] = 0.0f;
				_sparkles.textures[cnt] = ds::math::buildTexture(r.top + dy * y, r.left + dx * x, dx, dy);
				//float angle = ds::math::getTargetAngle(_sparkles.positions[cnt], v2(512, 384));
				float angle = ds::math::random(0.0f, TWO_PI);
				float v = _context->settings->introVelocity + ds::math::random(-_context->settings->introVelocityVariance, _context->settings->introVelocityVariance);
				_sparkles.velocities[cnt] = ds::math::getRadialVelocity(RADTODEG(angle), v);
				_sparkles.scales[cnt] = v2(1, 1);
				++cnt;
			}
		}
	}
	_warmUpTimer = _context->settings->introWarmupTimer;
	_running = true;
}

void IntroEffect::render() {
	if (_running) {
		if (_warmUpTimer > 0.0f) {
			ds::sprites::draw(v2(512, 384), _texture);
		}
		else {
			SparkleEffect::render();
		}
	}
}
// --------------------------------------------
// update
// --------------------------------------------
void IntroEffect::update(float dt) {
	if (_running) {
		if (_warmUpTimer > 0.0f) {
			_warmUpTimer -= dt;
		}
		else {
			for (int i = 0; i < _sparkles.countAlive; ++i) {
				_sparkles.timers[i] += dt;
				if (_sparkles.timers[i] > _context->settings->introTTL) {
					_sparkles.kill(i);
				}
			}
			for (int i = 0; i < _sparkles.countAlive; ++i) {
				_sparkles.positions[i] += _sparkles.velocities[i] * dt;
				_sparkles.scales[i] = lerp(v2(_context->settings->introStartScale, _context->settings->introStartScale), v2(_context->settings->introEndScale, _context->settings->introEndScale), _sparkles.timers[i] / _context->settings->introTTL);
			}
			if (_sparkles.countAlive == 0) {
				_running = false;
			}
		}
	}
}