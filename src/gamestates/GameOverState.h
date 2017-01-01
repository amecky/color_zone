#pragma once
#include "..\Common.h"
#include "..\Constants.h"
#include <gamestates\GameState.h>

class GameOverState : public ds::BasicMenuGameState {

public:
	GameOverState(GameContext* context);
	~GameOverState();
	int update(float dt);
	void render();
	int onGUIButton(int button);
	void activate();
	void deactivate();
	virtual int onChar(int ascii);
	int onKeyUp(WPARAM virtualKey);
private:
	GameContext* _context;
	ds::Texture _texture;
	float _timer;
	int _length;
	char _text[12];
	int _textIndex;
};

