#include "MainGame.h"
#include <utils\Log.h>
#include <renderer\graphics.h>
#include <base\InputSystem.h>

MainGame::MainGame(GameContext* context, ds::Game* game) : ds::GameState("MainGame", game), _context(context) {
	_map = new TileMap;
	_map->reset();
	_previewBlock.setPosition(v2(512, 700));
	_laser.active = false;
	_laser.timer = 0.0f;
	_laser.column = 0;
	_laser.texture = math::buildTexture(132, 220, 36, 36);
	_effect = new SparkleEffect(_context);
}

MainGame::~MainGame() {
	delete _effect;
}

// --------------------------------------------
// init
// --------------------------------------------
void MainGame::init() {	
}

// --------------------------------------------
// activate
// --------------------------------------------
void MainGame::activate() {
	_context->hud->activate();
	if (!_context->resume) {
		_map->reset();
		_laser.active = false;
		_laser.timer = _context->settings->laserStartDelay;
		_map->load(_context->levelIndex);
		_context->score = 0;
		_context->fillRate = 0;
		_context->hud->resetTimer(HUD_TIMER);
		_context->hud->setNumber(HUD_SCORE, 0);
		_context->hud->updateText(HUD_PERCENTAGE, "0 %");
		_effect->reset();
	}
	_context->resume = false;
}

// --------------------------------------------
// deactivate
// --------------------------------------------
void MainGame::deactivate() {
	_context->hud->deactivate();
}

// --------------------------------------------
// fill highscore
// --------------------------------------------
void MainGame::fillHighscore() {
	_context->currentScore.fillrate = _context->fillRate;
	_context->currentScore.score = _context->score;
	_context->currentScore.mode = _context->gameMode;
	_context->currentScore.level = _context->levelIndex;
	sprintf_s(_context->currentScore.name, 10,"%s",_context->name);
	ds::GameTimer* timer = _context->hud->getTimer(HUD_TIMER);
	_context->currentScore.minutes = timer->getMinutes();
	_context->currentScore.seconds = timer->getSeconds();
}
// --------------------------------------------
// move laser
// --------------------------------------------
void MainGame::moveLaser(float dt) {
	if (_laser.active) {
		_laser.timer += dt;
		// FIXME: color should be pulsing
		// FIXME: take this one from settings
		if (_laser.timer > 0.4f) {
			++_laser.column;
			if (_laser.column >= 0 && _laser.column < MAX_X) {
				int colors[MAX_Y];
				_map->getColumn(_laser.column, colors);
				for (int i = 0; i < MAX_Y; ++i) {
					if (colors[i] != -1) {
						_effect->start(_laser.column, i, ds::Rect(170,2 + colors[i]*36,32,32),4);
					}
				}
				int cleared = _map->clearColumn(_laser.column);
				_context->score += cleared * 100;
				if (cleared > 0) {
					_context->fillRate = _map->getFillRate();
					_context->hud->setNumber(HUD_SCORE, _context->score);
					char buffer[128];
					sprintf_s(buffer, 128, "%d%%", _context->fillRate);
					_context->hud->updateText(HUD_PERCENTAGE, buffer);
				}
			}
			_laser.timer = 0.0f;
			if (_laser.column >= MAX_X) {
				_laser.active = false;
				_laser.timer = 5.0f;
			}
		}
	}
	else {
		_laser.timer -= dt;
		if (_laser.timer <= 0.0f) {
			startLaser();
		}
	}
}
// --------------------------------------------
// update
// --------------------------------------------
int MainGame::update(float dt) {
	if (_context->gameMode == GM_TIMER) {
		ds::GameTimer* timer = _context->hud->getTimer(HUD_TIMER);
		if (timer->getMinutes() > 2) {
			fillHighscore();
			return 666;
		}
	}
	// move main block
	_mainBlock.update(dt);
	v2 mp = ds::input::getMousePosition();
	v2 converted = _map->convert(mp);
	_mainBlock.setPosition(converted);
	moveLaser(dt);	
	if (_context->gameMode == GM_COVERAGE) {
		if (_context->fillRate >= 80) {
			fillHighscore();
			return 666;
		}
	}
	_effect->update(dt);
	return 0;
}

// --------------------------------------------
// click
// --------------------------------------------
int MainGame::onButtonUp(int button, int x, int y) {
	if (button == 0) {
		if (_map->copyBlock(_mainBlock)) {
			_mainBlock.copyColors(_previewBlock);
			_previewBlock.pickColors();
		}
	}
	else if (button == 1) {
		_mainBlock.rotate();
	}
	return 0;
}

// --------------------------------------------
// render
// --------------------------------------------
void MainGame::render() {
	_map->render();
	if (_laser.active) {
		for (int i = 0; i < MAX_Y; ++i) {
			//ds::sprites::draw(_map->convert(_laser.column,i), _laser.texture);
		}
	}
	_effect->render();
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
int MainGame::onChar(int ascii) {
	if (ascii == '1') {
		_mainBlock.copyColors(_previewBlock);
		_previewBlock.pickColors();
	}
	if (ascii == 'd') {
		_context->resume = true;
		return 1;
	}
	if (ascii == 'x') {
		fillHighscore();
		return 666;
	}
	return 0;
}
