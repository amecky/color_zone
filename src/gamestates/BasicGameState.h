#pragma once
#include <StateMachine.h>
#include "..\Common.h"
// ---------------------------------------------------------------
// GameState
// ---------------------------------------------------------------
class BasicGameState : public GameState {

public:
	BasicGameState(GameContext* ctx, const char* name) : GameState(name), _ctx(ctx) {}
	virtual ~BasicGameState() {}
protected:
	GameContext* _ctx;
};
