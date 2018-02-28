#pragma once
#include "..\BaseApp.h"

struct GameContext;
struct Tile;
class TileMap;

class GameOverScene : public Scene {

public:
	GameOverScene(TileMap* tiles, GameContext* ctx) : Scene(), _tiles(tiles), _ctx(ctx) {}
	virtual ~GameOverScene() {}
	void render(SpriteBatchBuffer* buffer);
private:
	TileMap* _tiles;
	GameContext* _ctx;
};