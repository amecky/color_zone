#pragma comment(lib, "D11.lib")
#pragma comment(lib, "DieselCore.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr.lib")
#pragma warning(disable : 4995)

#pragma once
#include <base\BaseApp.h>
#include "Common.h"

class ColorZone : public ds::BaseApp {

public:
	ColorZone();
	virtual ~ColorZone();
	const char* getTitle() {
		return "ColorZone";
	}
	bool initialize() {
		return true;
	}
	void init();
	bool loadContent();
	void update(float dt);
	void render();
	void onShutdown();
private:
	void prepare(ds::Settings* settings);
	GameContext _context;
};

