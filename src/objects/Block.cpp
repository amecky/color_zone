#include "Block.h"
#include <math\GameMath.h>
#include "..\Constants.h"
#include <math\math.h>
#include <renderer\graphics.h>
#include <renderer\sprites.h>

const float ROTATION_TIME = 0.2f;

const float STARTING_ANGLES[] = { DEGTORAD(225.0f), DEGTORAD(135.0f) , DEGTORAD(45.0f) , DEGTORAD(315.0f) };

// -----------------------------------------------------------------
//
// -----------------------------------------------------------------
Block::Block(GameSettings* settings, bool showBorder) : _settings(settings) , _showBorder(showBorder) {
	_position = v2(512, 384);
	for (int i = 0; i < 5; ++i) {
		_textures[i] = math::buildTexture(96, i * 36, 36, 36);
	}
	for (int i = 0; i < 4; ++i) {
		_colors[i] = 0;
	}
	pickColors();
	_rotating = false;
	_rotationRadius = length(v2(HALF_SQUARE_SIZE, HALF_SQUARE_SIZE));
}

Block::~Block() {
}

// -----------------------------------------------------------------
// set position
// -----------------------------------------------------------------
void Block::setPosition(const v2& p) {
	_position = p;
}

// -----------------------------------------------------------------
// get position
// -----------------------------------------------------------------
const v2& Block::getPosition() const {
	return _position;
}

// -----------------------------------------------------------------
// pick 4 random colors
// -----------------------------------------------------------------
void Block::pickColors() {
	int firstColor = math::random(0, 3);
	int secondColor = math::random(0, 3);
	_colors[0] = firstColor;
	_colors[1] = firstColor;
	_colors[2] = secondColor;
	_colors[3] = secondColor;
	int r = math::random(0, 3);
	for (int i = 0; i < r; ++i) {
		rotate();
	}
}

// -----------------------------------------------------------------
// render
// -----------------------------------------------------------------
// draw order:
// 12
// 03
void Block::render() {
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	v2 p = _position + v2(HALF_SQUARE_SIZE, HALF_SQUARE_SIZE);
	float norm = _rotationTimer / ROTATION_TIME;
	for (int i = 0; i < 4; ++i) {
		float angle = STARTING_ANGLES[i];
		if (_rotating) {
			angle += norm * HALF_PI;
		}
		v2 pp = ds::math::getDistantPosition(p, angle, _rotationRadius);
		sprites->draw(pp, _textures[_colors[i]], angle + DEGTORAD(45.0f));
	}
	if (_showBorder) {
		float r = 0.0f;
		if (_rotating) {
			r = norm * HALF_PI;
		}
		sprites->draw(p, math::buildTexture(0, 935, 78, 78),r);
	}
}

// -----------------------------------------------------------------
// get color by index
// -----------------------------------------------------------------
int Block::getColor(int index) const {
	return _colors[index];
}

// -----------------------------------------------------------------
// copy colors
// ----------------------------------------------------------------- 
void Block::copyColors(const Block* other) {
	for (int i = 0; i < 4; ++i) {
		_colors[i] = other->getColor(i);
	}
}

// -----------------------------------------------------------------
// rotate
// -----------------------------------------------------------------
void Block::rotate() {	
	if (!_rotating) {
		_rotating = true;
		_rotationTimer = 0.0f;
	}
}

// -----------------------------------------------------------------
// update
// -----------------------------------------------------------------
void Block::update(float dt) {
	if (_rotating) {
		_rotationTimer += dt;
		float norm = _rotationTimer / ROTATION_TIME;
		v2 center = _position + v2(HALF_SQUARE_SIZE);
		if (norm >= 1.0f) {
			int tmp = _colors[0];
			for (int i = 0; i < 3; ++i) {
				_colors[i] = _colors[i + 1];
			}
			_colors[3] = tmp;
			_rotating = false;
		}		
	}
}