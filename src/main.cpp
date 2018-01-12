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
#include "gamestates/MainGameState.h"
#include "GameSettings.h"
#include "..\resource.h"
#include "objects\Levels.h"
#include "TileMap.h"
#include "game_ui.h"
// ---------------------------------------------------------------
// load text file
// ---------------------------------------------------------------
const char* loadTextFile(const char* name) {
	
	HRSRC resourceHandle = ::FindResource(NULL, MAKEINTRESOURCE(IDS_SETTINGS), RT_RCDATA);
	if (resourceHandle == 0) {
		return 0;
	}
	DWORD imageSize = ::SizeofResource(NULL, resourceHandle);
	if (imageSize == 0) {
		return 0;
	}
	HGLOBAL myResourceData = ::LoadResource(NULL, resourceHandle);
	char* pMyBinaryData = (char*)::LockResource(myResourceData);
	UnlockResource(myResourceData);
	char* ret = new char[imageSize];
	memcpy(ret, pMyBinaryData, imageSize);
	FreeResource(myResourceData);
	return ret;
}

// ---------------------------------------------------------------
// load image using stb_image
// ---------------------------------------------------------------
RID loadImage(const char* name) {
	int x, y, n;
	HRSRC resourceHandle = ::FindResource(NULL, MAKEINTRESOURCE(IDB_PNG1), "PNG");
	if (resourceHandle == 0) {
		return NO_RID;
	}
	DWORD imageSize = ::SizeofResource(NULL, resourceHandle);
	if (imageSize == 0) {
		return NO_RID;
	}
	HGLOBAL myResourceData = ::LoadResource(NULL, resourceHandle);
	void* pMyBinaryData = ::LockResource(myResourceData);
	unsigned char *data = stbi_load_from_memory((const unsigned char*)pMyBinaryData, imageSize, &x, &y, &n, 4);
	ds::TextureInfo info = { x,y,n,data,ds::TextureFormat::R8G8B8A8_UNORM , ds::BindFlag::BF_SHADER_RESOURCE };
	RID textureID = ds::createTexture(info, name);
	stbi_image_free(data);
	UnlockResource(myResourceData);
	FreeResource(myResourceData);
	return textureID;
}

void debug(const char* message) {
#ifdef DEBUG
	OutputDebugString(message);
	OutputDebugString("\n");
#endif
}

// ---------------------------------------------------------------
// bitmap font definitions
// ---------------------------------------------------------------
static const ds::vec2 FONT_DEF[] = {
	ds::vec2(1,24),   // A
	ds::vec2(24,21),  // B
	ds::vec2(45,20),  // C
	ds::vec2(66,22),  // D
	ds::vec2(88,19),  // E
	ds::vec2(108,19), // F
	ds::vec2(127,21), // G
	ds::vec2(149,21), // H
	ds::vec2(170, 9), // I
	ds::vec2(179,13), // J
	ds::vec2(192,21), // K
	ds::vec2(213,19), // L
	ds::vec2(232,29), // M
	ds::vec2(261,21), // N
	ds::vec2(282,23), // O
	ds::vec2(305,21), // P
	ds::vec2(327,21), // Q
	ds::vec2(348,21), // R
	ds::vec2(369,19), // S 
	ds::vec2(388,19), // T
	ds::vec2(407,21), // U
	ds::vec2(428,24), // V
	ds::vec2(452,30), // W
	ds::vec2(482,23), // X
	ds::vec2(505,22), // Y
	ds::vec2(527,19)  // Z
};

// ---------------------------------------------------------------
// build the font info needed by the game ui
// ---------------------------------------------------------------
void prepareFontInfo(dialog::FontInfo* info) {
	// default for every character just empty space
	for (int i = 0; i < 255; ++i) {
		info->texture_rects[i] = ds::vec4(112, 4, 20, 19);
	}
	// numbers
	for (int c = 48; c <= 57; ++c) {
		int idx = (int)c - 48;
		info->texture_rects[c] = ds::vec4(548 + idx * 22, 440, 22, 19);
	}
	// :
	info->texture_rects[58] = ds::vec4(766, 440, 18, 19);
	// %
	info->texture_rects[37] = ds::vec4(788, 440, 36, 19);
	// characters
	for (int c = 65; c <= 90; ++c) {
		ds::vec2 fd = FONT_DEF[(int)c - 65];
		info->texture_rects[c] = ds::vec4(0.0f + fd.x, 440.0f, fd.y, 19.0f);
	}
}

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

struct GameMode {

	enum Enum {
		GM_SELECT_MAP,
		GM_MAIN
	};
};
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	//
	// prepare application
	//
	ds::RenderSettings rs;
	rs.width = 1280;
	rs.height = 720;
	rs.title = "color_zone";
	rs.clearColor = ds::Color(0.0f, 0.0f, 0.0f, 1.0f);
	rs.multisampling = 4;
	rs.useGPUProfiling = false;
	ds::init(rs);
	//
	// load the one and only texture
	//
	RID textureID = loadImage("content\\Textures.png");
	SpriteBatchBufferInfo sbbInfo = { 2048, textureID , ds::TextureFilters::LINEAR};
	SpriteBatchBuffer spriteBuffer(sbbInfo);

	bool rendering = true;
	bool update = true;
	bool pressed = false;	
	
	gui::init();

#ifdef DEBUG
	twk_init("content\\settings.json");
#else
	twk_init();
#endif

	GameSettings settings;

#ifdef DEBUG
	twk_load();
#else
	const char* txt = loadTextFile("content\\settings.json");
	if (txt != 0) {
		twk_parse(txt);
		delete[] txt;
	}
#endif

	GameContext ctx;
	ctx.settings = &settings;
	ctx.buffer = &spriteBuffer;
	ctx.pick_colors();
	ctx.score = 0;
	ctx.fillRate = 0;
	ctx.levelIndex = 0;
	ctx.tileMap = new TileMap;

	ctx.levels = new LevelData;
	load_levels(ctx.levels);
	ctx.tileMap->reset();
	ctx.tileMap->build(ctx.levels, 0);

	dialog::FontInfo fontInfo;
	prepareFontInfo(&fontInfo);
	dialog::init(&spriteBuffer, fontInfo);

	MainGameState* mainState = new MainGameState(&ctx);
	mainState->activate();

	BackgroundData backgroundData;
	backgroundData.current = 0;
	backgroundData.next = 1;
	backgroundData.firstAlpha = 1.0f;
	backgroundData.secondAlpha = 1.0f;
	backgroundData.timer = 0.0f;
	backgroundData.ttl = 2.0f;

	float loadTimer = 0.0f;

	int settingsStates[6] = { 0 };

	GameMode::Enum mode = GameMode::GM_SELECT_MAP;

	while (ds::isRunning() && rendering) {

		ds::begin();
		
		if (ds::isKeyPressed('U') ) {
			if (!pressed) {
				update = !update;
				pressed = true;
			}
		}
		else {
			pressed = false;
		}

		if (update) {
			if (mode == GameMode::GM_MAIN) {
				mainState->tick(static_cast<float>(ds::getElapsedSeconds()));
			}
		}
#ifdef DEBUG
		loadTimer += ds::getElapsedSeconds();
		if (loadTimer >= 1.0f) {
			loadTimer -= 1.0f;
			twk_load();
		}
#endif
		updateBackgroundData(&ctx, &backgroundData, ds::getElapsedSeconds());

		spriteBuffer.begin();

		spriteBuffer.add({ 640,360 }, { 320,620,640,360 }, { 2.0f,2.0f }, 0.0f, backgroundData.color);

		
		if (mode == GameMode::GM_SELECT_MAP) {
			int r = show_map_selection(&ctx);
			if (r == 1) {
				mainState->activate();
				mode = GameMode::GM_MAIN;
			}
		}
		else if (mode == GameMode::GM_MAIN) {
			mainState->render();
		}
		
		spriteBuffer.flush();

		p2i dp(10, 710);
		int state = 1;
		gui::start(&dp, 300);
		int nc = twk_num_categories();
		for (int i = 0; i < nc; ++i) {
			show_tweakable_gui(twk_get_category_name(i), &settingsStates[i]);
		}
		gui::end();

		ds::dbgPrint(0, 34, "FPS: %d", ds::getFramesPerSecond());

		ds::end();
	}	
	ds::shutdown();
	gui::shutdown();
	twk_shutdown();
	// FIXME: clean up GameContext
	delete[] ctx.levels->tiles;
	delete ctx.levels;
	delete ctx.tileMap;
	delete mainState;
}