#include "GameOverState.h"
#include <utils\Log.h>
#include <renderer\graphics.h>
#include <gamestates\GameStateMachine.h>

GameOverState::GameOverState(GameContext* context) : ds::GameState("GameOverState"), _context(context) {
}


GameOverState::~GameOverState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void GameOverState::activate() {
	_gui->activate("GameOver");	
	ds::GUIDialog* dlg = _gui->get("GameOver");
	std::string str;
	ds::string::formatInt(_context->levelIndex, str);
	dlg->updateText(10, str.c_str());
	if (_context->gameMode == GM_TIMER) {
		dlg->updateText(11, "Timer mode");
	}
	else {
		dlg->updateText(11, "Coverage mode");
	}
	ds::string::formatInt(_context->currentScore.fillrate,str);
	str += "%";
	dlg->updateText(12, str.c_str());
	ds::string::formatTime(_context->currentScore.minutes, _context->currentScore.seconds, str);
	dlg->updateText(13, str.c_str());
	ds::string::formatInt(_context->currentScore.score, str, 6);	
	dlg->updateText(14, str.c_str());
}

// --------------------------------------------
// activate
// --------------------------------------------
void GameOverState::deactivate() {
	_gui->deactivate("GameOver");
}

// --------------------------------------------
// update
// --------------------------------------------
int GameOverState::update(float dt) {
	// nothing to do
	return 0;
}

// --------------------------------------------
// click
// --------------------------------------------
int GameOverState::onGUIButton(int button) {
	
	return button;
}

// --------------------------------------------
// render
// --------------------------------------------
void GameOverState::render() {
}


