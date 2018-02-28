#include "BackgroundScene.h"
#include "..\Common.h"
#include <SpriteBatchBuffer.h>
#include <ds_game_ui.h>
#include <ds_imgui.h>
#include "..\EventStream.h"
#include <ds_tweening.h>
#include "..\GameSettings.h"

void BackgroundScene::onActivation() {

}

// ---------------------------------------------------------------
// update background data
// ---------------------------------------------------------------
void BackgroundScene::update(float dt) {
	_backgroundData.color = tweening::interpolate(tweening::linear, _ctx->colors[_backgroundData.current], _ctx->colors[_backgroundData.next], _backgroundData.timer, _backgroundData.ttl);
	_backgroundData.color.a = tweening::interpolate(tweening::linear, _backgroundData.firstAlpha, _backgroundData.secondAlpha, _backgroundData.timer, _backgroundData.ttl);

	_backgroundData.timer += dt;
	if (_backgroundData.timer > _backgroundData.ttl) {
		++_backgroundData.current;
		if (_backgroundData.current > 7) {
			_backgroundData.current = 0;
		}
		_backgroundData.next = _backgroundData.current + 1;
		if (_backgroundData.next > 7) {
			_backgroundData.next = 0;
		}
		_backgroundData.timer -= _backgroundData.ttl;
		_backgroundData.ttl = ds::random(_ctx->settings->background.min_ttl, _ctx->settings->background.max_ttl);
		_backgroundData.firstAlpha = _backgroundData.secondAlpha;
		_backgroundData.secondAlpha = ds::random(_ctx->settings->background.min_intensity, _ctx->settings->background.max_intensity);
	}
}

// --------------------------------------------
// render
// --------------------------------------------
void BackgroundScene::render(SpriteBatchBuffer* buffer) {
	buffer->add({ 640,360 }, { 320,620,640,360 }, { 2.0f,2.0f }, 0.0f, _backgroundData.color);
}