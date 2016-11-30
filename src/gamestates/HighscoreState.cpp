#include "HighscoreState.h"
#include <core\log\Log.h>
#include <renderer\graphics.h>
#include <gamestates\GameStateMachine.h>
#include "..\objects\Levels.h"

HighscoreState::HighscoreState(GameContext* context) : ds::BasicMenuGameState("HighscoreState","Highscores"), _context(context) {
	_index = 0;
}


HighscoreState::~HighscoreState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void HighscoreState::activate() {
	_highscores.load();
	_dialog->activate();
	_index = 0;
	// add current score to highscores
	int ranking = _highscores.add(_context->currentScore);
	if (ranking != -1) {
		_highscores.save();
	}
	updateText();
}

// --------------------------------------------
// activate
// --------------------------------------------
void HighscoreState::deactivate() {
	_dialog->deactivate();
}

// --------------------------------------------
// update
// --------------------------------------------
int HighscoreState::update(float dt) {
	// nothing to do
	return 0;
}

void HighscoreState::updateText() {
	char buffer[64];
	_dialog->updateText(10, _context->levels->getName(_index));
	Highscore scores[MAX_SCORE_ENTRIES];
	_highscores.get(_index, scores);
	for (int i = 0; i < 5; ++i) {
		const Highscore& current = scores[i];
		if (current.score > 0) {
			_dialog->updateText(20 + i, current.name);
			ds::string::formatInt(current.score, buffer, 6);
			_dialog->updateText(40 + i, buffer);
		}
		else {
			_dialog->updateText(20 + i, "---");
			ds::string::formatInt(0, buffer, 6);
			_dialog->updateText(40 + i, buffer);
		}
	}
}
// --------------------------------------------
// click
// --------------------------------------------
int HighscoreState::onGUIButton(int button) {
	if (button == 2) {
		--_index;
		if (_index < 0) {
			_index = 0;
		}
		updateText();
		return 0;

	}
	else  if (button == 3) {
		++_index;
		if (_index >= MAX_LEVELS) {
			_index = MAX_LEVELS - 1;				
		}
		updateText();
		return 0;
	}
	return button;
}

// --------------------------------------------
// render
// --------------------------------------------
void HighscoreState::render() {
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	sprites->begin();
	_dialog->render();
	sprites->end();
}


