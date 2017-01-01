#include "FadingBackgroundPlugin.h"

FadingBackgroundPlugin::FadingBackgroundPlugin(GameContext* context) : ds::Plugin("FadingBackground") , _context(context) {
	_texture = math::buildTexture(620, 320, 640, 360);
}

FadingBackgroundPlugin::~FadingBackgroundPlugin() {
}

void FadingBackgroundPlugin::tick(float dt) {
	_timer += dt;
	float norm = _timer / _ttl;
	if (norm >= 1.0f) {
		_timer = 0.0f;
		_ttl = math::random(_context->settings->background.minTTL, _context->settings->background.maxTTL);
		_minValue = _maxValue;
		_maxValue = math::random(_context->settings->background.minIntensity, _context->settings->background.maxIntensity);
	}
}

void FadingBackgroundPlugin::preRender() {
	ds::Color c = _context->colors[7];
	float norm = _timer / _ttl;
	c.a = tweening::interpolate(tweening::linear, _minValue, _maxValue, _timer, _ttl);
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	sprites->draw(v2(640, 360), _texture, 0.0f, v2(2, 2), c);
}

void FadingBackgroundPlugin::activate() {
	_minValue = 1.0f;
	_maxValue = math::random(_context->settings->background.minIntensity, _context->settings->background.maxIntensity);
	_active = true;
}
