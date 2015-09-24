#pragma once
#include "Constants.h"
#include "TileMap.h"
#include "Block.h"

class MainGame {

struct Laser {
	bool active;
	float timer;
	int column;
	ds::Texture texture;
};

public:
	MainGame();
	~MainGame();
	void update(float dt);
	void render();
	void click(int button, int x, int y);
	void OnChar(int ascii);
	void start();	
private:	
	void startLaser();
	std::unique_ptr<TileMap> _map;
	int _levelIndex;
	Block _previewBlock;
	Block _mainBlock;
	Laser _laser;
};
