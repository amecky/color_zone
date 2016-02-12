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
#include <data\DynamicSettingsEditor.h>

ds::BaseApp *app = new ColorZone();

ColorZone::ColorZone() {
	//_CrtSetBreakAlloc(12876);
	
}


ColorZone::~ColorZone() {
	delete _context.settings;
}

bool ColorZone::loadContent() {
	_context.fillRate = 0;
	_context.levelIndex = 1;
	_context.score = 0;
	_context.resume = false;
	_context.gameMode = GM_TIMER;
	_context.name[0] = '\0';
	_context.filesystem.mount("levels");
	addGameState(new TileMapEditor(gui, &_context));
	addGameState(new MainGame(&_context));
	addGameState(new LevelSelectorState(gui, &_context));
	addGameState(new ds::BasicMenuGameState("StartMenu", "MainMenu", gui));
	addGameState(new ds::BasicMenuGameState("GamePause", "Pause", gui));
	addGameState(new ds::BasicMenuGameState("Credits", "Credits", gui));
	addGameState(new InputNameState(gui, &_context));
	addGameState(new GameOverState(gui, &_context));
	addGameState(new HighscoreState(gui, &_context));
	addGameState(new IntroState(&_context));
	addGameState(new TestState(&_context));
	connectGameStates("StartMenu", 1, "LevelSelectorState");
	connectGameStates("StartMenu", 2, "TileMapEditor");
	connectGameStates("StartMenu", 4, "Credits");
	connectGameStates("StartMenu", 5, "HighscoreState");
	connectGameStates("StartMenu", 6, "InputNameState");
	connectGameStates("TileMapEditor", 1, "StartMenu");
	connectGameStates("LevelSelectorState", 1, "MainGame");
	connectGameStates("LevelSelectorState", 7, "StartMenu");
	connectGameStates("MainGame", 666, "GameOverState");
	connectGameStates("MainGame", 1, "GamePause");
	connectGameStates("GamePause", 1, "MainGame");
	connectGameStates("GameOverState", 1, "MainGame");
	connectGameStates("GameOverState", 2, "StartMenu");
	connectGameStates("Credits", 1, "StartMenu");
	connectGameStates("HighscoreState", 1, "StartMenu");
	connectGameStates("InputNameState", 1, "StartMenu");
	connectGameStates("IntroState", 1, "InputNameState");

	_context.settings = new MyGameSettings;
	_context.settings->load();

	assetEditors->add(new ds::DynamicSettingsEditor(_context.settings));

	_context.hud = gui->get("HUD");
	/*
	ds::GUIDialog* dlg = gui->get("MainMenu");
	dlg->setTransition(1, 3, 0.5f);
	dlg->setTransition(2, 2, 0.5f);
	dlg->setTransition(5, 1, 0.5f);
	dlg->setTransition(4, 2, 0.5f);
	dlg->setTransition(6, 1, 0.5f);
	dlg->setTransition(3, 4, 0.5f);
	*/
	return true;
}

void ColorZone::init() {
	activate("StartMenu");
}

void ColorZone::update(float dt) {
}

void ColorZone::draw() {
}

void ColorZone::onGUIButton(ds::DialogID dlgID, int button) {
	LOG << "dialog: " << dlgID << " button: " << button;
	if (dlgID == 3 && button == 3) {
		shutdown();
	}
}