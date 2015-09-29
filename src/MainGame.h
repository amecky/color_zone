#pragma once
#include "Constants.h"
#include "TileMap.h"
#include "Block.h"
#include <base\GameState.h>
#include "Common.h"
#include "SparkleEffect.h"
#include <objects\HUD.h>

class MainGame : public ds::GameState {

struct Laser {
	bool active;
	float timer;
	int column;
	ds::Texture texture;
};

public:
	MainGame(GameContext* context);
	~MainGame();
	void init();
	int update(float dt);
	void render();
	int onButtonUp(int button, int x, int y);
	int onChar(int ascii);
	void activate();	
private:	
	void moveLaser(float dt);
	void startLaser();
	void fillHighscore();
	std::unique_ptr<TileMap> _map;
	GameContext* _context;
	Block _previewBlock;
	Block _mainBlock;
	Laser _laser;
	SparkleEffect* _effect;
	ds::HUD _hud;
};
