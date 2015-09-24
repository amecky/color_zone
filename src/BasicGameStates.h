#pragma once
#include <base\GameStateMachine.h>
#include <dialogs\DialogManager.h>

class StartMenuState : public ds::GameState {

public:
	StartMenuState(ds::DialogManager* gui);
	~StartMenuState() {}
	void activate();
	void deactivate();
	void onGUIButton(ds::DialogID dlgID, int button);
private:
	ds::DialogManager* _gui;
};

