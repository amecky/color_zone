#pragma once
#include "Common.h"
#include <stdint.h>
#include <vector>
#include "objects/Block.h"
#include "objects/Levels.h"
#include <SpriteBatchBuffer.h>
#include <ds_imgui.h>


// -------------------------------------------------------------
// Tile map
// -------------------------------------------------------------
class TileMap {

	struct Border {
		p2i pos;
		int type;
		bool inside;

		Border() : pos(0, 0), type(0), inside(true) {}
	};

public:	
	TileMap();
	~TileMap() {}
	void render(SpriteBatchBuffer* buffer, ds::Color* colors, GameSettings* settings);
	void render(SpriteBatchBuffer* buffer, int squareSize, float scale, ds::Color* colors, GameSettings* settings);
	const uint32_t getIndex(uint32_t x, uint32_t y) const;
	Tile& get(int x, int y);
	Tile& get(const p2i& p);
	const Tile& get(int x, int y) const;
	void set(int x, int y, const Tile& tile);
	const bool isValid(int x, int y) const;
	const bool isValid(const p2i& p) const;
	bool isCoherent(int gx, int gy);
	bool isBlockAvailable(int gx, int gy);
	bool isAvailable(int gx, int gy);
	bool isFree(int gx, int gy);
	void reset();
	void setBorder(int x, int y, int index);
	
	bool copyBlock(const Block* block);
	int clearColumn(int col);
	int getFillRate();
	void getColumn(int col,int* colors);

	void removeBlock(const p2i& gridPos);
	void setBlock(const p2i& gridPos);

	void build(LevelData* levels, int index);

	void debug();

private:	
	bool matches(int x, int y, const Tile& t);
	//void check(int xp, int yp, int lastDir, PointList& list, bool rec);
	int determineEdge(int x, int y, const Tile& t);
	void determineEdges();
	void setState(int x, int y, int index);
	Tile* _tiles;
	int _maxAvailable;
	std::vector<Border> _border;

	int _tile;
	int _markedTile;
	int _filledTile;
	int _basicBorder;
};

namespace map {

	p2i screen2grid(int x, int y);

	p2i screen2grid(const ds::vec2& p);

	p2i grid2screen(const p2i& p);

	bool isOuterRim(const p2i& p);

	bool is_inside(int x, int y);

	//p2i screen2grid(int x, int y);

}

