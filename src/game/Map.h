#pragma once
#include "lib\container\GridArray.h"
#include "LevelManager.h"
#include <math\Bitset.h>
#include <base\GameObject.h>
#include <vector>
#include <lib\DataArray.h>

#include "..\Common.h"

class Block;

class Map : public ds::GameObject {



struct Border {

	Vector2f position;
	ds::Rect textureRect;
};

typedef std::vector<Border> BorderPieces;

public:
	Map();
	~Map(void);
	void init();
	void update( float elapsed );
	bool addBlock(const Vector2f& pos,int firstColor,int secondColor,int thirdColor,int fourthColor);
	void cleanMarkedBlocks();
	int cleanRow(int row);
	void reset(const Level& level,Block* block);
	int getCoverage();
	void clear();
	void render();
	void debug() {
		map::debug(m_Tiles);
	}
private:
	void setEntryCorners(int x,int y);
	bool isValidEntry(int x,int y);
	int checkSurrounding(int xp,int yp);
	void check(int xp,int yp,int lastDir,std::vector<Vector2f> *list,bool rec);
	void markBlocks();
	void markSingleBlock(int gx,int gy);
	bool markSingleEntry(int gx,int gy);
	void setColor(int gx,int gy,int color,bool filled = false);
	//
	BorderPieces m_BorderPieces;
	TileMap m_Tiles;
	int m_TotalTiles;
	float m_CleanTimer;


};

