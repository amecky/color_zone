#include "SparkleEffect.h"
#include "..\TileMap.h"
#include "..\GameSettings.h"
#include <SpriteBatchBuffer.h>

const float PARTICLE_ANGLES[] = {
	135.0f,110.0f, 70.0f, 45.0f,
	160.0f,110.0f, 70.0f, 20.0f,
	200.0f,200.0f,340.0f,340.0f,
	225.0f,250.0f,290.0f,325.0f
};
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
SparkleEffect::SparkleEffect(GameContext* context) : _ctx(context) {
	int sizes = sizeof(ds::vec2) + sizeof(ds::vec2) + sizeof(ds::vec2) + sizeof(float) + sizeof(float) + sizeof(ds::vec2) + sizeof(int);
	_sparklesCapacity = 2048;
	_sparkles = new char[_sparklesCapacity * sizes];
	allocate(2048);
	_texture = ds::vec4(200, 0, 10, 10);
}


SparkleEffect::~SparkleEffect() {
	delete[] _sparkles;
}

void SparkleEffect::allocate(int sz) {
	_positions = (ds::vec2*)(_sparkles);
	_velocities = (ds::vec2*)(_positions + _sparklesCapacity);
	_accelerations = (ds::vec2*)(_velocities + _sparklesCapacity);
	_timers = (float*)(_accelerations + _sparklesCapacity);
	_rotations = (float*)(_timers + _sparklesCapacity);
	_scales = (ds::vec2*)(_rotations + _sparklesCapacity);
	_colors = (int*)(_scales + _sparklesCapacity);
}

// --------------------------------------------
// start block
// --------------------------------------------
void SparkleEffect::start(const p2i& pos, int color) {
	int pieces = 4;
	float dx = 4.0f;
	float dy = 4.0f;
	float gap = _ctx->settings->sparkleGap;
	p2i np = map::grid2screen(pos);
	np.x -= (pieces - 1) * gap + SQUARE_SIZE / 4;
	np.y += (pieces - 1) * gap + SQUARE_SIZE / 4;
	int cnt = 0;
	for (int y = 0; y < pieces; ++y) {
		for (int x = 0; x < pieces; ++x) {
			if (_sparklesCount < 2048) {
				int idx = _sparklesCount;
				_positions[idx] = ds::vec2(np.x + (dx + gap) * x, np.y - (dy + gap) * y);
				_timers[idx] = 0.0f;
				float v = _ctx->settings->sparkleVelocity + ds::random(-_ctx->settings->sparkleVelocityVariance, _ctx->settings->sparkleVelocityVariance);
				_velocities[idx] = ds::vec2( v * cosf(PARTICLE_ANGLES[cnt++]), v * sinf(PARTICLE_ANGLES[cnt++]));
				_accelerations[idx] = ds::vec2(0, 0);
				_colors[idx] = color;
				_scales[idx] = ds::vec2(1, 1);
				_rotations[idx] = 0.0f;
				++_sparklesCount;
			}
		}
	}
}

// --------------------------------------------
// update
// --------------------------------------------
void SparkleEffect::update(float dt) {
	for (int i = 0; i < _sparklesCount; ++i) {
		_timers[i] += dt;
		if (_timers[i] > _ctx->settings->sparkleTTL) {
			if (_sparklesCount > 0) {
				int l = _sparklesCount - 1;
				_positions[i] = _positions[l];
				_timers[i] = _timers[l];
				_velocities[i] = _velocities[l];
				_accelerations[i] = _accelerations[l];
				_colors[i] = _colors[l];
				_scales[i] = _scales[l];
				_rotations[i] = _rotations[l];
				--_sparklesCount;
			}
			//_sparkles.remove(i);
			// swap out
		}
	}
	for (int i = 0; i < _sparklesCount; ++i) {
		_velocities[i] += _accelerations[i] * dt;
		_positions[i] += _velocities[i] * dt;
		_scales[i] = lerp(ds::vec2(_ctx->settings->sparkleStartScale, _ctx->settings->sparkleStartScale), ds::vec2(_ctx->settings->sparkleEndScale, _ctx->settings->sparkleEndScale), _timers[i] / _ctx->settings->sparkleTTL);
		_rotations[i] = _timers[i] * ds::TWO_PI / _ctx->settings->sparkleTTL;
	}
}

// --------------------------------------------
// render
// --------------------------------------------
void SparkleEffect::render(SpriteBatchBuffer* buffer) {
	for (int i = 0; i < _sparklesCount; ++i) {
		buffer->add(_positions[i], _texture, _scales[i], _rotations[i], _ctx->colors[_colors[i]]);
	}
}
