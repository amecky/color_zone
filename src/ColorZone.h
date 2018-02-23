#pragma once
#include "BaseApp.h"
#include "TileMap.h"
#include "scenes\MainGameScene.h"
#include "scenes\MapSelectionScene.h"
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
	GameSettings* _gameSettings;
	GameContext _ctx;
};

