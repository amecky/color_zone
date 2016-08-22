#include "ColorZone.h"
#include "gamestates\TestState.h"
#include "gamestates\TileMapEditor.h"
#include "objects\Levels.h"

ds::BaseApp *app = new ColorZone();

ColorZone::ColorZone() {
	//_CrtSetBreakAlloc(12876);
	
}


ColorZone::~ColorZone() {
	delete _context.settings;
}

// -------------------------------------------------------
// prepare
// -------------------------------------------------------
void ColorZone::prepare(ds::Settings* settings) {
	settings->screenWidth = 1280;
	settings->screenHeight = 720;
	settings->clearColor = ds::Color(0, 0, 0, 255);
	settings->fullScreen = false;
	settings->reportingDirectory = "reports";
	settings->synched = true;
	settings->logTypes = ds::LogTypes::LT_CONSOLE | ds::LogTypes::LT_FILE;
}


// -------------------------------------------------------
// load content
// -------------------------------------------------------
bool ColorZone::loadContent() {
	_context.fillRate = 0;
	_context.levelIndex = 1;
	_context.score = 0;
	_context.resume = false;
	_context.name[0] = '\0';
	_context.settings = new GameSettings;
	_context.settings->load();
	_context.levels = new Levels;
	_context.levels->load();
	_context.spriteSheet = ds::res::getSpriteSheet("spritesheet");

	addGameState(new TestState(&_context, game));
	addGameState(new TileMapEditor(&_context, game));
	addGameState(new ds::BasicMenuGameState("MainMenu", "MainMenu", game));
	connectGameStates("TileMapEditor", 1, "TestState");
	connectGameStates("MainMenu", 1, "TestState");

	RID _material = ds::res::find("SpriteMaterial", ds::ResourceType::MATERIAL);
	ds::Material* m = ds::res::getMaterial(_material);
	m->texture = ds::res::find("TextureArray", ds::ResourceType::TEXTURE);

	//for (int i = 0; i < 20; ++i) {
		//int r = math::random(0, 3);
		//LOG << i << " = " << r;
	//}
	return true;
}

// -------------------------------------------------------
// init
// -------------------------------------------------------
void ColorZone::init() {
	activate("TestState");
}
