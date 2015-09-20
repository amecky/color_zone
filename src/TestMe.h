#pragma comment(lib, "Diesel2D.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr.lib")
#pragma warning(disable : 4995)

#pragma once
#include <base\BaseApp.h>
#include <io\Serializer.h>
#include "TileMap.h"

class TestMe : public ds::BaseApp {

public:
	TestMe();
	virtual ~TestMe();
	const char* getTitle() {
		return "Blocks::TestMe";
	}
	//void initialize();
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
};

