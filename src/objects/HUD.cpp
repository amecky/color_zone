#include "HUD.h"
#include <renderer\sprites.h>
#include <math\math.h>

const NumberDef DEFINITIONS[] = {
	//{ 410, 36 }, { 456, 24 }, { 492, 33 }, { 536, 28 }, { 574, 33 }, { 618, 34 }, { 662, 34 }, { 705, 34 }, { 746, 34 }, { 789, 34 }
	{ 410, 36 },{ 456, 24 },{ 492, 33 },{ 536, 28 },{ 574, 33 },{ 618, 34 },{ 662, 34 },{ 705, 34 },{ 746, 34 },{ 789, 34 }
};

HUD::HUD(GameSettings* settings) : _settings(settings) {
	for (int i = 0; i < 6; ++i) {
		_score_digits[i] = i;
	}
	for (int i = 0; i < 10; ++i) {
		//_textures[i] = math::buildTexture(0, DEFINITIONS[i].offset, DEFINITIONS[i].width, 32);		
		_textures[i] = math::buildTexture(96, i * 36, 36, 26);
	}
	for (int i = 0; i < 3; ++i) {
		_coverage_digits[i] = 0;
	}
	_delimiterTexture = math::buildTexture(96, 448, 16, 26);
	_perTexture = math::buildTexture(96, 376, 54, 26);
}


HUD::~HUD() {
}


void HUD::tick(float dt) {
	_timer.tick(dt);
}

void HUD::tickTimer(float dt) {

}

void HUD::start() {
	_timer.reset();
	_timer.start();
	setScore(0);
}

void HUD::convert(int value, int* array, int length) {
	int m = pow(10, length - 1);
	for (int i = 0; i < length; ++i) {
		array[i] = value / m;
		value = value - value / m * m;
		m /= 10;
	}
}

void HUD::setScore(int scr) {
	convert(scr, _score_digits, 6);
}

void HUD::setCoverage(int fillRate) {
	convert(fillRate, _coverage_digits, 3);
}

void HUD::render() {
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	// score
	v2 p = _settings->hud.scorePosition;
	for (int i = 0; i < 6; ++i) {
		sprites->draw(p, _textures[_score_digits[i]]);
		//p.x += DEFINITIONS[_score_digits[i]].width + _settings->hud.padding;
		p.x += 36 + _settings->hud.padding;
	}
	// coverage
	p = _settings->hud.coveragePosition;
	bool draw = true;
	for (int i = 0; i < 3; ++i) {
		draw = true;
		if (i < 2 && _coverage_digits[i] == 0) {
			draw = false;
		}
		if ( draw) {
			sprites->draw(p, _textures[_coverage_digits[i]]);
			//p.x += DEFINITIONS[_coverage_digits[i]].width + _settings->hud.padding;
			p.x += 36 + _settings->hud.padding;
		}		
	}
	p.x += 20;
	sprites->draw(p, _perTexture);
	// time
	int sec = _timer.getSeconds();
	int min = _timer.getMinutes();
}