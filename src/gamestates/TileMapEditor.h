#pragma once
#include "..\TileMap.h"
#include "..\Constants.h"
#include <base\GameState.h>

class TileMapEditor : public ds::GameState {

enum EditorMode {
	EM_EDIT_MAP,
	EM_BORDERS
};

public:
	TileMapEditor(ds::DialogManager* gui, GameContext* context);
	~TileMapEditor();
	int update(float dt);
	void render();
	int onButtonUp(int button, int x, int y);
	int onChar(int ascii);
	void activate();
	void deactivate();
	int onGUIButton(ds::DialogID dlgID, int button);
private:
	int selectBorder(int x, int y);
	EditorMode _mode;
	std::unique_ptr<TileMap> _map;
	int _currentBorder;
	int _levelIndex;
	ds::DialogManager* _gui;
	GameContext* _context;
};

