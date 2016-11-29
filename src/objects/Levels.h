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
	const char* getName(int index) const;
private:
	int _blockSize;
	int _total;
	Tile* _tiles;
	char* _names;
};

