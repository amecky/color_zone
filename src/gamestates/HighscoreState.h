#pragma once
#include "..\TileMap.h"
#include "..\Constants.h"
#include <gamestates\GameState.h>
#include "..\utils\HighscoreService.h"

class HighscoreState : public ds::BasicMenuGameState {

public:
	HighscoreState(GameContext* context);
	~HighscoreState();
	int update(float dt);
	void render();
	int onGUIButton(int button);
	void activate();
	void deactivate();
private:
	void updateText();
	GameContext* _context;
	int _index;
};

