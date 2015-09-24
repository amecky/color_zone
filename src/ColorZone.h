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
#include "MainGame.h"

class ColorZone : public ds::BaseApp {

enum GameMode {
	GM_EDITOR,
	GM_MAIN
};

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
	void startGame();
	int _textureID;
	MapDefinition _level;
	int _edgeCount;
	std::unique_ptr<TileMapEditor> _editor;
	std::unique_ptr<MainGame> _game;
	GameMode _mode;
};

