#pragma once
#include "..\BaseApp.h"

struct GameContext;
struct Tile;
class TileMap;

class MapSelectionScene : public Scene {

public:
	MapSelectionScene(TileMap* tiles, GameContext* ctx) : Scene(), _tiles(tiles), _ctx(ctx) {}
	virtual ~MapSelectionScene() {}
	void render(SpriteBatchBuffer* buffer);
private:
	TileMap* _tiles;
	GameContext* _ctx;
};