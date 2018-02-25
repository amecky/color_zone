#define DS_IMPLEMENTATION
#include <diesel.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define SPRITE_IMPLEMENTATION
#include <SpriteBatchBuffer.h>
#define GAMESETTINGS_IMPLEMENTATION
#include <ds_tweakable.h>
#define DS_GAME_UI_IMPLEMENTATION
#include <ds_game_ui.h>
#define DS_TWEENING_IMPLEMENTATION
#include <ds_tweening.h>
#define DS_IMGUI_IMPLEMENTATION
#include <ds_imgui.h>
#include "BaseApp.h"
//#include "ColorZone.h"

extern BaseApp* app;

// ---------------------------------------------------------------
// update background data
// ---------------------------------------------------------------
/*
void updateBackgroundData(GameContext* ctx, BackgroundData* data, float dt) {
	data->color = tweening::interpolate(tweening::linear, ctx->colors[data->current], ctx->colors[data->next], data->timer, data->ttl);
	data->color.a = tweening::interpolate(tweening::linear, data->firstAlpha, data->secondAlpha, data->timer, data->ttl);

	data->timer += dt;
	if (data->timer > data->ttl) {
		++data->current;
		if (data->current > 7) {
			data->current = 0;
		}
		data->next = data->current + 1;
		if (data->next > 7) {
			data->next = 0;
		}
		data->timer -= data->ttl;
		data->ttl = ds::random(ctx->settings->background.min_ttl, ctx->settings->background.max_ttl);
		data->firstAlpha = data->secondAlpha;
		data->secondAlpha = ds::random(ctx->settings->background.min_intensity, ctx->settings->background.max_intensity);
	}
}
*/
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {

	app->init();

	while (ds::isRunning()) {

		ds::begin();

		float dt = static_cast<float>(ds::getElapsedSeconds());

		app->tick(dt);
		
		ds::dbgPrint(0, 34, "FPS: %d", ds::getFramesPerSecond());

		ds::end();
	}	
	ds::shutdown();
	
	delete app;
	
}