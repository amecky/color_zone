#include "GameOverState.h"
#include <utils\Log.h>
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>
#include <base\GameStateMachine.h>

GameOverState::GameOverState(ds::DialogManager* gui, GameContext* context) : ds::GameState("GameOverState"), _context(context), _gui(gui) {
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
	dlg->updateText(10, str);
	if (_context->gameMode == GM_TIMER) {
		dlg->updateText(11, "Timer mode");
	}
	else {
		dlg->updateText(11, "Coverage mode");
	}
	ds::string::formatInt(_context->currentScore.fillrate,str);
	dlg->updateText(12, str + "%");
	ds::string::formatTime(_context->currentScore.minutes, _context->currentScore.seconds, str);
	dlg->updateText(13, str);
	ds::string::formatInt(_context->currentScore.score, str, 6);	
	dlg->updateText(14, str);
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
int GameOverState::onGUIButton(ds::DialogID dlgID, int button) {
	
	return button;
}

// --------------------------------------------
// render
// --------------------------------------------
void GameOverState::render() {
}


