#include "Levels.h"
#include "..\Common.h"
#include <string.h>
#include <windows.h>
#include "..\resource.h"

bool load_levels(LevelData* data) {
	data->blockSize = MAX_X * MAX_Y;
	data->total = data->blockSize * MAX_LEVELS;
	data->tiles = new Tile[data->total];
	for (int i = 0; i < data->total; ++i) {
		Tile& t = data->tiles[i];
		t.borders = -1;
		t.color = -1;
		t.edges = -1;
		t.state.clear();
		t.state.set(BIT_AVAILABLE);
	}
	data->names = new char[16 * MAX_LEVELS];
	char* tmp = data->names;
	//for (int i = 0; i < MAX_LEVELS; ++i) {
		//sprintf(tmp, "Number %d", i + 1);
		//tmp += 16;
	//}

	HRSRC myResource = FindResource(NULL, MAKEINTRESOURCE(IDR_BINARY1), RT_RCDATA);
	HGLOBAL myResourceData = ::LoadResource(NULL, myResource);
	void* pMyBinaryData = ::LockResource(myResourceData);
	memcpy(data->names, pMyBinaryData, 16 * MAX_LEVELS * sizeof(char));
	const char* n = (char*)pMyBinaryData + 16 * MAX_LEVELS * sizeof(char);
	memcpy(data->tiles, n, data->total * sizeof(Tile));
	UnlockResource(myResourceData);
	FreeResource(myResourceData);
	return true;
}

void copy_level(LevelData* data, int index, Tile* dest) {
	int all = MAX_X * MAX_Y;
	int idx = index * data->blockSize;
	Tile* src = data->tiles + idx;
	for (int i = 0; i < data->blockSize; ++i) {
		dest[i] = src[i];
	}
}

void update_level(LevelData* data, int index, Tile* src) {
	int all = MAX_X * MAX_Y;
	int idx = index * data->blockSize;
	Tile* dest = data->tiles + idx;
	for (int i = 0; i < data->blockSize; ++i) {
		dest[i] = src[i];
	}
}

const char* get_level_name(LevelData* data,int index) {
	int idx = index * 16;
	return data->names + idx;
}
