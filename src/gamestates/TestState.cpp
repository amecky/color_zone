#include "TestState.h"
#include <utils\Log.h>
#include <renderer\graphics.h>
#include <base\InputSystem.h>

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
	_context->pick_colors();
	_map->reset();
	_map->build(0);
	laser::start(_laser,_context->settings->laserStartDelay);
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
	_hud->setNumber(HUD_LASER, _context->settings->laserStartDelay);
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
}

// --------------------------------------------
// move laser
// --------------------------------------------
void TestState::moveLaser(float dt) {
	int column = -1;
	if (laser::move(_laser, _context->settings->laserStepDelay, _context->settings->laserStartDelay, dt, &column)) {
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

	_effect->update(dt);

	return 0;
}

// --------------------------------------------
// click
// --------------------------------------------
int TestState::onButtonUp(int button, int x, int y) {
	if (button == 0) {
		if (_map->copyBlock(&_mainBlock)) {
			block::copy_colors(&_mainBlock,&_previewBlock);
			block::pick_colors(&_previewBlock);
			_previewBlock.flashing = true;
			_previewBlock.flashTimer = 0.0f;
		}
	}
	else if (button == 1) {
		block::start_rotating(&_mainBlock);
	}
	return 0;
}

// --------------------------------------------
// render
// --------------------------------------------
void TestState::render() {
	
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	sprites->begin();
	_map->render();
	_effect->render();
	block::render(&_previewBlock, _context->colors);
	block::render_boxed(&_mainBlock, _context->colors);
	laser::render(_laser);
	

	for (int i = 0; i < 8; ++i) {
		sprites->draw(v2(480 + i * 40, 30), math::buildTexture(0, 36, 36, 36), 0.0f, v2(1, 1), _context->colors[i]);
	}

	sprites->end();
	_hud->render();
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
	if (ascii == 's') {
		laser::start(_laser, _context->settings->laserStartDelay);
	}
	if (ascii == 'd') {
		_context->resume = true;
		return 1;
	}
	if (ascii == 'x') {
		fillHighscore();
		return 1;
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
