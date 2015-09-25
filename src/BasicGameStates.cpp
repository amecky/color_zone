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
	if (dlgID == 1 && button == 0) {
		stateMachine->activate("MainGame");
	}
	if (dlgID == 1 && button == 2) {
		stateMachine->activate("TileMapEditor");
	}
}

GamePauseState::GamePauseState(ds::DialogManager* gui) : ds::GameState("GamePause"), _gui(gui) {
}

void GamePauseState::activate() {
	_gui->activate("Pause");
}

void GamePauseState::deactivate() {
	_gui->deactivate("Pause");
}

void GamePauseState::onGUIButton(ds::DialogID dlgID, int button) {
	if (dlgID == 4 && button == 1) {
		stateMachine->activate("MainGame");
	}	
}