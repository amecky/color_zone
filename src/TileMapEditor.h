#pragma once
#include "TileMap.h"
#include "Constants.h"

class TileMapEditor {

enum EditorMode {
	EM_EDIT_MAP,
	EM_BORDERS
};

public:
	TileMapEditor();
	~TileMapEditor();
	void update(float dt);
	void render();
	void click(int button, int x, int y);
	void OnChar(int ascii);
private:
	int selectBorder(int x, int y);
	EditorMode _mode;
	std::unique_ptr<TileMap> _map;
	int _currentBorder;
	int _levelIndex;
};

