#include "TileMapEditor.h"
#include <utils\Log.h>
#include <renderer\graphics.h>
#include <gamestates\GameStateMachine.h>

TileMapEditor::TileMapEditor(GameContext* context, ds::Game* game) : ds::BasicMenuGameState("TileMapEditor", "editor", game), _context(context) {
	_map = new TileMap;
}


TileMapEditor::~TileMapEditor() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void TileMapEditor::activate() {
	_map->reset();
	_mode = EM_EDIT_MAP;
	_currentBorder = 0;
	_levelIndex = 1;
	//_gui->activate("editor");
	_dialog->activate();
	updateLevelLabel();
	updateModeLabel();
}

void TileMapEditor::deactivate() {
	//_gui->deactivate("editor");
	_dialog->deactivate();
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
	ds::BasicMenuGameState::onButtonUp(button, x, y);
	int border = selectBorder(x, y);
	if (border != -1) {
		_currentBorder = border;
	}	
	else {
		v2 p = _map->convertToGridPos(x, y);
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
	
	graphics::setCamera(graphics::getOrthoCamera());
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	sprites->begin();
	for (int i = 0; i < 21; ++i) {
		v2 p(92 + i * 40, 680);
		if (i == _currentBorder) {
			sprites->draw(p, math::buildTexture(0, 88, 44, 44), 0.0f, v2(0.75f, 0.75f));
		}
		else {
			sprites->draw(p, math::buildTexture(0, 44, 44, 44), 0.0f, v2(0.75f, 0.75f));
		}
		sprites->draw(p, math::buildTexture(44, 44 * i, 44, 44), 0.0f, v2(0.75f, 0.75f));
	}
	// map
	_map->render();
	sprites->end();
	_dialog->render();
	//ds::renderer::drawDebugMessages();
	
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
	return 0;
}

int TileMapEditor::onGUIButton(int button) {
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
	if (button == 20) {
		++_levelIndex;
		if (_levelIndex >= MAX_LEVELS) {
			_levelIndex = MAX_LEVELS - 1;
		}
		updateLevelLabel();
	}
	if (button == 21) {
		--_levelIndex;
		if (_levelIndex < 1) {
			_levelIndex = 1;
		}
		updateLevelLabel();
	}
	if (button == 22) {
		if ( _mode == EM_BORDERS) {
			_mode = EM_EDIT_MAP;
			updateModeLabel();
		}
		else {
			_mode = EM_BORDERS;
			updateModeLabel();
		}
	}
	return button;
}
