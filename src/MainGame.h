#pragma once
#include "Constants.h"
#include "TileMap.h"
#include "Block.h"
#include <base\GameState.h>

class MainGame : public ds::GameState {

struct Laser {
	bool active;
	float timer;
	int column;
	ds::Texture texture;
};

public:
	MainGame(const char* name);
	~MainGame();
	void update(float dt);
	void render();
	void onButtonUp(int button, int x, int y);
	void onChar(int ascii);
	void activate();	
private:	
	void startLaser();
	std::unique_ptr<TileMap> _map;
	int _levelIndex;
	Block _previewBlock;
	Block _mainBlock;
	Laser _laser;
};
