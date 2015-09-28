#pragma once
#include "TileMap.h"
#include "Constants.h"
#include <base\GameState.h>

class LevelSelectorState : public ds::GameState {

public:
	LevelSelectorState(ds::DialogManager* gui,GameContext* context);
	~LevelSelectorState();
	int update(float dt);
	void render();
	int onGUIButton(ds::DialogID dlgID, int button);
	void activate();
	void deactivate();
private:
	std::unique_ptr<TileMap> _map;
	GameContext* _context;
	ds::DialogManager* _gui;
};

