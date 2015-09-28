#include "LevelSelectorState.h"
#include <utils\Log.h>
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>
#include <base\GameStateMachine.h>

LevelSelectorState::LevelSelectorState(ds::DialogManager* gui,GameContext* context) : ds::GameState("LevelSelectorState"), _context(context) , _gui(gui) {
	_map = std::make_unique<TileMap>();
	activate();
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
	return button;
}

// --------------------------------------------
// render
// --------------------------------------------
void LevelSelectorState::render() {	
	_map->render(SQUARE_SIZE/2,0.5f);
}


