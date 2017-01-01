#pragma once
#include "..\Constants.h"
#include "..\TileMap.h"
#include "..\objects\Block.h"
#include <gamestates\GameState.h>
#include "..\Common.h"
#include "..\SparkleEffect.h"
#include "..\objects\Laser.h"

class MainState : public ds::GameState {

	enum GameMode {
		GM_RUNNING,
		GM_OVER
	};

public:
	MainState(GameContext* context);
	~MainState();
	void init();
	int update(float dt);
	void render();
	int onButtonUp(int button, int x, int y);
	int onChar(int ascii);
	void activate();	
private:	
	void stopGame();
	void moveLaser(float dt);
	void fillHighscore();
	TileMap* _map;
	GameContext* _context;
	Block _previewBlock;
	Block _mainBlock;
	Laser* _laser;
	ds::GUIDialog* _hud;
	ds::GUIDialog* _gameOver;
	SparkleEffect* _effect;
	GameMode _mode;
};
