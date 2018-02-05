#include "MainGameState.h"
#include "..\Common.h"
#include "..\GameSettings.h"
#include "..\game_ui.h"
#include "..\utils\GameTimer.h"
#include <ds_tweening.h>

MainGameState::MainGameState(GameContext* ctx) : _ctx(ctx) {
	//_map->build(_levels, 0);
	_laser = new Laser(_ctx);
	_laser->reset();
	_effect = new SparkleEffect(_ctx);
}

void MainGameState::activate() {
	_ctx->score = 0;
	_ctx->fillRate = 0;
	//_ctx->tileMap->reset();
	//_ctx->tileMap->build(_ctx->levels,_ctx->levelIndex);
	_laser->start();
	_effect->reset();
	start_timer(&_ctx->timer, 0, 0, TimerMode::TM_INC);

}

void MainGameState::deactivate() {
}

// --------------------------------------------
// move laser
// --------------------------------------------
void MainGameState::moveLaser(float dt) {
	int column = -1;
	if (_laser->move(dt, &column)) {
		int colors[MAX_Y];
		//_ctx->tileMap->getColumn(column, colors);
		for (int i = 0; i < MAX_Y; ++i) {
			if (colors[i] != -1) {
				_effect->start(p2i(column, i), colors[i]);
			}
		}
		/*
		int cleared = _ctx->tileMap->clearColumn(column);
		_ctx->score += cleared * 100;
		if (cleared > 0) {
			_ctx->fillRate = _ctx->tileMap->getFillRate();
			//LOG << "fillRate: " << _context->fillRate;
			//_hud->setNumber(HUD_SCORE, _context->score);
			//_hud->updateTextFormatted(HUD_PERCENTAGE, "%d%%", _context->fillRate);
		}
		*/
	}
	if (_laser->tick(dt)) {
		//_hud->setNumber(HUD_LASER, 0);
	}
	if (!_laser->isRunning()) {
		//_hud->setNumber(HUD_LASER, _laser->getIdleSeconds() + 1);
	}
}

int MainGameState::tick(float elapsed) {
	
	moveLaser(elapsed);

	_effect->update(elapsed);

	tick_timer(&_ctx->timer, elapsed);
	
	return 0;
}

void MainGameState::render() {
	
	_laser->render();
	
	_effect->render();

	show_hud(_ctx);

}