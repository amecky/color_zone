#include "game_ui.h"
#include "Common.h"
#include <ds_game_ui.h>
#include "objects\Levels.h"
#include <ds_tweakable.h>
#include <ds_imgui.h>
#include "GameSettings.h"
#include "TileMap.h"
#include "Common.h"
#include "EventStream.h"

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
	ds::vec2 block_pos = ctx->settings->hud.score_position;
	block_pos.x += 60.0f;
	dialog::Image(block_pos, ds::vec4(310, 220, 160, 40), ds::Color(6,134,235,255));
	dialog::FormattedText(ctx->settings->hud.score_position, false, ds::vec2(1.0f), ds::Color(255, 255, 255, 255), "%06d",ctx->score);

	block_pos = ctx->settings->hud.coverage_position;
	block_pos.x += 60.0f;
	dialog::Image(block_pos, ds::vec4(310, 220, 120, 40), ds::Color(6, 134, 235, 255));
	dialog::FormattedText(ctx->settings->hud.coverage_position, false, ds::vec2(1.0f), ds::Color(255, 255, 255, 255), "%3d%%", ctx->fillRate);

	block_pos = ctx->settings->hud.timer_position;
	block_pos.x += 50.0f;
	dialog::Image(block_pos, ds::vec4(310, 220, 140, 40), ds::Color(6, 134, 235, 255));
	dialog::FormattedText(ctx->settings->hud.timer_position, false, ds::vec2(1.0f), ds::Color(255, 255, 255, 255), "%02d:%02d", ctx->timer.minutes,ctx->timer.seconds);
	
	block_pos = ctx->settings->hud.laser_idle_position;
	block_pos.x += 20.0f;
	dialog::Image(block_pos, ds::vec4(310, 220, 80, 40), ds::Color(6, 134, 235, 255));
	if (ctx->laserIdle > 0) {
		dialog::FormattedText(ctx->settings->hud.laser_idle_position, false, ds::vec2(1.0f), ds::Color(255, 255, 255, 255), "%2d", ctx->laserIdle);
	}
	dialog::end();
}

int show_map_selection(TileMap& map, GameContext* ctx) {
	dialog::begin();	
	// name
	dialog::Image(ds::vec2(640, 550), ds::vec4(310, 220, 360, 40), ds::Color(6, 134, 235, 255));
	const char* name = map.getLevelName();
	dialog::Text(ds::vec2(120, 550), name);

	int lvl = map.getLevel();
	// back button
	if (dialog::Button(ds::vec2(400, 550), ds::vec4(60, 210, 40, 40))) {
		--lvl;
		if (lvl < 0) {
			lvl = 0;
		}
		map.buildLevel(lvl);
		//copy_level(ctx->levels, ctx->levelIndex, tiles);
	}
	// next button
	if (dialog::Button(ds::vec2(860, 550), ds::vec4(120, 210, 40, 40))) {
		++lvl;
		if (lvl >= MAX_LEVELS) {
			lvl = MAX_LEVELS - 1;
		}
		map.buildLevel(lvl);
		//copy_level(ctx->levels, ctx->levelIndex, tiles);
	}
	// main menu button
	if (dialog::Button(ds::vec2(640, 150), ds::vec4(0, 260, 300, 50), "Select")) {
		return 1;
	}
	dialog::end();

	//render_tiles(tiles, ctx->buffer, HALF_SQUARE_SIZE, 0.5f, ctx->colors, ctx->settings);
	map.render(ctx->colors, 0.5f);
	
	return 0;
}

