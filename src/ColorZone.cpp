#include "ColorZone.h"
#include "gamestates\TestState.h"
#include "gamestates\TileMapEditor.h"
#include "objects\Levels.h"
#include <utils\Assert.h>
#include <io\Huffmann.h>

ds::BaseApp *app = new ColorZone();

ColorZone::ColorZone() {
	//_CrtSetBreakAlloc(12876);
	_timer = 0.0f;
	_ttl = 1.0f;
	_minValue = 0.6f;
	_maxValue = 0.8f;
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
	addGameState(new ds::BasicMenuGameState("MainMenu", "MainMenu", game));
	connectGameStates("TileMapEditor", 1, "TestState");
	connectGameStates("MainMenu", 1, "TestState");
	connectGameStates("TestState", 22, "MainMenu");
	connectGameStates("TestState", 21, "TestState");

	RID _material = ds::res::find("SpriteMaterial", ds::ResourceType::MATERIAL);
	ds::Material* m = ds::res::getMaterial(_material);
	m->texture = ds::res::find("TextureArray", ds::ResourceType::TEXTURE);
	
	_maxValue = math::random(_context.settings->background.minIntensity, _context.settings->background.maxIntensity);

	ds::huffmann::first("this is an example of a huffman tree");
	ds::huffmann::decompress("test.huf");
	return true;
}

// -------------------------------------------------------
// init
// -------------------------------------------------------
void ColorZone::init() {
	renewBackgroundSettings();
	activate("TestState");
}

// -------------------------------------------------------
// update
// -------------------------------------------------------
void ColorZone::update(float dt) {
	_timer += dt;
	float norm = _timer / _ttl;
	if (norm >= 1.0f) {		
		renewBackgroundSettings();
		LOG << "ttl: " << _ttl << " min: " << _minValue << " max: " << _maxValue;
	}
}

// -------------------------------------------------------
// renew background settings
// -------------------------------------------------------
void ColorZone::renewBackgroundSettings() {
	_timer = 0.0f;
	_ttl = math::random(_context.settings->background.minTTL, _context.settings->background.maxTTL);
	_minValue = _maxValue;
	_maxValue = math::random(_context.settings->background.minIntensity, _context.settings->background.maxIntensity);
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void ColorZone::render() {
	ds::Color c = _context.colors[7];
	float norm = _timer / _ttl;
	c.a = tweening::interpolate(tweening::linear, _minValue, _maxValue, _timer, _ttl);
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	sprites->draw(v2(640, 360), _context.spriteSheet->get(_background), 0.0f, v2(2, 2), c);
}
