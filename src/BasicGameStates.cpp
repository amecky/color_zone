#include "BasicGameStates.h"
#include <utils\Log.h>

StartMenuState::StartMenuState(ds::DialogManager* gui) : ds::GameState("StartMenu") , _gui(gui) {
}

void StartMenuState::activate() {
	_gui->activate("MainMenu");
}

void StartMenuState::deactivate() {
	_gui->deactivate("MainMenu");
}

void StartMenuState::onGUIButton(ds::DialogID dlgID, int button) {
	LOG << "dialog: " << dlgID << " button: " << button;
	if (dlgID == 1 && button == 0) {
		stateMachine->activate("MainGame");
	}
	if (dlgID == 1 && button == 2) {
		stateMachine->activate("TileMapEditor");
	}
}