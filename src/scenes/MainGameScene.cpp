#include "MainGameScene.h"
#include "..\Common.h"
#include <ds_game_ui.h>
#include <ds_imgui.h>
#include "..\TileMap.h"
#include "..\EventStream.h"
#include "..\objects\Block.h"

MainGameScene::MainGameScene(TileMap* tiles, GameContext* ctx) : Scene(), _tiles(tiles), _ctx(ctx) {
	_currentBlock = new Block;
	initialise_block(_currentBlock);
	_nextBlock = new Block;
	initialise_block(_nextBlock);
	_currentBlock->position = p2i(640, 560);
	pick_block_colors(_currentBlock);
	_nextBlock->position = p2i(640, 660);
	for (int i = 0; i < 6; ++i) {
		pick_block_colors(_nextBlock);
	}
	copy_block_colors(_currentBlock, _nextBlock);
	pick_block_colors(_nextBlock);
	_buttonDown[0] = 0;
	_buttonDown[1] = 0;
}

MainGameScene::~MainGameScene() {
	delete _currentBlock;
	delete _nextBlock;
}

void MainGameScene::onActivation() {
	_buttonDown[0] = 0;
	_buttonDown[1] = 0;
}

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
}

void MainGameScene::render(SpriteBatchBuffer* buffer) {
	_tiles->render(_ctx->colors);
	render_block_boxed(_currentBlock, buffer, _ctx->colors);
	render_block(_nextBlock, buffer, _ctx->colors);
}