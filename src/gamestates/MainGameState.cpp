#include "MainGameState.h"
#include "..\Common.h"
#include "..\GameSettings.h"

MainGameState::MainGameState(GameContext* ctx) : BasicGameState(ctx, "MainState") {
	_current.setPosition(p2i(640, 560));
	_current.pick_colors();
	_next.setPosition(p2i(640, 660));
	_next.pick_colors();
	_next.pick_colors();
	_buttonDown[0] = 0;
	_buttonDown[1] = 0;
	_map = new TileMap(_ctx);
	//_map->build(_levels, 0);
	_laser = new Laser(_ctx);
	_laser->reset();
	_effect = new SparkleEffect(_ctx);
}

void MainGameState::activate() {
	_ctx->score = 0;
	_ctx->fillRate = 0;
	_map->reset();
	_map->build(_levels,_ctx->levelIndex);
	_laser->start();
	_next.pick_colors();
	_current.copy_colors(&_next);
	_next.pick_colors();
	_next.startFlashing();
	_active = true;
	_effect->reset();
}

void MainGameState::deactivate() {
	_active = false;
}

// --------------------------------------------
// move laser
// --------------------------------------------
void MainGameState::moveLaser(float dt) {
	int column = -1;
	if (_laser->move(dt, &column)) {
		int colors[MAX_Y];
		_map->getColumn(column, colors);
		for (int i = 0; i < MAX_Y; ++i) {
			if (colors[i] != -1) {
				_effect->start(p2i(column, i), colors[i]);
			}
		}
		int cleared = _map->clearColumn(column);
		_ctx->score += cleared * 100;
		if (cleared > 0) {
			_ctx->fillRate = _map->getFillRate();
			//LOG << "fillRate: " << _context->fillRate;
			//_hud->setNumber(HUD_SCORE, _context->score);
			//_hud->updateTextFormatted(HUD_PERCENTAGE, "%d%%", _context->fillRate);
		}
	}
	if (_laser->tick(dt)) {
		//_hud->setNumber(HUD_LASER, 0);
	}
	if (!_laser->isRunning()) {
		//_hud->setNumber(HUD_LASER, _laser->getIdleSeconds() + 1);
	}
}

int MainGameState::tick(float elapsed, EventStream* stream) {
	if (ds::isMouseButtonPressed(0)) {
		_buttonDown[0] = 1;
	}
	else {
		if (_buttonDown[0] == 1) {
			_buttonDown[0] = 0;
			if (_map->copyBlock(&_current)) {
				_current.copy_colors(&_next);
				_next.pick_colors();
				_next.startFlashing();
			}
		}
	}
	if (ds::isMouseButtonPressed(1)) {
		_buttonDown[1] = 1;
	}
	else {
		if (_buttonDown[1] == 1) {
			_buttonDown[1] = 0;
			_current.start_rotating();
		}
	}

	_current.follow_mouse();

	_current.update(elapsed);
	if (_next.isFlashing()) {
		_next.flash_scale(elapsed, 0.2f);
	}

	moveLaser(elapsed);

	_effect->update(elapsed);

	return 0;
}

void MainGameState::render() {
	_ctx->buffer->begin();
	_map->render();
	_current.render_boxed(_ctx->buffer,_ctx->colors);
	_next.render(_ctx->buffer, _ctx->colors);
	_laser->render();
	_effect->render();
	_ctx->buffer->flush();
}