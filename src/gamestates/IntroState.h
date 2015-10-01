#pragma once
#include "..\TileMap.h"
#include "..\Constants.h"
#include <base\GameState.h>
#include "..\SparkleEffect.h"

class IntroState : public ds::GameState {

public:
	IntroState(GameContext* context);
	~IntroState();
	int update(float dt);
	void render();
	void activate();
	int onChar(int ascii);
private:
	GameContext* _context;
	ds::Texture _texture;
	IntroEffect* _effect;
};

