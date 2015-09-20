#pragma once
#include "Common.h"

class TileMap {

typedef std::vector<Edge> Edges;

public:	
	TileMap(int w, int h) : _tiles(0), _width(w), _height(h) {
		_tiles = new Tile[w * h];
	}
	~TileMap() {
		if (_tiles) {
			delete[] _tiles;
		}
	}
	bool loadTextFile(int nr);
	void render();
	const uint32 getIndex(uint32 x, uint32 y) const;
	Tile& get(int x, int y);
	const Tile& get(int x, int y) const;
	void set(int x, int y, const Tile& tile);
	const bool isValid(int x, int y) const;
	bool isCoherent(int gx, int gy);
	bool isBlockAvailable(int gx, int gy);
	bool isAvailable(int gx, int gy);
	bool isFree(int gx, int gy);
private:
	int determineEdge(int x, int y);
	int determineOuterEdge(int x, int y);
	void determineEdges();
	void addEdge(int x, int y, int offsetX, int offsetY, const ds::Texture& t);
	v2 convert(int x, int y);
	int _width;
	int _height;
	Tile* _tiles;
	Edges _edges;
};

