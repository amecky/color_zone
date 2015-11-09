#include "TileMapEditor.h"
#include <utils\Log.h>
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>
#include <base\GameStateMachine.h>

TileMapEditor::TileMapEditor(ds::DialogManager* gui, GameContext* context) : ds::GameState("TileMapEditor") , _gui(gui) , _context(context) {
	_map = std::make_unique<TileMap>();
}


TileMapEditor::~TileMapEditor() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void TileMapEditor::activate() {
	_dialog = _gui->get("Editor");
	_map->reset();
	_mode = EM_EDIT_MAP;
	_currentBorder = 0;
	_levelIndex = 1;
	_gui->activate("Editor");
	updateLevelLabel();
	updateModeLabel();
}

void TileMapEditor::deactivate() {
	_gui->deactivate("Editor");
}

// --------------------------------------------
// update
// --------------------------------------------
int TileMapEditor::update(float dt) {
	// nothing to do
	return 0;
}

// --------------------------------------------
// select border icon
// --------------------------------------------
int TileMapEditor::selectBorder(int x, int y) {
	int my = 768 - y;
	if (my < 680 || my > 720) {
		return -1;
	}
	// x - start_x - icon_size / 2
	int xp = (x - 72) / 40;
	if (xp >= 0) {
		return xp;
	}
	return -1;
}

// --------------------------------------------
// click
// --------------------------------------------
int TileMapEditor::onButtonUp(int button, int x, int y) {
	int border = selectBorder(x, y);
	if (border != -1) {
		_currentBorder = border;
	}	
	else {
		v2 p = _map->convertToGridPos(x, 768 - y);
		if (_mode == EM_EDIT_MAP) {			
			if (_map->isValid(p.x, p.y)) {
				Tile& t = _map->get(p.x, p.y);
				if (button == 1) {
					t.state.clear(BIT_AVAILABLE);
				}
				else {
					t.state.set(BIT_AVAILABLE);
				}
			}
		}
		else if (_mode == EM_BORDERS) {
			if (_map->isValid(p.x, p.y)) {
				if (button == 0) {
					_map->setBorder(p.x,p.y, _currentBorder);
				}
				else {
					_map->setBorder(p.x, p.y, -1);
				}
			}
		}
	}
	return 0;
}

// --------------------------------------------
// render
// --------------------------------------------
void TileMapEditor::render() {
	// draw border selection
	for (int i = 0; i < 21; ++i) {
		v2 p(92 + i * 40, 700);
		if (i == _currentBorder) {
			ds::sprites::draw(p, ds::math::buildTexture(0, 88, 44, 44), 0.0f, 0.75f, 0.75f);
		}
		else {
			ds::sprites::draw(p, ds::math::buildTexture(0, 44, 44, 44), 0.0f, 0.75f, 0.75f);
		}
		ds::sprites::draw(p, ds::math::buildTexture(44, 44 * i, 44, 44), 0.0f, 0.75f, 0.75f);
	}
	// map
	_map->render();
	ds::debug::drawDebugMessages();
	
}

void TileMapEditor::updateModeLabel() {
	if (_mode == EM_EDIT_MAP) {	
		_dialog->updateText(18, "Edit mode: Map");
	}
	else {
		_dialog->updateText(18, "Edit mode: Border");
	}
}

void TileMapEditor::updateLevelLabel() {
	char buffer[32];
	sprintf_s(buffer, 32, "Level: %d", _levelIndex);
	_dialog->updateText(19, buffer);
}
// --------------------------------------------
// on char
// --------------------------------------------
int TileMapEditor::onChar(int ascii) {
	ds::GUIDialog* dlg = _gui->get("Editor");
	if (ascii == '1') {
		_mode = EM_EDIT_MAP;
		updateModeLabel();
	}
	if (ascii == '2') {
		_mode = EM_BORDERS;		
		updateModeLabel();
	}	
	if (ascii == '+') {
		++_levelIndex;
		if (_levelIndex >= MAX_LEVELS) {
			_levelIndex = MAX_LEVELS - 1;
		}
		updateLevelLabel();
	}
	if (ascii == '-') {
		--_levelIndex;
		if (_levelIndex < 1) {
			_levelIndex = 1;
		}
		updateLevelLabel();
	}
	return 0;
}

int TileMapEditor::onGUIButton(ds::DialogID dlgID, int button) {
	if (button == 1) {
		return 1;
	}
	if (button == 2) {
		_map->save(_levelIndex);
		return 0;
	}
	if (button == 3) {
		_map->load(_levelIndex);
		return 0;
	}
	if (button == 4) {
		_map->reset();
		return 0;
	}
	return button;
}
