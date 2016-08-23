#include "ColorZone.h"
#include "gamestates\TestState.h"
#include "gamestates\GameOverState.h"
#include "gamestates\TileMapEditor.h"
#include "objects\Levels.h"
#include <utils\Assert.h>

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
	_context.pick_colors();
	_background = _context.spriteSheet->findIndex("background");
	addGameState(new TestState(&_context, game));
	addGameState(new TileMapEditor(&_context, game));
	addGameState(new GameOverState(&_context, game));
	addGameState(new ds::BasicMenuGameState("MainMenu", "MainMenu", game));
	connectGameStates("TileMapEditor", 1, "TestState");
	connectGameStates("MainMenu", 1, "TestState");
	connectGameStates("TestState", 1, "GameOver");
	connectGameStates("GameOver", 2, "MainMenu");

	RID _material = ds::res::find("SpriteMaterial", ds::ResourceType::MATERIAL);
	ds::Material* m = ds::res::getMaterial(_material);
	m->texture = ds::res::find("TextureArray", ds::ResourceType::TEXTURE);
	
	return true;
}

// -------------------------------------------------------
// init
// -------------------------------------------------------
void ColorZone::init() {
	activate("MainMenu");
}

void ColorZone::render() {
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	sprites->begin();
	sprites->draw(v2(640, 360), _context.spriteSheet->get(_background), 0.0f, v2(2, 2), _context.colors[7]);
	sprites->end();
}
