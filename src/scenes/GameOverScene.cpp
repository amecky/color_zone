#include "GameOverScene.h"
#include "..\Common.h"
#include <ds_game_ui.h>
#include <ds_imgui.h>
#include "..\TileMap.h"
#include "..\EventStream.h"

// --------------------------------------------
// render
// --------------------------------------------
void GameOverScene::render(SpriteBatchBuffer* buffer) {
	dialog::begin();
	dialog::Text(ds::vec2(120, 650), "Select map", true, ds::vec2(1.5f));
	// name
	dialog::Image(ds::vec2(640, 550), ds::vec4(310, 220, 360, 40), ds::Color(6, 134, 235, 255));
	const char* name = _tiles->getLevelName();
	dialog::Text(ds::vec2(120, 550), name);

	int lvl = _tiles->getLevel();
	// back button
	if (dialog::Button(ds::vec2(400, 550), ds::vec4(60, 210, 40, 40))) {
		--lvl;
		if (lvl < 0) {
			lvl = 0;
		}
		_tiles->buildLevel(lvl);
	}
	// next button
	if (dialog::Button(ds::vec2(860, 550), ds::vec4(120, 210, 40, 40))) {
		++lvl;
		if (lvl >= MAX_LEVELS) {
			lvl = MAX_LEVELS - 1;
		}
		_tiles->buildLevel(lvl);
	}
	// main menu button
	if (dialog::Button(ds::vec2(640, 150), ds::vec4(0, 380, 300, 50), "Select", ds::Color(0,180,0,255))) {
		_events->add(100);
	}
	dialog::end();

	_tiles->render(_ctx->colors, 0.5f);
}