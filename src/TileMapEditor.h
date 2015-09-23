#pragma once
#include "TileMap.h"
#include "Constants.h"

class TileMapEditor {

enum EditorMode {
	EM_EDIT_MAP,
	EM_BORDERS
};

public:
	TileMapEditor(int width,int height);
	~TileMapEditor();
	void update(float dt);
	void render();
	void click(int button, int x, int y);
	void OnChar(int ascii);
private:
	int _width;
	int _height;
	EditorMode _mode;
	std::unique_ptr<TileMap> _map;
	int _borders[MAX_X][MAX_Y];
	int _currentBorder;
};

