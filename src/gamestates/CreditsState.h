#pragma once
#include "..\TileMap.h"
#include "..\Constants.h"
#include <gamestates\GameState.h>
#include "..\SparkleEffect.h"

class CreditsState : public ds::BasicMenuGameState {

public:
	CreditsState(GameContext* context);
	~CreditsState();
	int update(float dt);
	void render();
	void activate();
	void deactivate();
private:
	void updateText();
	GameContext* _context;
	int _index;
	float _timer;
};

