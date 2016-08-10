#include "TestState.h"
#include <utils\Log.h>
#include <renderer\graphics.h>
#include <base\InputSystem.h>

TestState::TestState(GameContext* context, ds::Game* game) : ds::GameState("TestState", game), _context(context) {
	_map = new TileMap;
	_laser = new Laser(context->settings);
	_map->reset();
	_previewBlock = new Block(_context->settings, false);
	_previewBlock->setPosition(v2(512, 700));
	_mainBlock = new Block(_context->settings, true);
	_effect = new SparkleEffect(_context);
	_hud = ds::res::getGUIDialog("HUD");
}

TestState::~TestState() {	
	delete _mainBlock;
	delete _previewBlock;
	delete _effect;
	delete _laser;
	delete _map;
}

void TestState::init() {
}

void TestState::activate() {
	//if (!_context->resume) {
		_map->reset();
		_laser->start();
		_hud->activate();
		//_map->load(1);
		//_context->score = 0;
		//_context->fillRate = 0;
		_hud->resetTimer(HUD_TIMER);
		_hud->startTimer(HUD_TIMER);
		_hud->setNumber(HUD_SCORE, 0);
		_hud->updateText(HUD_PERCENTAGE, "0 %");
		_effect->reset();
	//}
	//_context->resume = false;
}

void TestState::fillHighscore() {	
}
// --------------------------------------------
// move laser
// --------------------------------------------
void TestState::moveLaser(float dt) {
	int column = -1;
	if (_laser->move(dt, &column)) {
		int colors[MAX_Y];
		_map->getColumn(column, colors);
		for (int i = 0; i < MAX_Y; ++i) {
			if (colors[i] != -1) {
				_effect->start(column, i, ds::Rect(170, 2 + colors[i] * 36, 32, 32), 4);
			}
		}
		int cleared = _map->clearColumn(column);
		_context->score += cleared * 100;
		if (cleared > 0) {
			_context->fillRate = _map->getFillRate();
			LOG << "fillRate: " << _context->fillRate;
			_hud->setNumber(HUD_SCORE, _context->score);
			char buffer[128];
			sprintf_s(buffer, 128, "%d%%", _context->fillRate);
			_hud->updateText(HUD_PERCENTAGE, buffer);
		}
	}
	_laser->tick(dt);
}
// --------------------------------------------
// update
// --------------------------------------------
int TestState::update(float dt) {

	_mainBlock->update(dt);

	_hud->tick(dt);
	if (_context->gameMode == GM_TIMER) {
		//ds::GameTimer* timer = _hud.getTimer(2);
		//if (timer->getMinutes() > 0) {
			//fillHighscore();
			//return 666;
		//}
	}
	// move main block
	v2 mp = ds::input::getMousePosition();
	v2 converted = map::convert(mp);
	_mainBlock->setPosition(converted);
	// FIXME: disabled for testen
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
int TestState::onButtonUp(int button, int x, int y) {
	if (button == 0) {
		if (_map->copyBlock(_mainBlock)) {
			_mainBlock->copyColors(_previewBlock);
			_previewBlock->pickColors();
		}
	}
	else if (button == 1) {
		_mainBlock->rotate();
	}
	return 0;
}

// --------------------------------------------
// render
// --------------------------------------------
void TestState::render() {
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	sprites->begin();
	_map->render();
	_effect->render();
	_previewBlock->render();
	_mainBlock->render();
	_laser->render();	
	sprites->end();
	_hud->render();
}

// --------------------------------------------
// on char
// --------------------------------------------
int TestState::onChar(int ascii) {
	if (ascii == '1') {
		_mainBlock->copyColors(_previewBlock);
		_previewBlock->pickColors();
	}
	if (ascii == 's') {
		_laser->start();
	}
	if (ascii == 'd') {
		_context->resume = true;
		return 1;
	}
	if (ascii == 'x') {
		fillHighscore();
		return 666;
	}
	if (ascii == 'r') {
		_hud->load();
	}
	return 0;
}
