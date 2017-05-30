#include "MainGameState.h"

MainGameState::MainGameState(SpriteBatchBuffer* buffer, GameContext* ctx) : _buffer(buffer) , _ctx(ctx) {
	_current.setPosition(p2i(512, 560));
	_current.pick_colors();
	_next.setPosition(p2i(512, 660));
	_next.pick_colors();
	_next.pick_colors();
	_buttonDown = false;
	_map = new TileMap(_ctx);
	_map->build(_levels, 0);
}

void MainGameState::tick(float elapsed) {
	if (ds::isMouseButtonPressed(0)) {
		_buttonDown = true;
	}
	else {
		if (_buttonDown) {
			_buttonDown = false;
			_current.copy_colors(&_next);
			_next.pick_colors();
		}
	}
}

void MainGameState::render() {
	_buffer->begin();
	_map->render(_buffer);
	_current.render(_buffer,_ctx->colors);
	_next.render(_buffer, _ctx->colors);
	_buffer->flush();
}