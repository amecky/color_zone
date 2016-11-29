#include "Levels.h"

Levels::Levels() {
	_blockSize = MAX_X * MAX_Y;
	_total = _blockSize * MAX_LEVELS;
	_tiles = new Tile[_total];
	for (int i = 0; i < _total; ++i) {
		Tile& t = _tiles[i];
		t.borders = -1;
		t.color = -1;
		t.edges = -1;
		t.state.clear();
		t.state.set(BIT_AVAILABLE);
	}
	_names = new char[16 * MAX_LEVELS];
	char* tmp = _names;
	for (int i = 0; i < MAX_LEVELS; ++i) {
		sprintf(tmp, "Number %d", i + 1);
		tmp += 16;
	}
}

Levels::~Levels() {
	delete[] _names;
	delete[] _tiles;
}

void Levels::copy(int index, Tile* dest) {
	int all = MAX_X * MAX_Y;
	int idx = index * _blockSize;
	Tile* src = _tiles + idx;
	for (int i = 0; i < _blockSize; ++i) {
		dest[i] = src[i];
	}
}

void Levels::update(int index, Tile* src) {
	int all = MAX_X * MAX_Y;
	int idx = index * _blockSize;
	Tile* dest = _tiles + idx;
	for (int i = 0; i < _blockSize; ++i) {
		dest[i] = src[i];
	}
}

void Levels::load() {
	FILE* f = fopen("data\\block.lvl", "rb");
	if (f) {
		fread(_names, sizeof(char), 16 * MAX_LEVELS, f);
		Tile* dest = _tiles;
		for (int i = 0; i < _total; ++i) {
			fread(dest, sizeof(Tile), 1, f);
			++dest;
		}
		fclose(f);
	}
}

void Levels::save() {
	FILE* f = fopen("data\\block.lvl", "wb");
	Tile* dest = _tiles;
	fwrite(_names, sizeof(char), 16 * MAX_LEVELS, f);
	for (int i = 0; i < _total; ++i) {
		fwrite(dest, sizeof(Tile), 1, f);
		++dest;
	}
	fclose(f);
}

const char* Levels::getName(int index) const {
	int idx = index * 16;
	return _names + idx;
}