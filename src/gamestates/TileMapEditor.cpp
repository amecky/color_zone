#include "TileMapEditor.h"
#include <core\log\Log.h>
#include <renderer\graphics.h>
#include <gamestates\GameStateMachine.h>
#include "..\objects\Levels.h"

TileMapEditor::TileMapEditor(GameContext* context) : ds::BasicMenuGameState("TileMapEditor", "editor"), _context(context) {
	_map = new TileMap(_context);
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
	_map->build(_levelIndex - 1);
	_dialog->activate();
	updateLevelLabel();
	updateModeLabel();
}

void TileMapEditor::deactivate() {
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
	if (y < 655 || y > 690) {
		return -1;
	}
	// x - start_x - icon_size / 2
	int xp = (x - 212) / 50;
	if (xp >= 0 && xp < 16) {
		return xp;
	}
	return -1;
}

// --------------------------------------------
// click
// --------------------------------------------
int TileMapEditor::onButtonUp(int button, int x, int y) {
	int btn = ds::BasicMenuGameState::onButtonUp(button, x, y);
	if (btn != 0) {
		return btn;
	}
	int border = selectBorder(x, y);
	if (border != -1) {
		_currentBorder = border;
	}	
	else {
		p2i p = map::screen2grid(x, y);
		if (_mode == EM_EDIT_MAP) {			
			if (button == 1) {
				_map->removeBlock(p);
			}
			else {
				_map->setBlock(p);
			}
		}
		else if (_mode == EM_BORDERS) {
			if (map::is_inside(x,y)) {
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
	graphics::setCamera(graphics::getOrthoCamera());
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	sprites->begin();
	for (int i = 0; i < 16; ++i) {
		v2 p(232 + i * 50, 670);
		if (i == _currentBorder) {
			sprites->draw(p, math::buildTexture(140, 320, 44, 44));
		}
		else {
			sprites->draw(p, math::buildTexture(140, 380, 44, 44));
		}
		sprites->draw(p, math::buildTexture(44, 44 * i, 44, 44));
	}
	// map
	_map->render();	
	_dialog->render();
	sprites->end();
}

void TileMapEditor::updateModeLabel() {
	if (_mode == EM_EDIT_MAP) {	
		_dialog->updateText(18, "Map");
	}
	else {
		_dialog->updateText(18, "BRD");
	}
}

void TileMapEditor::updateLevelLabel() {
	char buffer[32];
	sprintf_s(buffer, 32, "L:%2d", _levelIndex);
	_dialog->updateText(19, buffer);
	_dialog->updateText(16, _context->levels->getName(_levelIndex - 1));
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
		_map->save(_levelIndex - 1);
		_context->levels->save();
		return 0;
	}
	if (button == 3) {
		_context->levels->load();
		_map->build(_levelIndex - 1);
		return 0;
	}
	if (button == 4) {
		_map->reset();
		return 0;
	}
	if (button == 20) {
		++_levelIndex;
		if (_levelIndex > MAX_LEVELS) {
			_levelIndex = MAX_LEVELS;
		}
		updateLevelLabel();
		_map->build(_levelIndex - 1);
	}
	if (button == 21) {
		--_levelIndex;
		if (_levelIndex < 1) {
			_levelIndex = 1;
		}
		updateLevelLabel();
		_map->build(_levelIndex - 1);
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
