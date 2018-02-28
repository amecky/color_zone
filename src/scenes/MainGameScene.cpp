#include "MainGameScene.h"
#include "..\Common.h"
#include <ds_game_ui.h>
#include <ds_imgui.h>
#include "..\TileMap.h"
#include "..\EventStream.h"
#include "..\objects\Block.h"
#include "..\GameSettings.h"
#include "..\utils\SparkleEffect.h"
#include "..\objects\Laser.h"
// --------------------------------------------
// ctor
// --------------------------------------------
MainGameScene::MainGameScene(TileMap* tiles, GameContext* ctx) : Scene(), _tiles(tiles), _ctx(ctx) {
	_currentBlock = new Block;
	initialise_block(_currentBlock);
	_nextBlock = new Block;
	initialise_block(_nextBlock);
	_currentBlock->position = p2i(640, 560);
	pick_block_colors(_currentBlock);
	_nextBlock->position = p2i(640 - SQUARE_SIZE, 660);
	for (int i = 0; i < 6; ++i) {
		pick_block_colors(_nextBlock);
	}
	copy_block_colors(_currentBlock, _nextBlock);
	pick_block_colors(_nextBlock);
	_buttonDown[0] = 0;
	_buttonDown[1] = 0;

	_sparkleEffect = new SparkleEffect(_ctx);
	_sparkleEffect->reset();
	_dbgSparkCol = 2;
	_laser = new Laser(_ctx);
}

// --------------------------------------------
// dtor
// --------------------------------------------
MainGameScene::~MainGameScene() {
	delete _laser;
	delete _sparkleEffect;
	delete _currentBlock;
	delete _nextBlock;
}

// --------------------------------------------
// on activation
// --------------------------------------------
void MainGameScene::onActivation() {
	_buttonDown[0] = 0;
	_buttonDown[1] = 0;
	start_timer(&_timer, 0, 0, TimerMode::TM_INC);
	_sparkleEffect->reset();
	_laser->reset();
	_laser->start();
}

// --------------------------------------------
// handle buttons
// --------------------------------------------
ButtonDefinition::Enum MainGameScene::handleButtons() {
	ButtonDefinition::Enum buttonClicked = ButtonDefinition::NONE;
	if (ds::isMouseButtonPressed(0)) {
		_buttonDown[0] = 1;
	}
	else {
		if (_buttonDown[0] == 1) {
			_buttonDown[0] = 0;
			buttonClicked = ButtonDefinition::LEFT;
		}
	}
	if (ds::isMouseButtonPressed(1)) {
		_buttonDown[1] = 1;
	}
	else {
		if (_buttonDown[1] == 1) {
			_buttonDown[1] = 0;
			buttonClicked = ButtonDefinition::RIGHT;
		}
	}
	return buttonClicked;
}

// --------------------------------------------
// update
// --------------------------------------------
void MainGameScene::update(float dt) {

	ButtonDefinition::Enum buttonClicked = handleButtons();

	if (buttonClicked == ButtonDefinition::RIGHT) {
		if (!_currentBlock->rotating) {
			_currentBlock->rotating = true;
			_currentBlock->rotationTimer = 0.0f;
		}
	}
	if (buttonClicked == ButtonDefinition::LEFT) {
		if (_tiles->copyBlock(_currentBlock)) {
			copy_block_colors(_currentBlock, _nextBlock);
			pick_block_colors(_nextBlock);
			_nextBlock->flashing = true;
			_nextBlock->flashTimer = 0.0f;
			//calculate_fill_rate(ctx.tiles);
		}
	}

	follow_mouse(_currentBlock);
	rotate_block(_currentBlock, dt);
	flash_block_scale(_nextBlock, dt, 0.2f);

	tick_timer(&_timer, dt);

	_sparkleEffect->update(dt);

	_laser->tick(dt);
	_laser->flash(dt);
	if (_laser->move(dt)) {
		int colors[MAX_Y];
		_tiles->getColor(_laser->getColumn() - 1, colors);
		for (int i = 0; i < MAX_Y; ++i) {
			if (colors[i] != -1) {
				_sparkleEffect->start(p2i(_laser->getColumn() - 1, i), colors[i]);
			}
		}
		_tiles->clearColumn(_laser->getColumn() - 1);
	}
}

// --------------------------------------------
// render
// --------------------------------------------
void MainGameScene::render(SpriteBatchBuffer* buffer) {
	_tiles->render(_ctx->colors);
	render_block_boxed(_currentBlock, buffer, _ctx->colors);
	render_block(_nextBlock, buffer, _ctx->colors);
	_laser->render(buffer);
	_sparkleEffect->render(buffer);
	showHUD();
}

// --------------------------------------------
// show HUD
// --------------------------------------------
void MainGameScene::showHUD() {
	dialog::begin();
	dialog::FormattedText(_ctx->settings->hud.score_position, false, ds::vec2(1.0f), ds::Color(255, 255, 255, 255), "%06d", _ctx->score);
	dialog::FormattedText(_ctx->settings->hud.coverage_position, false, ds::vec2(1.0f), ds::Color(255, 255, 255, 255), "%3d%%", _ctx->fillRate);
	dialog::FormattedText(_ctx->settings->hud.timer_position, false, ds::vec2(1.0f), ds::Color(255, 255, 255, 255), "%02d:%02d", _timer.minutes, _timer.seconds);
	if (_laser->getIdleSeconds() > 0) {
		dialog::FormattedText(_ctx->settings->hud.laser_idle_position, false, ds::vec2(1.0f), ds::Color(255, 255, 255, 255), "%2d", _laser->getIdleSeconds());
	}
	dialog::end();
}

// --------------------------------------------
// showGUI
// --------------------------------------------
void MainGameScene::showGUI() {
	p2i dp(10, 710);
	int state = 1;
	gui::start(&dp, 300);
	/*
	int nc = twk_num_categories();
	for (int i = 0; i < nc; ++i) {
		show_tweakable_gui(twk_get_category_name(i), &settingsStates[i]);
	}
	if (gui::begin("Laser", &state)) {
		//gui::Value("Idle", l.getIdleSeconds());
		if (gui::Button("Start")) {
			start_laser(&laser, 4.0f);// ctx.settings->laser.startDelay);
		}
	}
	*/
	gui::begin("Sparkles", 0);
	gui::Input("Spark Col", &_dbgSparkCol);
	if (gui::Button("Sparkles")) {
		int colors[MAX_Y];
		_tiles->getColor(_dbgSparkCol, colors);
		for (int i = 0; i < MAX_Y; ++i) {
			if (colors[i] != -1) {
				_sparkleEffect->start(p2i(_dbgSparkCol, i), colors[i]);
			}
		}
	}
	
	_tiles->showDebugGUI();

	gui::end();
}