#pragma once
#include "..\TileMap.h"
#include "..\Constants.h"
#include <gamestates\GameState.h>

class LevelSelectorState : public ds::BasicMenuGameState {

public:
	LevelSelectorState(GameContext* context);
	~LevelSelectorState();
	int update(float dt);
	void render();
	int onGUIButton(int button);
	void activate();
	void deactivate();
private:
	TileMap* _map;
	GameContext* _context;
	int _levels[MAX_LEVELS];
	int _numLevels;
	int _index;
};

