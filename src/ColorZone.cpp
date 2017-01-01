#include "ColorZone.h"
#include "gamestates\MainState.h"
#include "gamestates\TileMapEditor.h"
#include "gamestates\LevelSelectorState.h"
#include "gamestates\HighscoreState.h"
#include "gamestates\CreditsState.h"
#include "gamestates\GameOverState.h"
#include "gamestates\UserNameState.h"
#include "objects\Levels.h"
#include <core\base\Assert.h>
#include <core\io\Huffmann.h>
#include <core\io\FileRepository.h>
#include "audio\AudioManager.h"
#include "utils\FadingBackgroundPlugin.h"

ds::BaseApp *app = new ColorZone();

ColorZone::ColorZone() {
	//_CrtSetBreakAlloc(12876);
}


ColorZone::~ColorZone() {
	delete _context.highscoreService;
	delete _context.levels;
	delete _context.settings;
}

// -------------------------------------------------------
// prepare
// -------------------------------------------------------
void ColorZone::prepare(ds::Settings* settings) {
	//settings->repositoryMode = ds::repository::RM_RELEASE;
	settings->screenWidth = 1280;
	settings->screenHeight = 720;
	settings->clearColor = ds::Color(0, 0, 0, 255);
	settings->fullScreen = false;
	settings->reportingDirectory = "reports";
	settings->synched = true;
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
	_context.levelIndex = 0;
	_context.spriteSheet = ds::res::getSpriteSheet("spritesheet");
	_context.pick_colors();
	_context.highscoreService = new HighscoreService;
	_context.highscoreService->load();

	ds::plugins::add(new FadingBackgroundPlugin(&_context));
	ds::UserNameDialogSettings ids;
	ids.dialogName = "username";
	ids.textID = 111;
	ids.textPosition = v2(540, 360);
	ids.cursorID = 3;
	ids.eventID = 200;

	addGameState(new MainState(&_context));
	addGameState(new TileMapEditor(&_context));
	addGameState(new LevelSelectorState(&_context));
	addGameState(new HighscoreState(&_context));
	addGameState(new CreditsState(&_context));
	addGameState(new GameOverState(&_context));
	addGameState(new ds::BasicMenuGameState("MainMenu", "MainMenu"));
	addGameState(new ds::UserNameState(ids));
	connectGameStates("TileMapEditor", 1, "MainMenu");
	connectGameStates("UserNameState", 6, "MainMenu");
	connectGameStates("MainMenu", 1, "LevelSelectorState");
	connectGameStates("MainMenu", 2, "TileMapEditor");
	connectGameStates("MainMenu", 5, "HighscoreState");
	connectGameStates("MainMenu", 4, "CreditsState");
	connectGameStates("CreditsState", 1, "MainMenu");
	connectGameStates("HighscoreState", 1, "MainMenu");
	connectGameStates("MainState", 22, "MainMenu");
	connectGameStates("MainState", 21, "MainState");
	connectGameStates("LevelSelectorState", 2, "MainMenu");
	connectGameStates("LevelSelectorState", 1, "MainState");

	RID _material = ds::res::find("SpriteMaterial", ds::ResourceType::MATERIAL);
	ds::Material* m = ds::res::getMaterial(_material);
	m->texture = ds::res::find("TextureArray", ds::ResourceType::TEXTURE);
	
	//ds::audio::play(SID("255"));
	return true;
}

// -------------------------------------------------------
// init
// -------------------------------------------------------
void ColorZone::init() {
	activate("UserNameState");
}

// -------------------------------------------------------
// onShutdown
// -------------------------------------------------------
void ColorZone::onShutdown() {
	_context.highscoreService->save();
}

// -------------------------------------------------------
// update
// -------------------------------------------------------
void ColorZone::update(float dt) {
	if (ds::events::num() > 0) {
		if (ds::events::containsType(200)) {
			ds::plugins::deactivate(SID("InputDialog"));
		}
	}
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void ColorZone::render() {}
