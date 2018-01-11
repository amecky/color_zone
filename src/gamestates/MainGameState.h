#pragma once
#include <diesel.h>
#include "..\objects\Block.h"
#include "..\Common.h"
#include "..\objects\Levels.h"
#include "..\TileMap.h"
#include "..\objects\Laser.h"
#include "..\utils\SparkleEffect.h"



class MainGameState {

public:
	MainGameState(GameContext* ctx);
	int tick(float dt);
	void activate();
	void deactivate();
	void render();
private:
	GameContext* _ctx;
	void moveLaser(float dt);
	int _buttonDown[2];
	Block _current;
	Block _next;
	Laser* _laser;
	SparkleEffect* _effect;
	
};