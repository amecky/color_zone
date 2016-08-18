#include "TestState.h"
#include <utils\Log.h>
#include <renderer\graphics.h>
#include <base\InputSystem.h>

TestState::TestState(GameContext* context, ds::Game* game) : ds::GameState("TestState", game), _context(context) {
	_map = new TileMap(_context);
	_laser = new Laser(_context);
	_map->reset();
	_previewBlock = new Block(_context, false);
	_previewBlock->setPosition(p2i(760, 660));
	_mainBlock = new Block(_context, true);
	_effect = new SparkleEffect(_context);
	//_hud = new HUD(_context->settings);
	_hud = ds::res::getGUIDialog("HUD");
}

TestState::~TestState() {
	//delete _hud;
	delete _mainBlock;
	delete _previewBlock;
	delete _effect;
	delete _laser;
	delete _map;
}

void TestState::init() {
}

void TestState::activate() {
	_context->pick_colors();
	_map->reset();
	_map->build(0);
	_laser->start();
	//_hud->start();
	_hud->activate();
	//_map->load(1);
	_context->score = 0;
	_effect->reset();	
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
				_effect->start(p2i(column, i), colors[i]);
			}
		}
		int cleared = _map->clearColumn(column);
		_context->score += cleared * 100;
		if (cleared > 0) {
			_context->fillRate = _map->getFillRate();
			LOG << "fillRate: " << _context->fillRate;
			//_hud->setScore(_context->score);
			//_hud->setCoverage(_context->fillRate);
		}
	}
	_laser->tick(dt);
}
// --------------------------------------------
// update
// --------------------------------------------
int TestState::update(float dt) {

	// move main block
	v2 mp = ds::input::getMousePosition();
	p2i tmp = map::screen2grid(mp);
	p2i converted = map::grid2screen(tmp);
	_mainBlock->setPosition(converted);
	_mainBlock->update(dt);

	_hud->tick(dt);
	
	const ds::GameTimer* timer = _hud->getTimer(5);
	if (timer->getMinutes() > 0) {
		//fillHighscore();
		//return 666;
	}

	moveLaser(dt);	

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
		_context->settings->load();
	}
	if (ascii == 'k') {
		_context->pick_colors();
	}
	if (ascii == 'p') {
		_effect->start(p2i(8, 8), 1);
	}
	return 0;
}
