#pragma once
#include "..\BaseApp.h"

// forward declarations
struct GameContext;
struct Tile;
class TileMap;
struct Block;

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
private:
	ButtonDefinition::Enum handleButtons();
	TileMap* _tiles;
	GameContext* _ctx;
	Block* _currentBlock;
	Block* _nextBlock;
	int _buttonDown[2];
};