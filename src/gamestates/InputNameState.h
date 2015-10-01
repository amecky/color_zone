#pragma once
#include "..\Common.h"
#include "..\Constants.h"
#include <base\GameState.h>

class InputNameState : public ds::GameState {

public:
	InputNameState(ds::DialogManager* gui, GameContext* context);
	~InputNameState();
	int update(float dt);
	void render();
	int onGUIButton(ds::DialogID dlgID, int button);
	void activate();
	void deactivate();
	virtual int onChar(int ascii);
	int onKeyUp(WPARAM virtualKey);
private:
	GameContext* _context;
	ds::DialogManager* _gui;
	ds::GUIDialog* _dialog;
};

