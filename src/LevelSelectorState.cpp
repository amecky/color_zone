#include "LevelSelectorState.h"
#include <utils\Log.h>
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>
#include <base\GameStateMachine.h>

LevelSelectorState::LevelSelectorState(ds::DialogManager* gui,GameContext* context) : ds::GameState("LevelSelectorState"), _context(context) , _gui(gui) {
	_map = std::make_unique<TileMap>();
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
	_numLevels = 0;
	_index = 0;
	char buffer[128];
	for (size_t i = 0; i < _context->filesystem.numFiles(); ++i) {
		const FSFile& f = _context->filesystem.getFile(i);
		strcpy(buffer, f.name + 1);
		_levels[_numLevels++] = atoi(buffer);
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
int LevelSelectorState::onGUIButton(ds::DialogID dlgID, int button) {
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


