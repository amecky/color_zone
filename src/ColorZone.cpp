#include "ColorZone.h"
#include <sprites\SpriteBatch.h>
#include <base\GameStateMachine.h>
#include "LevelSelectorState.h"
#include "GameOverState.h"

ds::BaseApp *app = new ColorZone();

ColorZone::ColorZone() {
	//_CrtSetBreakAlloc(12876);
	m_Width = 1024;
	m_Height = 768;
	_context.fillRate = 0;
	_context.levelIndex = 1;
	_context.score = 0;
	_context.resume = false;
	_context.gameMode = GM_TIMER;
	_context.filesystem.mount("levels");
	m_ClearColor = ds::Color(0.0f, 0.0f, 0.0f, 1.0f);
	stateMachine->add(new TileMapEditor());
	stateMachine->add( new MainGame(&_context));
	stateMachine->add(new LevelSelectorState(&gui,&_context)); 
	stateMachine->add(new ds::BasicMenuGameState("StartMenu", "MainMenu", &gui));
	stateMachine->add(new ds::BasicMenuGameState("GamePause","Pause",&gui));
	stateMachine->add(new GameOverState(&gui,&_context));
	stateMachine->connect("StartMenu", 1, "LevelSelectorState");
	stateMachine->connect("StartMenu", 2, "TileMapEditor");
	stateMachine->connect("TileMapEditor", 1, "StartMenu");
	stateMachine->connect("LevelSelectorState", 1, "MainGame");
	stateMachine->connect("MainGame", 666, "GameOverState");
	stateMachine->connect("MainGame", 1, "GamePause");
	stateMachine->connect("GamePause", 1, "MainGame");
	stateMachine->connect("GameOverState", 1, "MainGame");
	stateMachine->connect("GameOverState", 2, "StartMenu");
}


ColorZone::~ColorZone() {
}

bool ColorZone::loadContent() {
	_textureID = ds::renderer::loadTexture("Textures");
	assert(_textureID != -1);
	ds::debug::loadSystemFont("Verdana", "Verdana", 14, true);	
	ds::BitmapFont* font = ds::renderer::createBitmapFont("xscale");
	ds::assets::load("xscale", font, ds::CVT_FONT);
	ds::renderer::initializeBitmapFont(*font, _textureID);
	initializeGUI();
	_loader = new SettingsLoader;
	uint32 convID = ds::assets::registerConverter(_loader);
	ds::assets::load("color_zone", _loader, convID);
	_context.settings = _loader->get();	
	return true;
}

void ColorZone::init() {
	stateMachine->activate("LevelSelectorState");
}

void ColorZone::update(float dt) {
}

void ColorZone::draw() {
}

void ColorZone::onGUIButton(ds::DialogID dlgID, int button) {
	if (dlgID == 1 && button == 3) {
		shutdown();
	}
}