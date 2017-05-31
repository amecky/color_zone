#pragma once
#include <diesel.h>
#include "..\objects\Block.h"
#include "..\Common.h"
#include "..\objects\Levels.h"
#include "..\TileMap.h"
#include "..\objects\Laser.h"
#include "BasicGameState.h"
#include "..\utils\SparkleEffect.h"

class MainGameState : public BasicGameState {

public:
	MainGameState(GameContext* ctx);
	int tick(float dt, EventStream* stream);
	void activate();
	void deactivate();
	void render();
private:
	void moveLaser(float dt);
	int _buttonDown[2];
	Block _current;
	Block _next;
	Levels _levels;
	TileMap* _map;
	Laser* _laser;
	SparkleEffect* _effect;
};