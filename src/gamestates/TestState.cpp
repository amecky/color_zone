#include "TestState.h"
#include <core\log\Log.h>
#include <renderer\graphics.h>
#include <base\InputSystem.h>
#include <audio\AudioManager.h>

TestState::TestState(GameContext* context, ds::Game* game) : ds::GameState("TestState", game), _context(context) {
	_map = new TileMap(_context);
	_laser = new Laser;
	laser::init(_laser);
	_map->reset();
	block::init(&_previewBlock);
	_previewBlock.position = p2i(640, 660);
	//_previewBlock = new Block(_context, false);
	//_previewBlock->setPosition(p2i(760, 660));
	block::init(&_mainBlock);
	_effect = new SparkleEffect(_context);
	//_hud = new HUD(_context->settings);
	_hud = ds::res::getGUIDialog("HUD");
	_gameOver = ds::res::getGUIDialog("GameOver");
	_mode = GM_OVER;
}

TestState::~TestState() {
	//delete _hud;
	//delete _mainBlock;
	//delete _previewBlock;
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
	laser::start(_laser,_context->settings->laser.startDelay);
	_hud->activate();
	_context->score = 0;
	_context->fillRate = 0;
	_effect->reset();	
	block::pick_colors(&_previewBlock);
	block::copy_colors(&_mainBlock, &_previewBlock);
	block::pick_colors(&_previewBlock);
	_previewBlock.flashing = true;
	_previewBlock.flashTimer = 0.0f;
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
	//std::string str;
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
	if (laser::move(_laser, _context->settings->laser.stepDelay, _context->settings->laser.startDelay, dt, &column)) {
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
	if (laser::tick(_laser, dt)) {
		_hud->setNumber(HUD_LASER, 0);
	}
	if (_laser->state == LS_IDLE || _laser->state == LS_WARMING_UP) {
		_hud->setNumber(HUD_LASER, _laser->timer + 1);
	}
}
// --------------------------------------------
// update
// --------------------------------------------
int TestState::update(float dt) {
	if (_mode == GM_RUNNING) {
		block::follow_mouse(&_mainBlock);

		block::update(&_mainBlock, dt);
		if (_previewBlock.flashing) {
			block::flash_scale(&_previewBlock, dt, 0.2f);
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
				block::copy_colors(&_mainBlock, &_previewBlock);
				block::pick_colors(&_previewBlock);
				_previewBlock.flashing = true;
				_previewBlock.flashTimer = 0.0f;
			}
		}
		else if (button == 1) {
			block::start_rotating(&_mainBlock);
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
		block::render(&_previewBlock, _context->colors);
		block::render_boxed(&_mainBlock, _context->colors);
		laser::render(_laser);
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
		block::copy_colors(&_mainBlock,&_previewBlock);
		block::pick_colors(&_previewBlock);
		_previewBlock.flashing = true;
		_previewBlock.flashTimer = 0.0f;
	}
	if (ascii == '2') {
		ds::audio::play(SID("255"));
	}
	if (ascii == 's') {
		laser::start(_laser, _context->settings->laser.startDelay);
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
