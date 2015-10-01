#pragma once
#include "..\TileMap.h"
#include "..\Constants.h"
#include <base\GameState.h>
#include "..\Highscores.h"

class HighscoreState : public ds::GameState {

public:
	HighscoreState(ds::DialogManager* gui, GameContext* context);
	~HighscoreState();
	int update(float dt);
	void render();
	int onGUIButton(ds::DialogID dlgID, int button);
	void activate();
	void deactivate();
private:
	void updateText();
	GameContext* _context;
	ds::DialogManager* _gui;
	Highscores _highscores;
	int _availableLevels[MAX_LEVELS];
	int _numLevels;
	int _index;
	GameMode _mode;
};

