#include "MainGame.h"
#include <utils\Log.h>
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>

MainGame::MainGame(const char* name) : ds::GameState(name) {
	_map = std::make_unique<TileMap>(MAX_X, MAX_Y);
	_map->reset();
	_levelIndex = 1;
	_previewBlock.setPosition(v2(512, 700));
	_laser.active = false;
	_laser.timer = 0.0f;
	_laser.column = 0;
	_laser.texture = ds::math::buildTexture(132, 220, 36, 36);
}

MainGame::~MainGame() {
}

void MainGame::activate() {
	_map->reset();
	_levelIndex = 1;
	_laser.active = false;
	_map->load(_levelIndex);
}

// --------------------------------------------
// update
// --------------------------------------------
void MainGame::update(float dt) {
	// move main block
	v2 mp = ds::renderer::getMousePosition();
	v2 converted = _map->convert(mp);
	_mainBlock.setPosition(converted);
	// move laser
	if (_laser.active) {
		_laser.timer += dt;
		if (_laser.timer > 0.4f) {
			++_laser.column;
			_map->clearColumn(_laser.column);
			_laser.timer = 0.0f;
			if (_laser.column >= MAX_X) {
				_laser.active = false;
			}
		}
	}
}

// --------------------------------------------
// click
// --------------------------------------------
void MainGame::onButtonUp(int button, int x, int y) {
	if (button == 0) {
		if (_map->copyBlock(_mainBlock)) {
			_mainBlock.copyColors(_previewBlock);
			_previewBlock.pickColors();
		}
	}
	else if (button == 1) {
		_mainBlock.rotate();
	}
}

// --------------------------------------------
// render
// --------------------------------------------
void MainGame::render() {
	_map->render();
	if (_laser.active) {
		for (int i = 0; i < MAX_Y; ++i) {
			ds::sprites::draw(v2(START_X + 36 * _laser.column, START_Y + i * SQUARE_SIZE), _laser.texture);
		}
	}
	_previewBlock.render();
	_mainBlock.render();
}

// --------------------------------------------
// start laser
// --------------------------------------------
void MainGame::startLaser() {
	if (!_laser.active) {
		_laser.active = true;
		_laser.column = -1;
		_laser.timer = 0.0f;
	}
}

// --------------------------------------------
// on char
// --------------------------------------------
void MainGame::onChar(int ascii) {
	if (ascii == '1') {
		_mainBlock.copyColors(_previewBlock);
		_previewBlock.pickColors();
	}
	if (ascii == 's') {
		startLaser();
	}
}
