#pragma once
#include "..\Common.h"
#include "..\Constants.h"
#include <gamestates\GameState.h>

class InputNameState : public ds::GameState {

public:
	InputNameState(GameContext* context);
	~InputNameState();
	int update(float dt);
	void render();
	int onGUIButton(int button);
	void activate();
	void deactivate();
	virtual int onChar(int ascii);
	int onKeyUp(WPARAM virtualKey);
private:
	GameContext* _context;
	ds::GUIDialog* _dialog;
	ds::Texture _texture;
	float _timer;
	bool _visible;
	int _length;
};

