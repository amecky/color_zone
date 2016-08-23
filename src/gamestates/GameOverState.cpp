#include "GameOverState.h"
#include <utils\Log.h>
#include <renderer\graphics.h>
#include <gamestates\GameStateMachine.h>

GameOverState::GameOverState(GameContext* context, ds::Game* game) : ds::BasicMenuGameState("GameOver", "GameOver", game), _context(context) {
}


GameOverState::~GameOverState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void GameOverState::activate() {
	std::string str;
	ds::string::formatInt(_context->levelIndex, str);
	_dialog->updateText(10, str.c_str());
	ds::string::formatInt(_context->currentScore.fillrate,str);
	str += "%";
	_dialog->updateText(11, str.c_str());
	ds::string::formatTime(_context->currentScore.minutes, _context->currentScore.seconds, str);
	_dialog->updateText(12, str.c_str());
	ds::string::formatInt(_context->currentScore.score, str, 6);	
	_dialog->updateText(13, str.c_str());
}

// --------------------------------------------
// activate
// --------------------------------------------
void GameOverState::deactivate() {
}



