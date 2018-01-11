#include "game_ui.h"
#include "Common.h"
#include <ds_game_ui.h>
#include "TileMap.h"

void show_hud(GameContext* ctx) {
	dialog::begin();
	dialog::FormattedText(ds::vec2(120, 670), false, ds::vec2(1.0f), "%06d",ctx->score);
	dialog::FormattedText(ds::vec2(800, 670), false, ds::vec2(1.0f), "%02d:%02d", ctx->timer.minutes,ctx->timer.seconds);
	dialog::FormattedText(ds::vec2(400, 670), false, ds::vec2(1.0f), "%3d%%", ctx->fillRate);
	dialog::end();
}

int show_map_selection(GameContext* ctx) {
	dialog::begin();
	const char* name = get_level_name(ctx->levels, ctx->levelIndex);
	dialog::Text(ds::vec2(120, 550), name);
	// back button
	if (dialog::Button(ds::vec2(400, 550), ds::vec4(60, 210, 40, 40))) {
		--ctx->levelIndex;
		if (ctx->levelIndex < 0) {
			ctx->levelIndex = 0;
		}
		ctx->tileMap->build(ctx->levels, ctx->levelIndex);
	}
	// next button
	if (dialog::Button(ds::vec2(860, 550), ds::vec4(120, 210, 40, 40))) {
		++ctx->levelIndex;
		if (ctx->levelIndex >= MAX_LEVELS) {
			ctx->levelIndex = MAX_LEVELS - 1;
		}
		ctx->tileMap->build(ctx->levels, ctx->levelIndex);
	}
	// main menu button
	if (dialog::Button(ds::vec2(640, 150), ds::vec4(0, 260, 300, 50), "Select")) {
		return 1;
	}
	dialog::end();

	ctx->tileMap->render(ctx->buffer, HALF_SQUARE_SIZE, 0.5f, ctx->colors, ctx->settings);
	
	return 0;
}