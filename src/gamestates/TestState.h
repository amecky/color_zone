#pragma once
#include "..\Constants.h"
#include "..\TileMap.h"
#include "..\objects\Block.h"
#include <gamestates\GameState.h>
#include "..\Common.h"
#include "..\SparkleEffect.h"
#include "..\objects\Laser.h"
#include "..\objects\HUD.h"

class TestState : public ds::GameState {

public:
	TestState(GameContext* context, ds::Game* game);
	~TestState();
	void init();
	int update(float dt);
	void render();
	int onButtonUp(int button, int x, int y);
	int onChar(int ascii);
	void activate();	
private:	
	void moveLaser(float dt);
	void fillHighscore();
	TileMap* _map;
	GameContext* _context;
	Block* _previewBlock;
	Block* _mainBlock;
	Laser* _laser;
	HUD* _hud;
	SparkleEffect* _effect;
};
