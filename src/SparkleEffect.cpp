#include "SparkleEffect.h"
#include "Constants.h"
#include <renderer\sprites.h>
#include "TileMap.h"
/*
v2* _positions;
v2* _velocities;
v2* _accelerations;
float* _timers;
float* _rotations;
v2* _scales;
int* _colors;
*/
// --------------------------------------------
// Sparkle effect
// --------------------------------------------
SparkleEffect::SparkleEffect(GameContext* context) : _context(context) {
	int sizes[] = { sizeof(v2), sizeof(v2), sizeof(v2), sizeof(float), sizeof(float), sizeof(v2), sizeof(int) };
	_sparkles.init(sizes, 7);
	allocate(2048);
	_texture = math::buildTexture(0, 200, 10, 10);
}


SparkleEffect::~SparkleEffect() {
}

void SparkleEffect::allocate(int sz) {
	if (_sparkles.resize(sz)) {
		_positions = (v2*)_sparkles.get_ptr(0);
		_velocities = (v2*)_sparkles.get_ptr(1);
		_accelerations = (v2*)_sparkles.get_ptr(2);
		_timers = (float*)_sparkles.get_ptr(3);
		_rotations = (float*)_sparkles.get_ptr(4);
		_scales = (v2*)_sparkles.get_ptr(5);
		_colors = (int*)_sparkles.get_ptr(6);
	}
}

// --------------------------------------------
// start block
// --------------------------------------------
void SparkleEffect::start(const p2i& pos, int color) {
	int pieces = 4;
	float dx = 4.0f;
	float dy = 4.0f;
	float gap = _context->settings->sparkleGap;
	p2i np = map::grid2screen(pos);
	np.x -= (pieces - 1) * gap + SQUARE_SIZE / 4;
	np.y += (pieces - 1) * gap + SQUARE_SIZE / 4;
	int cnt = 0;
	for (int y = 0; y < pieces; ++y) {
		for (int x = 0; x < pieces; ++x) {
			if (_sparkles.size < 2048) {
				int idx = _sparkles.size;
				_positions[idx] = v2(np.x + (dx + gap) * x, np.y - (dy + gap) * y);
				_timers[idx] = 0.0f;
				float v = _context->settings->sparkleVelocity + math::random(-_context->settings->sparkleVelocityVariance, _context->settings->sparkleVelocityVariance);
				_velocities[idx] = math::getRadialVelocity(PARTICLE_ANGLES[cnt++], v);
				_accelerations[idx] = v2(0, 0);
				_colors[idx] = color;
				_scales[idx] = v2(1, 1);
				_rotations[idx] = 0.0f;
				++_sparkles.size;
			}
		}
	}
}

// --------------------------------------------
// update
// --------------------------------------------
void SparkleEffect::update(float dt) {
	for (int i = 0; i < _sparkles.size; ++i) {
		_timers[i] += dt;
		if (_timers[i] > _context->settings->sparkleTTL ) {
			_sparkles.remove(i);
		}
	}
	for (int i = 0; i < _sparkles.size; ++i) {
		_velocities[i] += _accelerations[i] * dt;
		_positions[i] += _velocities[i] * dt;
		_scales[i] = lerp(v2(_context->settings->sparkleStartScale, _context->settings->sparkleStartScale), v2(_context->settings->sparkleEndScale, _context->settings->sparkleEndScale),_timers[i]/_context->settings->sparkleTTL);
		_rotations[i] = _timers[i] * TWO_PI / _context->settings->sparkleTTL;
	}
}

// --------------------------------------------
// render
// --------------------------------------------
void SparkleEffect::render() {
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	for (int i = 0; i < _sparkles.size; ++i) {
		sprites->draw(_positions[i], _texture, _rotations[i],_scales[i],_context->colors[_colors[i]]);
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
	/*
	_texture = math::buildTexture(r);
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
				_sparkles.textures[cnt] = math::buildTexture(r.top + dy * y, r.left + dx * x, dx, dy);
				//float angle = ds::math::getTargetAngle(_sparkles.positions[cnt], v2(512, 384));
				float angle = math::random(0.0f, TWO_PI);
				float v = _context->settings->introVelocity + math::random(-_context->settings->introVelocityVariance, _context->settings->introVelocityVariance);
				_sparkles.velocities[cnt] = math::getRadialVelocity(RADTODEG(angle), v);
				_sparkles.scales[cnt] = v2(1, 1);
				++cnt;
			}
		}
	}
	_warmUpTimer = _context->settings->introWarmupTimer;
	_running = true;
	*/
}

void IntroEffect::render() {
	if (_running) {
		if (_warmUpTimer > 0.0f) {
			//ds::sprites::draw(v2(512, 384), _texture);
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
	/*
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
	*/
}