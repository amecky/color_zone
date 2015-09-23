#pragma comment(lib, "Diesel2D.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr.lib")
#pragma warning(disable : 4995)

#pragma once
#include <base\BaseApp.h>
#include <io\Serializer.h>
#include "TileMap.h"
#include "TileMapEditor.h"

class ColorZone : public ds::BaseApp {

public:
	ColorZone();
	virtual ~ColorZone();
	const char* getTitle() {
		return "ColorZone";
	}
	bool loadContent();
	virtual void OnChar(char ascii, unsigned int keyState);
	virtual void update(float dt);
	void draw();
	virtual void OnButtonUp(int button, int x, int y);
private:
	int _textureID;
	MapDefinition _level;
	int _edgeCount;
	Edge _edges[128];
	TileMap* _map;
	std::unique_ptr<TileMapEditor> _editor;
};

