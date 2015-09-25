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
#include "SettingsConverter.h"

class ColorZone : public ds::BaseApp {

public:
	ColorZone();
	virtual ~ColorZone();
	const char* getTitle() {
		return "ColorZone";
	}
	bool loadContent();
	virtual void update(float dt);
	void draw();
	void onGUIButton(ds::DialogID dlgID, int button);
private:
	int _textureID;
	GameContext _context;
	SettingsLoader* _loader;
	//GameSettings* _settings;
};

