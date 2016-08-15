#pragma once
#include "..\Common.h"

class Levels {

public:
	Levels();
	~Levels();
	void copy(int index, Tile* dest);
	void update(int index, Tile* src);
	void load();
	void save();
private:
	int _blockSize;
	int _total;
	Tile* _tiles;
};

