#pragma once
#include "..\BaseApp.h"
#include "..\utils\GameTimer.h"

// forward declarations
struct GameContext;
struct Tile;
class TileMap;
struct Block;
class SparkleEffect;
class Laser;

struct ButtonDefinition {

	enum Enum {
		NONE,
		LEFT,
		RIGHT
	};

};

// -----------------------------------------------
// Main game scene
// -----------------------------------------------
class MainGameScene : public Scene {

public:
	MainGameScene(TileMap* tiles, GameContext* ctx);
	virtual ~MainGameScene();
	void render(SpriteBatchBuffer* buffer);
	void update(float dt);
	void onActivation();
	void showGUI();
private:
	void showHUD();
	ButtonDefinition::Enum handleButtons();
	TileMap* _tiles;
	GameContext* _ctx;
	Block* _currentBlock;
	Block* _nextBlock;
	int _buttonDown[2];
	TimerData _timer;
	SparkleEffect* _sparkleEffect;
	Laser* _laser;
	// debug
	int _dbgSparkCol;
};