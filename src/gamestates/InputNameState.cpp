#include "InputNameState.h"
#include <utils\Log.h>
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>
#include <base\GameStateMachine.h>

InputNameState::InputNameState(ds::DialogManager* gui, GameContext* context) : ds::GameState("InputNameState"), _context(context), _gui(gui) {
}


InputNameState::~InputNameState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void InputNameState::activate() {
	_dialog = _gui->get("Username");
	_gui->activate("Username");
	_context->name = "";
	_dialog->updateText(111, _context->name);
}

// --------------------------------------------
// activate
// --------------------------------------------
void InputNameState::deactivate() {
	_gui->deactivate("Username");
}

// --------------------------------------------
// update
// --------------------------------------------
int InputNameState::update(float dt) {
	// nothing to do
	return 0;
}

// --------------------------------------------
// click
// --------------------------------------------
int InputNameState::onGUIButton(ds::DialogID dlgID, int button) {
	return button;
}

// --------------------------------------------
// render
// --------------------------------------------
void InputNameState::render() {
}

int InputNameState::onChar(int ascii) {
	if (_context->name.length() < 10) {
		_context->name.push_back((char)ascii);
		_dialog->updateText(111, _context->name);
	}
	return 0;
}

int InputNameState::onKeyUp(WPARAM virtualKey) {
	if (virtualKey == VK_BACK)  {
		if (_context->name.length() > 0) {
			_context->name = _context->name.substr(0, _context->name.length() - 2);
			_dialog->updateText(111, _context->name);
		}
	}
	if (virtualKey == VK_RETURN) {
		return 1;
	}
	return 0;
}

