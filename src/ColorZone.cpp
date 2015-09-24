#include "ColorZone.h"
#include <sprites\SpriteBatch.h>
#include <base\GameStateMachine.h>
#include "BasicGameStates.h"

ds::BaseApp *app = new ColorZone();

ColorZone::ColorZone() {
	//_CrtSetBreakAlloc(12876);
	m_Width = 1024;
	m_Height = 768;
	//m_ClearColor = ds::Color(0.0f, 0.0f, 0.0f, 1.0f);
	//_editor = std::make_unique<TileMapEditor>();
	stateMachine->add<TileMapEditor>("TileMapEditor");
	stateMachine->add<MainGame>("MainGame");
	stateMachine->add(new StartMenuState(&gui));	
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
	//startGame();
	//gui.activate("MainMenu");
	stateMachine->activate("StartMenu");
	return true;
}

void ColorZone::startGame() {
	stateMachine->activate("MainGame");
	
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