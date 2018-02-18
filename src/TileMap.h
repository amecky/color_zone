#pragma once
#include <stdint.h>

// -------------------------------------------------------------
// forward declarations
// -------------------------------------------------------------
struct p2i;
struct GameSettings;
class SpriteBatchBuffer;
struct Tile;
struct LevelData;
struct Block;
class PointList;
namespace ds {
	struct Color;
	struct vec2;
}

// -------------------------------------------------------------
// Tile map
// -------------------------------------------------------------
class TileMap {

public:	
	TileMap(SpriteBatchBuffer* buffer, GameSettings* settings);
	~TileMap();
	void buildLevel(int lvl);
	void render(ds::Color* colors, float scale = 1.0f);
	const char* getLevelName() const;
	int getLevel() const;
	bool copyBlock(const Block* block);
	void resetLevel();
	int clearColumn(int col);
	//int getFillRate();
	void getColor(int col,int* colors);
	void showDebugGUI();
private:	
	bool isValid(int x, int y) const;
	bool isValid(const p2i& p) const;
	bool isBlockAvailable(int gx, int gy) const;
	bool isAvailable(int gx, int gy) const;
	bool isFree(int gx, int gy) const;
	uint32_t getIndex(uint32_t x, uint32_t y) const;
	Tile& get(int x, int y);
	Tile& get(const p2i& p);
	const Tile& get(int x, int y) const;
	void set(int x, int y, const Tile& tile);
	void setState(int x, int y, int index);
	bool isCoherent(int gx, int gy) const;
	bool matches(int x, int y, const Tile& t);
	void check(int xp, int yp, int lastDir, PointList& list, bool rec);
	int determineEdge(int x, int y, const Tile& t);
	void determineEdges();
	//void setState(int x, int y, int index);
	int _currentLevel;
	Tile* _tiles;
	SpriteBatchBuffer* _sprites;
	GameSettings* _settings;
	LevelData* _levelData;
	int _guiState;
	//int _maxAvailable;
	//int _tile;
	//int _markedTile;
	//int _filledTile;
	//int _basicBorder;
};

namespace map {

	p2i screen2grid(int x, int y);

	p2i screen2grid(const ds::vec2& p);

	p2i grid2screen(const p2i& p);

	bool isOuterRim(const p2i& p);

	bool is_inside(int x, int y);

	//p2i screen2grid(int x, int y);

}

