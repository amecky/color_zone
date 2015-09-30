#include "HighscoreState.h"
#include <utils\Log.h>
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>
#include <base\GameStateMachine.h>

HighscoreState::HighscoreState(ds::DialogManager* gui, GameContext* context) : ds::GameState("HighscoreState"), _context(context), _gui(gui) {
}


HighscoreState::~HighscoreState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void HighscoreState::activate() {
	_highscores.load();
	_index = 0;
	_numLevels = _context->filesystem.getAvailableLevels(_availableLevels);
	_gui->activate("Highscores");	
	// add current score to highscores
	int ranking = _highscores.add(_context->currentScore);
	if (ranking != -1) {
		_highscores.save();
	}
	_mode = GM_TIMER;
	updateText();
}

// --------------------------------------------
// activate
// --------------------------------------------
void HighscoreState::deactivate() {
	_gui->deactivate("Highscores");
}

// --------------------------------------------
// update
// --------------------------------------------
int HighscoreState::update(float dt) {
	// nothing to do
	return 0;
}

void HighscoreState::updateText() {
	ds::GUIDialog* dlg = _gui->get("Highscores");
	std::string str;
	ds::string::formatInt(_availableLevels[_index], str);
	dlg->updateText(110, "Level " + str);
	if (_mode == GM_TIMER) {
		dlg->updateText(111, "Timer mode");
	}
	else {
		dlg->updateText(111, "Coverage mode");
	}
	Highscore scores[MAX_SCORE_ENTRIES];
	_highscores.get(_availableLevels[_index], _mode, scores);
	std::string nr;
	for (int i = 0; i < 5; ++i) {
		const Highscore& current = scores[i];
		if (current.score > 0) {
			dlg->updateText(20 + i, current.name);
			ds::string::formatInt(current.score, nr, 6);
			dlg->updateText(30 + i, nr);
		}
		else {
			dlg->updateText(20 + i, "---");
			ds::string::formatInt(0, nr, 6);
			dlg->updateText(30 + i, nr);
		}
	}
}
// --------------------------------------------
// click
// --------------------------------------------
int HighscoreState::onGUIButton(ds::DialogID dlgID, int button) {
	if (button == 2) {
		if (_mode == GM_TIMER) {
			_mode = GM_COVERAGE;
		}
		else {
			--_index;
			if (_index < 0) {
				_index = 0;
			}
			_mode = GM_TIMER;
		}
		updateText();
		return 0;

	}
	else  if (button == 3) {
		if (_mode == GM_TIMER) {
			_mode = GM_COVERAGE;
		}
		else {
			++_index;
			if (_index >= _numLevels) {
				_index = _numLevels - 1;				
			}
			_mode = GM_TIMER;
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
}


