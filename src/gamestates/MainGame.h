#pragma once
#include "..\Constants.h"
#include "..\TileMap.h"
#include "..\objects\Block.h"
#include "..\objects\Laser.h"
#include <gamestates\GameState.h>
#include "..\Common.h"
#include "..\SparkleEffect.h"

class MainGame : public ds::GameState {

public:
	MainGame(GameContext* context, ds::Game* game);
	~MainGame();
	void init();
	int update(float dt);
	void render();
	int onButtonUp(int button, int x, int y);
	int onChar(int ascii);
	void activate();	
	void deactivate();
private:	
	void moveLaser(float dt);
	void fillHighscore();
	TileMap* _map;
	GameContext* _context;
	Block _previewBlock;
	Block _mainBlock;
	Laser* _laser;
	SparkleEffect* _effect;
};
