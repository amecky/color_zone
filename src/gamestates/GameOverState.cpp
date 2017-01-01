#include "GameOverState.h"
#include <renderer\graphics.h>
#include <gamestates\GameStateMachine.h>

GameOverState::GameOverState(GameContext* context) : ds::BasicMenuGameState("GameOverState","GameOver"), _context(context) {
	_texture = math::buildTexture(0, 200, 20, 4);
}


GameOverState::~GameOverState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void GameOverState::activate() {
	_dialog->activate();
	//_dialog->updateText(111, _context->name.c_str());
	_textIndex = 0;
	_text[0] = '\0';
	_timer = 0.0f;
	//v2 l = _dialog->getTextSize(111);
	//_length = l.x;
}

// --------------------------------------------
// activate
// --------------------------------------------
void GameOverState::deactivate() {
	_dialog->deactivate();
}

// --------------------------------------------
// update
// --------------------------------------------
int GameOverState::update(float dt) {
	// nothing to do
	_timer += dt;
	if (_timer > 0.5f) {
		_timer -= 0.5f;
	}
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
	_dialog->render();
}

int GameOverState::onChar(int ascii) {
	if (_context->name.length() < 10) {
		_context->name.push_back((char)ascii);
		//_dialog->updateText(111, _context->name.c_str());
		//v2 l = _dialog->getTextSize(111);
		//_length = l.x;
	}
	return 0;
}

int GameOverState::onKeyUp(WPARAM virtualKey) {
	if (virtualKey == VK_BACK)  {
		if (_context->name.length() > 0) {
			_context->name = _context->name.substr(0, _context->name.length() - 2);
			//_dialog->updateText(111, _context->name.c_str());
			//v2 l = _dialog->getTextSize(111);
			//_length = l.x;
		}
	}
	if (virtualKey == VK_RETURN) {
		return 1;
	}
	return 0;
}

