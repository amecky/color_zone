#pragma once
#include "..\TileMap.h"
#include "..\Constants.h"
#include <gamestates\GameState.h>

class GameOverState : public ds::GameState {

public:
	GameOverState(GameContext* context);
	~GameOverState();
	int update(float dt);
	void render();
	int onGUIButton(int button);
	void activate();
	void deactivate();
private:
	GameContext* _context;
};

