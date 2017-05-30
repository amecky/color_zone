#pragma once
#include <diesel.h>
#include "..\objects\Block.h"
#include <SpriteBatchBuffer.h>
#include "..\Common.h"
#include "..\objects\Levels.h"
#include "..\TileMap.h"

class MainGameState {

public:
	MainGameState(SpriteBatchBuffer* buffer, GameContext* ctx);
	void tick(float elapsed);
	void render();
private:
	bool _buttonDown;
	GameContext* _ctx;
	SpriteBatchBuffer* _buffer;
	Block _current;
	Block _next;
	Levels _levels;
	TileMap* _map;
};