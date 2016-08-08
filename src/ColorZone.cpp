#include "ColorZone.h"
#include "gamestates\TestState.h"
#include "gamestates\TileMapEditor.h"

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
	settings->screenWidth = 1024;
	settings->screenHeight = 768;
	settings->clearColor = ds::Color(0, 0, 0, 255);
	settings->fullScreen = false;
	settings->reportingDirectory = "reports";
	settings->synched = true;
	settings->logTypes = ds::LogTypes::LT_CONSOLE | ds::LogTypes::LT_FILE;
}

bool ColorZone::loadContent() {
	_context.fillRate = 0;
	_context.levelIndex = 1;
	_context.score = 0;
	_context.resume = false;
	_context.gameMode = GM_TIMER;
	_context.name[0] = '\0';
	_context.filesystem.mount("levels");	
	addGameState(new TestState(&_context, game));
	addGameState(new TileMapEditor(&_context, game));
	_context.settings = new MyGameSettings;
	_context.settings->load();

	RID _material = ds::res::find("SpriteMaterial", ds::ResourceType::MATERIAL);
	ds::Material* m = ds::res::getMaterial(_material);
	m->texture = ds::res::find("TextureArray", ds::ResourceType::TEXTURE);

	return true;
}

void ColorZone::init() {
	activate("TileMapEditor");
}



void ColorZone::update(float dt) {
}

void ColorZone::render() {
}

