#pragma once
#include "..\TileMap.h"
#include "..\Constants.h"
#include <gamestates\GameState.h>

class GameOverState : public ds::BasicMenuGameState {

public:
	GameOverState(GameContext* context, ds::Game* game);
	~GameOverState();
	void activate();
	void deactivate();
private:
	GameContext* _context;
};

