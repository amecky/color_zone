#include "UserNameState.h"
#include <renderer\graphics.h>
#include <gamestates\GameStateMachine.h>

namespace ds {

	UserNameState::UserNameState(const UserNameDialogSettings& settings) : ds::BasicMenuGameState("UserNameState", settings.dialogName), _settings(settings) {
		_timer = 0.0f;
		_text[0] = '\0';
		_textIndex = 0;
	}


	UserNameState::~UserNameState() {
	}

	// --------------------------------------------
	// activate
	// --------------------------------------------
	void UserNameState::activate() {
		_dialog->activate();
		_timer = 0.0f;
		_dialog->setColor(_settings.cursorID, ds::Color(255, 255, 255, 255));
		_text[0] = '\0';
		_textIndex = 0;
		_dialog->updateText(_settings.textID, _text);
	}

	// --------------------------------------------
	// activate
	// --------------------------------------------
	void UserNameState::deactivate() {
		_dialog->deactivate();
	}

	int UserNameState::onGUIButton(int button) {
		if (_textIndex > 0) {
			return button;
		}
		return 0;
	}
	// --------------------------------------------
	// update
	// --------------------------------------------
	int UserNameState::update(float dt) {
		_timer += dt;
		if (_timer > 0.5f) {
			_timer = 0.0f;
			_visible = !_visible;
		}
		if (_visible) {
			_dialog->setColor(_settings.cursorID, ds::Color(255, 255, 255, 255));
		}
		else {
			_dialog->setColor(_settings.cursorID, ds::Color(255, 255, 255, 0));
		}
		return 0;
	}

	// --------------------------------------------
	// render
	// --------------------------------------------
	void UserNameState::render() {
		//_dialog->render();
		graphics::turnOffZBuffer();
		graphics::selectViewport(0);
		ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
		sprites->begin();
		_dialog->render();
		sprites->end();
		graphics::turnOnZBuffer();
	}

	// --------------------------------------------
	// onChar
	// --------------------------------------------
	int UserNameState::onChar(int ascii) {
		bool updated = false;
		if (ascii >= 32 && ascii < 128) {
			if (_textIndex < 11) {
				_text[_textIndex] = ascii;
				_text[_textIndex + 1] = '\0';
				++_textIndex;
				updated = true;
			}
		}
		else if (ascii == 8) {
			// Backspace
			if (_textIndex > 0) {
				--_textIndex;
				_text[_textIndex] = '\0';
				updated = true;
			}
		}
		else if (ascii == 13) {
			// Return
			if (_textIndex > 0) {
				events::send(_settings.eventID);
			}
		}
		if (updated) {
			_dialog->updateText(_settings.textID, _text);
			v2 size = _dialog->getTextSize(_settings.textID);
			v2 p = _settings.textPosition;
			p.x += size.x * 0.5f;
			_dialog->setPosition(_settings.textID, p);
			p.x = _settings.textPosition.x + 10.0f + size.x;
			p.y -= 10.0f;
			_dialog->setPosition(_settings.cursorID, p);
		}
		return 0;
	}

}