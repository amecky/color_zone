#pragma once
#include "..\Common.h"
#include "..\Constants.h"
#include <gamestates\GameState.h>

namespace ds {

	struct UserNameDialogSettings {

		const char* dialogName;
		int textID;
		v2 textPosition;
		int cursorID;
		int eventID;
	};

	class UserNameState : public ds::BasicMenuGameState {

	public:
		UserNameState(const UserNameDialogSettings& settings);
		~UserNameState();
		int update(float dt);
		void render();
		int onGUIButton(int button);
		void activate();
		void deactivate();
		virtual int onChar(int ascii);
	private:
		UserNameDialogSettings _settings;
		float _timer;
		bool _visible;
		char _text[12];
		int _textIndex;
	};

}