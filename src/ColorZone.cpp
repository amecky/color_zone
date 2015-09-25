#include "ColorZone.h"
#include <sprites\SpriteBatch.h>
#include <base\GameStateMachine.h>
#include "BasicGameStates.h"

ds::BaseApp *app = new ColorZone();

ColorZone::ColorZone() {
	//_CrtSetBreakAlloc(12876);
	m_Width = 1024;
	m_Height = 768;
	_context.fillRate = 0;
	_context.levelIndex = 2;
	_context.score = 0;
	m_ClearColor = ds::Color(0.0f, 0.0f, 0.0f, 1.0f);
	stateMachine->add<TileMapEditor>("TileMapEditor");
	stateMachine->add( new MainGame(&_context));
	stateMachine->add(new StartMenuState(&gui));	
	stateMachine->add(new GamePauseState(&gui));
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
	stateMachine->activate("GamePause");
	return true;
}

void ColorZone::update(float dt) {
}

void ColorZone::draw() {
}

void ColorZone::onGUIButton(ds::DialogID dlgID, int button) {
	if (dlgID == 1 && button == 1) {
		shutdown();
	}
}