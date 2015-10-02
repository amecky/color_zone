#include "InputNameState.h"
#include <utils\Log.h>
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>
#include <base\GameStateMachine.h>

InputNameState::InputNameState(ds::DialogManager* gui, GameContext* context) : ds::GameState("InputNameState"), _context(context), _gui(gui) {
	_texture = ds::math::buildTexture(0, 200, 20, 4);
}


InputNameState::~InputNameState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void InputNameState::activate() {
	_dialog = _gui->get("Username");
	_gui->activate("Username");
	_dialog->updateText(111, _context->name);
	_visible = true;
	_timer = 0.0f;
	v2 l = _dialog->getTextSize(111);
	_length = l.x;
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
	_timer += dt;
	if (_timer > 0.5f) {
		_timer -= 0.5f;
		_visible = !_visible;
	}
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
	if (_visible) {
		ds::sprites::draw(v2(370 + _length + 20, 445), _texture);
	}
}

int InputNameState::onChar(int ascii) {
	if (_context->name.length() < 10) {
		_context->name.push_back((char)ascii);
		_dialog->updateText(111, _context->name);
		v2 l = _dialog->getTextSize(111);
		_length = l.x;
	}
	return 0;
}

int InputNameState::onKeyUp(WPARAM virtualKey) {
	if (virtualKey == VK_BACK)  {
		if (_context->name.length() > 0) {
			_context->name = _context->name.substr(0, _context->name.length() - 2);
			_dialog->updateText(111, _context->name);
			v2 l = _dialog->getTextSize(111);
			_length = l.x;
		}
	}
	if (virtualKey == VK_RETURN) {
		return 1;
	}
	return 0;
}

