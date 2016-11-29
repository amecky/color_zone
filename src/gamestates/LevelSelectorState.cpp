#include "LevelSelectorState.h"
#include <core\log\Log.h>
#include <renderer\graphics.h>
#include <gamestates\GameStateMachine.h>
#include "..\objects\Levels.h"

LevelSelectorState::LevelSelectorState(GameContext* context) : ds::BasicMenuGameState("LevelSelectorState", "LevelSelector"), _context(context) {
	_map = new TileMap(_context);
	_numLevels = 0;
}


LevelSelectorState::~LevelSelectorState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void LevelSelectorState::activate() {
	_map->reset();
	_map->build(0);
	_dialog->activate();
	_dialog->updateText(8, _context->levels->getName(0));
	_index = 0;
}

// --------------------------------------------
// activate
// --------------------------------------------
void LevelSelectorState::deactivate() {
	_dialog->deactivate();
}

// --------------------------------------------
// update
// --------------------------------------------
int LevelSelectorState::update(float dt) {
	// nothing to do
	return 0;
}

// --------------------------------------------
// click
// --------------------------------------------
int LevelSelectorState::onGUIButton(int button) {
	bool reload = false;
	if (button == 5) {
		--_index;
		if (_index < 0) {
			_index = 0;
		}		
		reload = true;
	}
	if (button == 6) {
		++_index;
		if (_index >= MAX_LEVELS) {
			_index = MAX_LEVELS - 1;
		}		
		reload = true;
	}
	if (reload) {		
		_context->levelIndex = _index;
		_map->reset();
		_map->build(_context->levelIndex);
		_dialog->updateText(8, _context->levels->getName(_index));
	}
	/*
	if (button == 4 || button == 5) {
		ds::GUIDialog* dlg = _gui->get("LevelSelector");
		if (_context->gameMode == GM_TIMER) {
			_context->gameMode = GM_COVERAGE;			
			dlg->updateText(6, "Coverage mode");
		}
		else {
			_context->gameMode = GM_TIMER;
			dlg->updateText(6, "Timer mode");
		}
	}
	*/
	return button;
}

// --------------------------------------------
// render
// --------------------------------------------
void LevelSelectorState::render() {	
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	sprites->begin();
	_map->render(SQUARE_SIZE / 2, 0.5f);
	_dialog->render();
	sprites->end();
}


