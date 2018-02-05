#include "game_ui.h"
#include "Common.h"
#include <ds_game_ui.h>
#include "objects\Levels.h"
#include <ds_tweakable.h>
#include <ds_imgui.h>
#include "GameSettings.h"
#include "tiles.h"

void show_tweakable_gui(const char* category, int* state) {
	Tweakable  vars[256];
	if (gui::begin(category, state)) {
		int num = twk_get_tweakables(category, vars, 256);
		for (int i = 0; i < num; ++i) {
			if (vars[i].type == TweakableType::ST_FLOAT) {
				gui::Input(vars[i].name, vars[i].ptr.fPtr);
			}
			else if (vars[i].type == TweakableType::ST_INT) {
				gui::Input(vars[i].name, vars[i].ptr.iPtr);
			}
			else if (vars[i].type == TweakableType::ST_VEC2) {
				gui::Input(vars[i].name, vars[i].ptr.v2Ptr);
			}
			else if (vars[i].type == TweakableType::ST_VEC3) {
				gui::Input(vars[i].name, vars[i].ptr.v3Ptr);
			}
			else if (vars[i].type == TweakableType::ST_VEC4) {
				gui::Input(vars[i].name, vars[i].ptr.v4Ptr);
			}
			else if (vars[i].type == TweakableType::ST_COLOR) {
				gui::Input(vars[i].name, vars[i].ptr.cPtr);
			}
		}
	}
}

void show_hud(GameContext* ctx) {
	dialog::begin();
	dialog::FormattedText(ctx->settings->hud.score_position, false, ds::vec2(1.0f), "%06d",ctx->score);
	dialog::FormattedText(ctx->settings->hud.timer_position, false, ds::vec2(1.0f), "%02d:%02d", ctx->timer.minutes,ctx->timer.seconds);
	dialog::FormattedText(ctx->settings->hud.coverage_position, false, ds::vec2(1.0f), "%3d%%", ctx->fillRate);
	dialog::end();
}

int show_map_selection(Tile* tiles, GameContext* ctx) {
	dialog::begin();
	const char* name = get_level_name(ctx->levels, ctx->levelIndex);
	dialog::Text(ds::vec2(120, 550), name);
	// back button
	if (dialog::Button(ds::vec2(400, 550), ds::vec4(60, 210, 40, 40))) {
		--ctx->levelIndex;
		if (ctx->levelIndex < 0) {
			ctx->levelIndex = 0;
		}
		copy_level(ctx->levels, ctx->levelIndex, tiles);
	}
	// next button
	if (dialog::Button(ds::vec2(860, 550), ds::vec4(120, 210, 40, 40))) {
		++ctx->levelIndex;
		if (ctx->levelIndex >= MAX_LEVELS) {
			ctx->levelIndex = MAX_LEVELS - 1;
		}
		copy_level(ctx->levels, ctx->levelIndex, tiles);
	}
	// main menu button
	if (dialog::Button(ds::vec2(640, 150), ds::vec4(0, 260, 300, 50), "Select")) {
		return 1;
	}
	dialog::end();

	render_tiles(tiles, ctx->buffer, HALF_SQUARE_SIZE, 0.5f, ctx->colors, ctx->settings);
	
	return 0;
}