#include "TileMapEditor.h"
#include <utils\Log.h>
#include <sprites\SpriteBatch.h>

TileMapEditor::TileMapEditor(int width, int height) : _width(width), _height(height) {
	_map = std::make_unique<TileMap>(width, height);
	_map->reset();
	_mode = EM_EDIT_MAP;
	for (int y = 0; y < MAX_Y; ++y) {
		for (int x = 0; x < MAX_X; ++x) {
			_borders[x][y] = -1;
		}
	}
	_currentBorder = 0;
}


TileMapEditor::~TileMapEditor() {
	LOG << "deleting editor";
}

void TileMapEditor::update(float dt) {

}

void TileMapEditor::click(int button, int x, int y) {
	LOG << "button: " << button;
	if (_mode == EM_EDIT_MAP) {
		int xp = (x - START_X + SQUARE_SIZE / 2) / SQUARE_SIZE;
		int yp = ((768 - y) - START_Y + SQUARE_SIZE / 2) / SQUARE_SIZE;
		LOG << "clicked at: " << xp << " " << yp;
		if (_map->isValid(xp, yp)) {
			Tile& t = _map->get(xp, yp);
			if (button == 1) {
				t.used = false;
				t.state = TS_EMPTY;
			}
			else {
				t.used = true;
				t.state = TS_AVAILABLE;
			}
		}
	}
	else if (_mode == EM_BORDERS) {
		int xp = (x - START_X + SQUARE_SIZE / 2) / SQUARE_SIZE;
		int yp = ((768 - y) - START_Y + SQUARE_SIZE / 2) / SQUARE_SIZE;
		LOG << "clicked at: " << xp << " " << yp;
		if (_map->isValid(xp, yp)) {
			if (button == 0) {
				_borders[xp][yp] = _currentBorder;
			}			
			else {
				_borders[xp][yp] = -1;
			}
		}
	}
}

void TileMapEditor::render() {
	// draw icons
	// draw border selection
	//for (int i = 0; i < 15; ++i) {
		v2 p(100, 720);
		ds::sprites::draw(p, ds::math::buildTexture(0, 44 , 44, 44));
		ds::sprites::draw(p, ds::math::buildTexture(44, 44 * _currentBorder,44,44));		
	//}
	_map->render();
	for (int y = 0; y < MAX_Y; ++y) {
		for (int x = 0; x < MAX_X; ++x) {
			if (_borders[x][y] != -1) {
				v2 p = v2(START_X + x * SQUARE_SIZE, START_Y + y * SQUARE_SIZE);
				int w = 44;
				int h = 44;
				ds::sprites::draw(p, ds::math::buildTexture(44, 44 * _borders[x][y], w, h));
					
			}
		}
	}
}

void TileMapEditor::OnChar(int ascii) {
	if (ascii == '1') {
		_mode = EM_EDIT_MAP;
		LOG << "Map edit modus";
	}
	if (ascii == '2') {
		_mode = EM_BORDERS;
		LOG << "Border edit modus";
	}
	if (ascii == '+') {
		++_currentBorder;
		if (_currentBorder > 22) {
			_currentBorder = 0;
		}
	}
	if (ascii == '-') {
		--_currentBorder;
		if (_currentBorder < 0) {
			_currentBorder = 22;
		}
	}	
	LOG << " index: " << _currentBorder;
}