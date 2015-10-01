#include "Block.h"
#include <math\GameMath.h>
#include <sprites\SpriteBatch.h>
#include "Constants.h"

Block::Block() {
	_position = v2(512, 384);
	for (int i = 0; i < 5; ++i) {
		_textures[i] = ds::math::buildTexture(132, i * 36, 36, 36);
	}
	for (int i = 0; i < 4; ++i) {
		_colors[i] = 0;
	}
	pickColors();
}

Block::~Block() {
}

void Block::setPosition(const v2& p) {
	_position = p;
}

const v2& Block::getPosition() const {
	return _position;
}

void Block::pickColors() {
	int firstColor = ds::math::random(0, 4);
	int secondColor = ds::math::random(0, 4);
	_colors[0] = firstColor;
	_colors[1] = firstColor;
	_colors[2] = secondColor;
	_colors[3] = secondColor;
	int r = ds::math::random(0, 3);
	for (int i = 0; i < r; ++i) {
		rotate();
	}
}

// draw order:
// 12
// 03
void Block::render() {
	ds::sprites::draw(v2(_position.x, _position.y), _textures[_colors[0]]);
	ds::sprites::draw(v2(_position.x, _position.y + SQUARE_SIZE), _textures[_colors[1]]);
	ds::sprites::draw(v2(_position.x + SQUARE_SIZE, _position.y + SQUARE_SIZE), _textures[_colors[2]]);
	ds::sprites::draw(v2(_position.x + SQUARE_SIZE, _position.y), _textures[_colors[3]]);
}

int Block::getColor(int index) const {
	return _colors[index];
}

void Block::copyColors(const Block& other) {
	for (int i = 0; i < 4; ++i) {
		_colors[i] = other.getColor(i);
	}
}

void Block::rotate() {
	int tmp = _colors[0];
	for (int i = 0; i < 3; ++i) {
		_colors[i] = _colors[i + 1];
	}
	_colors[3] = tmp;
}