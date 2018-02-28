#pragma once
#include "..\BaseApp.h"

struct GameContext;
struct Tile;
class TileMap;

class HighscoreScene : public Scene {

public:
	HighscoreScene(TileMap* tiles, GameContext* ctx) : Scene(), _tiles(tiles), _ctx(ctx) {}
	virtual ~HighscoreScene() {}
	void render(SpriteBatchBuffer* buffer);
private:
	TileMap* _tiles;
	GameContext* _ctx;
};