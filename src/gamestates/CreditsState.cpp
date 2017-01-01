#include "CreditsState.h"
#include <core\log\Log.h>
#include <renderer\graphics.h>
#include <gamestates\GameStateMachine.h>

const char* TEXT[] = { "Coding","Graphics","Sounds","Font","Contact" };
const char* VALUES[] = { "Andreas Mecky","Andreas Mecky","freesounds.org","Nullshock","amecky@gmail.com" };

CreditsState::CreditsState(GameContext* context) : ds::BasicMenuGameState("CreditsState","credits"), _context(context) {
}


CreditsState::~CreditsState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void CreditsState::activate() {
	_dialog->activate();
	_index = 0;
	updateText();
	_timer = 0.0f;
}

void CreditsState::deactivate() {
	_dialog->deactivate();
}

// --------------------------------------------
// update
// --------------------------------------------
int CreditsState::update(float dt) {
	_timer += dt;
	if (_timer > 4.0f) {
		++_index;
		_timer = 0.0f;
		if (_index > 4) {
			_index = 0;
		}
		updateText();
	}
	_dialog->tick(dt);
	return 0;
}

// --------------------------------------------
// render
// --------------------------------------------
void CreditsState::render() {
	_dialog->render();
}

void CreditsState::updateText() {
	_dialog->updateText(3, TEXT[_index]);
	_dialog->updateText(4, VALUES[_index]);
	_dialog->fadeColor(3, ds::Color(0, 0, 0, 255), ds::Color(255, 255, 255, 255), 0.3f);
	_dialog->fadeColor(4, ds::Color(0, 0, 0, 255), ds::Color(255, 255, 255, 255), 0.3f);
}

