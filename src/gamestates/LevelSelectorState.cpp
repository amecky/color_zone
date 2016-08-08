#include "LevelSelectorState.h"
#include <utils\Log.h>
#include <renderer\graphics.h>
#include <gamestates\GameStateMachine.h>

LevelSelectorState::LevelSelectorState(GameContext* context) : ds::GameState("LevelSelectorState"), _context(context) {
	_map = new TileMap;
}


LevelSelectorState::~LevelSelectorState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void LevelSelectorState::activate() {
	_map->reset();
	_map->load(_context->levelIndex);
	_gui->activate("LevelSelector");
	_context->filesystem.refresh();
	_numLevels = _context->filesystem.getAvailableLevels(_levels);
	_index = 0;
	for (int i = 0; i < _numLevels; ++i) {
		if (_levels[i] == _context->levelIndex) {
			_index = i;
		}
	}	
	LOG << "available levels: " << _numLevels;
}

// --------------------------------------------
// activate
// --------------------------------------------
void LevelSelectorState::deactivate() {
	_gui->deactivate("LevelSelector");
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
	if (button == 2) {
		--_index;
		if (_index < 0) {
			_index = 0;
		}		
		reload = true;
	}
	if (button == 3) {
		++_index;
		if (_index >= _numLevels) {
			_index = _numLevels - 1;
		}		
		reload = true;
	}
	if (reload) {
		_context->levelIndex = _levels[_index];
		_map->reset();
		_map->load(_context->levelIndex);
	}
	// "text" { "id" : "6" , "pos" : "150,595" , "text" : "Timer mode" }
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
	return button;
}

// --------------------------------------------
// render
// --------------------------------------------
void LevelSelectorState::render() {	
	_map->render(SQUARE_SIZE/2,0.5f);
}


