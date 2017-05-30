#include "Levels.h"
#include "..\Common.h"
#include <string.h>
#include <windows.h>
#include "..\resource.h"

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

	HRSRC myResource = FindResource(NULL, MAKEINTRESOURCE(IDR_BINARY1), RT_RCDATA);
	HGLOBAL myResourceData = ::LoadResource(NULL, myResource);
	void* pMyBinaryData = ::LockResource(myResourceData);
	memcpy(_names, pMyBinaryData, 16 * MAX_LEVELS * sizeof(char));
	const char* n = (char*)pMyBinaryData + 16 * MAX_LEVELS * sizeof(char);
	memcpy(_tiles, n, _total * sizeof(Tile));
}

Levels::~Levels() {
	delete[] _names;
	delete[] _tiles;
}

void Levels::copy(int index, Tile* dest) const {
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

const char* Levels::getName(int index) const {
	int idx = index * 16;
	return _names + idx;
}