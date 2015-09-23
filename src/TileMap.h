#pragma once
#include "Common.h"
#include <utils\Log.h>

class TileMap {

typedef std::vector<Edge> Edges;

public:	
	TileMap(int w, int h) : _tiles(nullptr) , _width(w), _height(h) {
		_tiles = std::make_unique<Tile[]>(w * h);
	}
	~TileMap() {
		LOG << "deleting TileMap";		
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
	void reset();
	void save(const char* levelname, int index);
private:
	int determineEdge(int x, int y);
	void determineEdges();
	v2 convert(int x, int y);
	int _width;
	int _height;
	std::unique_ptr<Tile[]> _tiles;
	Edges _edges;
};

