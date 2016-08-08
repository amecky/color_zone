#pragma once
#include "..\TileMap.h"
#include "..\Constants.h"
#include <gamestates\GameState.h>

class TileMapEditor : public ds::BasicMenuGameState {

enum EditorMode {
	EM_EDIT_MAP,
	EM_BORDERS
};

public:
	TileMapEditor(GameContext* context, ds::Game* game);
	~TileMapEditor();
	int update(float dt);
	void render();
	int onButtonUp(int button, int x, int y);
	int onChar(int ascii);
	void activate();
	void deactivate();
	int onGUIButton(int button);
private:
	void updateLevelLabel();
	void updateModeLabel();
	int selectBorder(int x, int y);
	EditorMode _mode;
	TileMap* _map;
	int _currentBorder;
	int _levelIndex;
	GameContext* _context;
};

