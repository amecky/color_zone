#include "IntroState.h"
#include <utils\Log.h>
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>
#include <base\GameStateMachine.h>

IntroState::IntroState(GameContext* context) : ds::GameState("IntroState"), _context(context) {
	_texture = ds::math::buildTexture(790, 410, 550, 90);
	_effect = new IntroEffect(_context);
}


IntroState::~IntroState() {
	delete _effect;
}

// --------------------------------------------
// activate
// --------------------------------------------
void IntroState::activate() {
	_effect->start(0, 0, ds::Rect(801, 401, 620, 80), 55, 18 , 0.0f);
}

int IntroState::onChar(int ascii) {
	if (ascii == 'r') {
		_effect->start(0, 0, ds::Rect(801, 401, 620, 80), 55, 18, 0.0f);
	}
	return 0;
}
// --------------------------------------------
// update
// --------------------------------------------
int IntroState::update(float dt) {
	_effect->update(dt);
	//if (!_effect->isRunning()) {
		//return 1;
	//}
	return 0;
}

// --------------------------------------------
// render
// --------------------------------------------
void IntroState::render() {
	//ds::sprites::draw(v2(512, 384), _texture);
	_effect->render();
	//_context->settings->showDialog();
}


