#include "ColorZone.h"
#include <SpriteBatchBuffer.h>
#include <ds_game_ui.h>
#include <Windows.h>
#include "..\resource.h"

BaseApp *app = new ColorZone();
// ---------------------------------------------------------------
// bitmap font definitions
// ---------------------------------------------------------------
static const ds::vec2 FONT_DEF[] = {
	ds::vec2(1,24),   // A
	ds::vec2(24,21),  // B
	ds::vec2(45,20),  // C
	ds::vec2(66,22),  // D
	ds::vec2(88,19),  // E
	ds::vec2(108,19), // F
	ds::vec2(127,21), // G
	ds::vec2(149,21), // H
	ds::vec2(170, 9), // I
	ds::vec2(179,13), // J
	ds::vec2(192,21), // K
	ds::vec2(213,19), // L
	ds::vec2(232,29), // M
	ds::vec2(261,21), // N
	ds::vec2(282,23), // O
	ds::vec2(305,21), // P
	ds::vec2(327,21), // Q
	ds::vec2(348,21), // R
	ds::vec2(369,19), // S 
	ds::vec2(388,19), // T
	ds::vec2(407,21), // U
	ds::vec2(428,24), // V
	ds::vec2(452,30), // W
	ds::vec2(482,23), // X
	ds::vec2(505,22), // Y
	ds::vec2(527,19)  // Z
};


ColorZone::ColorZone() : BaseApp() {
	_settings.screenWidth = 1280;
	_settings.screenHeight = 720;
	_settings.windowTitle = "ColorZone";
	_settings.clearColor = ds::Color(16,16,16,255);
}


ColorZone::~ColorZone() {
	delete _mainGameScene;
	delete _mapSelectionScene;
	delete _gameSettings;
}

// ---------------------------------------------------------------
// build the font info needed by the game ui
// ---------------------------------------------------------------
void prepareFontInfo(dialog::FontInfo* info) {
	// default for every character just empty space
	for (int i = 0; i < 255; ++i) {
		info->texture_rects[i] = ds::vec4(112, 4, 20, 19);
	}
	// numbers
	for (int c = 48; c <= 57; ++c) {
		int idx = (int)c - 48;
		info->texture_rects[c] = ds::vec4(548 + idx * 22, 440, 22, 19);
	}
	// :
	info->texture_rects[58] = ds::vec4(766, 440, 18, 19);
	// %
	info->texture_rects[37] = ds::vec4(788, 440, 36, 19);
	// characters
	for (int c = 65; c <= 90; ++c) {
		ds::vec2 fd = FONT_DEF[(int)c - 65];
		info->texture_rects[c] = ds::vec4(0.0f + fd.x, 440.0f, fd.y, 19.0f);
	}
}

// ---------------------------------------------------------------
// initialize
// ---------------------------------------------------------------
void ColorZone::initialize() {
	//
	// load the one and only texture
	// move to colorzone->init
	RID textureID = loadImageFromResource(MAKEINTRESOURCE(IDB_PNG1), "PNG");
	SpriteBatchBufferInfo sbbInfo = { 2048, textureID , ds::TextureFilters::LINEAR };
	_buffer = new SpriteBatchBuffer(sbbInfo);

	initializeSettings("content\\settings.json");
	_gameSettings = new GameSettings;
	loadSettings();

	dialog::FontInfo fontInfo;
	prepareFontInfo(&fontInfo);
	dialog::init(_buffer, fontInfo);

	_tiles = new TileMap(_buffer, _gameSettings);
	_tiles->buildLevel(0);

	_ctx.settings = _gameSettings;
	_ctx.pick_colors();
	_ctx.score = 0;
	_ctx.fillRate = 0;
	_ctx.pick_colors();
	_ctx.laserIdle = 10;

	_mapSelectionScene = new MapSelectionScene(_tiles, &_ctx);
	_mainGameScene = new MainGameScene(_tiles, &_ctx);
	setActiveScene(_mainGameScene);
}

// ---------------------------------------------------------------
// handle events
// ---------------------------------------------------------------
void ColorZone::handleEvents(ds::EventStream* events) {
	if (events->num() > 0) {
		for (uint32_t i = 0; i < events->num(); ++i) {
			int type = events->getType(i);
			if (type == 100) {
				setActiveScene(_mainGameScene);
			}
		}
	}
}
