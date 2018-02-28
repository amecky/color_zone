#pragma once
#include "BaseApp.h"
#include "TileMap.h"
#include "scenes\MainGameScene.h"
#include "scenes\MapSelectionScene.h"
#include "scenes\GameOverScene.h"
#include "scenes\HighscoreScene.h"
#include "scenes\MainMenuScene.h"
#include "scenes\BackgroundScene.h"
#include "Common.h"
#include "GameSettings.h"

class ColorZone : public BaseApp {

public:
	ColorZone();
	~ColorZone();
	void initialize();
	void handleEvents(ds::EventStream* events);
private:
	TileMap* _tiles;
	MapSelectionScene* _mapSelectionScene;
	MainGameScene* _mainGameScene;
	MainMenuScene* _mainMenuScene;
	HighscoreScene* _highscoreScene;
	GameOverScene* _gameOverScene;
	BackgroundScene* _backgroundScene;
	GameSettings* _gameSettings;
	GameContext _ctx;
};

