#include "ColorZone.h"
#include <sprites\SpriteBatch.h>
#include <base\GameStateMachine.h>
#include "gamestates\LevelSelectorState.h"
#include "gamestates\GameOverState.h"
#include "gamestates\HighscoreState.h"
#include "gamestates\InputNameState.h"
#include "gamestates\TileMapEditor.h"
#include "gamestates\MainGame.h"
#include "gamestates\IntroState.h"
#include "gamestates\TestState.h"

ds::BaseApp *app = new ColorZone();

ColorZone::ColorZone() {
	//_CrtSetBreakAlloc(12876);
	_settings.screenWidth = 1024;
	_settings.screenHeight = 768;
	_settings.clearColor = ds::Color(0.0f, 0.0f, 0.0f, 1.0f);
	_settings.showEditor = true;
	_context.fillRate = 0;
	_context.levelIndex = 1;
	_context.score = 0;
	_context.resume = false;
	_context.gameMode = GM_TIMER;
	_context.name = "";
	_context.filesystem.mount("levels");
	stateMachine->add(new TileMapEditor(&gui, &_context));
	stateMachine->add( new MainGame(&_context));
	stateMachine->add(new LevelSelectorState(&gui,&_context)); 
	stateMachine->add(new ds::BasicMenuGameState("StartMenu", "MainMenu", &gui));
	stateMachine->add(new ds::BasicMenuGameState("GamePause","Pause",&gui));
	stateMachine->add(new ds::BasicMenuGameState("Credits", "Credits", &gui));
	stateMachine->add(new InputNameState(&gui, &_context));
	stateMachine->add(new GameOverState(&gui,&_context));
	stateMachine->add(new HighscoreState(&gui, &_context));
	stateMachine->add(new IntroState(&_context));
	stateMachine->add(new TestState(&_context));
	stateMachine->connect("StartMenu", 1, "LevelSelectorState");
	stateMachine->connect("StartMenu", 2, "TileMapEditor");
	stateMachine->connect("StartMenu", 4, "Credits");
	stateMachine->connect("StartMenu", 5, "HighscoreState");
	stateMachine->connect("StartMenu", 6, "InputNameState");
	stateMachine->connect("TileMapEditor", 1, "StartMenu");
	stateMachine->connect("LevelSelectorState", 1, "MainGame");
	stateMachine->connect("LevelSelectorState", 7, "StartMenu");
	stateMachine->connect("MainGame", 666, "GameOverState");
	stateMachine->connect("MainGame", 1, "GamePause");
	stateMachine->connect("GamePause", 1, "MainGame");
	stateMachine->connect("GameOverState", 1, "MainGame");
	stateMachine->connect("GameOverState", 2, "StartMenu");
	stateMachine->connect("Credits", 1, "StartMenu");
	stateMachine->connect("HighscoreState", 1, "StartMenu");
	stateMachine->connect("InputNameState", 1, "StartMenu");
	stateMachine->connect("IntroState", 1, "InputNameState");
}


ColorZone::~ColorZone() {
}

bool ColorZone::loadContent() {
	_textureID = ds::renderer::loadTexture("Textures");
	assert(_textureID != -1);
	ds::debug::loadSystemFont("Verdana", "Verdana", 14, true);	
	ds::BitmapFont* font = ds::renderer::createBitmapFont("xscale");
	ds::assets::load("xscale", font, ds::CVT_FONT);
	ds::renderer::initializeBitmapFont(font, _textureID);
	ds::sprites::initializeTextSystem(_textureID, "xscale");
	gui::initialize();
	initializeGUI();
	_context.hud = gui.get("HUD");
	_loader = new SettingsLoader;
	uint32 convID = ds::assets::registerConverter(_loader);
	ds::assets::load("color_zone", _loader, convID);
	_context.settings = _loader->get();	
	ds::GUIDialog* dlg = gui.get("MainMenu");
	dlg->setTransition(1, 3, 0.5f);
	dlg->setTransition(2, 2, 0.5f);
	dlg->setTransition(4, 1, 0.5f);
	dlg->setTransition(3, 4, 0.5f);
	return true;
}

void ColorZone::init() {
	stateMachine->activate("StartMenu");
}

void ColorZone::update(float dt) {
}

void ColorZone::draw() {
	//ds::sprites::drawText(100,100,"Hello world");	
}

void ColorZone::onGUIButton(ds::DialogID dlgID, int button) {
	LOG << "dialog: " << dlgID << " button: " << button;
	if (dlgID == 3 && button == 3) {
		shutdown();
	}
}