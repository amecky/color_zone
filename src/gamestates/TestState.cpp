#include "TestState.h"
#include <core\log\Log.h>
#include <renderer\graphics.h>
#include <base\InputSystem.h>
#include <audio\AudioManager.h>

TestState::TestState(GameContext* context) : ds::GameState("TestState"), _context(context) {
	_map = new TileMap(_context);
	_laser = new Laser;
	_map->reset();
	_previewBlock.setPosition(p2i(640, 660));
	_effect = new SparkleEffect(_context);
	_hud = ds::res::getGUIDialog("HUD");
	_gameOver = ds::res::getGUIDialog("GameOver");
	_mode = GM_OVER;
}

TestState::~TestState() {
	delete _effect;
	delete _laser;
	delete _map;
}

void TestState::init() {
}

void TestState::activate() {
	_mode = GM_RUNNING;
	_gameOver->deactivate();
	_context->pick_colors();
	_map->reset();
	_map->build(0);
	_laser->start(_context->settings->laser.startDelay);
	_hud->activate();
	_context->score = 0;
	_context->fillRate = 0;
	_effect->reset();	
	_previewBlock.pick_colors();
	_mainBlock.copy_colors(&_previewBlock);
	_previewBlock.pick_colors();
	_previewBlock.startFlashing();
	_hud->setNumber(HUD_SCORE, _context->score);
	char buffer[32];
	sprintf_s(buffer, 32, "%d%%", _context->fillRate);
	_hud->updateText(HUD_PERCENTAGE, buffer);
	_hud->setNumber(HUD_LASER, _context->settings->laser.startDelay);
	// FIXME: set HUD colors (5 -> text 6 -> timer etc)
}

// --------------------------------------------
// fill current score
// --------------------------------------------
void TestState::fillHighscore() {	
	_context->currentScore.fillrate = _context->fillRate;
	_context->currentScore.level = 1;
	_context->currentScore.score = _context->score;
	ds::GameTimer* timer = _hud->getTimer(HUD_TIMER);
	_context->currentScore.minutes = timer->getMinutes();
	_context->currentScore.seconds = timer->getSeconds();
	_gameOver->updateTextFormatted(10, "%d",_context->levelIndex);
	_gameOver->updateTextFormatted(11, "%d %%", _context->currentScore.fillrate);
	_gameOver->updateTextFormatted(12, "%02d:%02d", _context->currentScore.minutes, _context->currentScore.seconds);
	_gameOver->updateTextFormatted(13, "%06d", _context->currentScore.score);
}

// --------------------------------------------
// move laser
// --------------------------------------------
void TestState::moveLaser(float dt) {
	int column = -1;
	if (_laser->move(_context->settings->laser.stepDelay, _context->settings->laser.startDelay, dt, &column)) {
		int colors[MAX_Y];
		_map->getColumn(column, colors);
		for (int i = 0; i < MAX_Y; ++i) {
			if (colors[i] != -1) {
				_effect->start(p2i(column, i), colors[i]);
			}
		}
		int cleared = _map->clearColumn(column);
		_context->score += cleared * 100;
		if (cleared > 0) {
			_context->fillRate = _map->getFillRate();
			LOG << "fillRate: " << _context->fillRate;
			_hud->setNumber(HUD_SCORE,_context->score);
			_hud->updateTextFormatted(HUD_PERCENTAGE, "%d%%", _context->fillRate);
		}
	}
	if (_laser->tick(dt)) {
		_hud->setNumber(HUD_LASER, 0);
	}
	if (!_laser->isRunning()) {
		_hud->setNumber(HUD_LASER, _laser->getIdleSeconds() + 1);
	}
}
// --------------------------------------------
// update
// --------------------------------------------
int TestState::update(float dt) {
	if (_mode == GM_RUNNING) {
		_mainBlock.follow_mouse();

		_mainBlock.update(dt);
		if (_previewBlock.isFlashing()) {
			_previewBlock.flash_scale(dt, 0.2f);
		}

		_hud->tick(dt);

		const ds::GameTimer* timer = _hud->getTimer(HUD_TIMER);
		if (timer->getMinutes() >= 3) {
			// FIXME: find a better way to end the game???
			fillHighscore();
			return 1;
		}

		moveLaser(dt);
	}
	_effect->update(dt);

	return 0;
}

// --------------------------------------------
// click
// --------------------------------------------
int TestState::onButtonUp(int button, int x, int y) {
	if (_mode == GM_RUNNING) {
		if (button == 0) {
			if (_map->copyBlock(&_mainBlock)) {
				_mainBlock.copy_colors(&_previewBlock);
				_previewBlock.pick_colors();
				_previewBlock.startFlashing();
			}
		}
		else if (button == 1) {
			_mainBlock.start_rotating();
		}
	}
	else {
		int ret = _gameOver->onButton(button, x, y, true);
		if (ret != -1) {
			return ret;
		}

	}
	return 0;
}

// --------------------------------------------
// render
// --------------------------------------------
void TestState::render() {
	
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	_map->render();
	if (_mode == GM_RUNNING) {
		_effect->render();
		_previewBlock.render(_context->colors);
		_mainBlock.render_boxed(_context->colors);
		_laser->render();
		// FIXME: only for visual debugging
		for (int i = 0; i < 8; ++i) {
			sprites->draw(v2(480 + i * 40, 30), math::buildTexture(0, 36, 36, 36), 0.0f, v2(1, 1), _context->colors[i]);
		}
	}
	_hud->render();
	_gameOver->render();
}

// --------------------------------------------
// on char
// --------------------------------------------
int TestState::onChar(int ascii) {
	if (ascii == '1') {
		_mainBlock.copy_colors(&_previewBlock);
		_previewBlock.pick_colors();
		_previewBlock.startFlashing();
	}
	if (ascii == '2') {
		ds::audio::play(SID("255"));
	}
	if (ascii == 's') {
		_laser->start(_context->settings->laser.startDelay);
	}
	if (ascii == 'd') {
		_context->resume = true;
		return 1;
	}
	if (ascii == 'x') {
		fillHighscore();
		//return 1;
		_gameOver->activate();
		_mode = GM_OVER;
	}
	if (ascii == 'r') {
		_context->settings->load();
	}
	if (ascii == 'k') {
		_context->pick_colors();
	}
	if (ascii == 'p') {
		_effect->start(p2i(8, 8), 1);
	}
	return 0;
}
