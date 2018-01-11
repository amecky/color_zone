#pragma once
#include "..\Common.h"

struct LevelData {
	int blockSize;
	int total;
	Tile* tiles;
	char* names;
};

bool load_levels(LevelData* data);

void copy_level(LevelData* data, int index, Tile* dest);

void update_level(LevelData* data, int index, Tile* src);

const char* get_level_name(LevelData* data, int index);

