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
#include <ds_stretchbuffer.h>
#include "tiles.h"
#include "objects\Block.h"
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
RID loadImageFromResource(LPCTSTR name, LPCTSTR type) {
	int x, y, n;
	HRSRC resourceHandle = ::FindResource(NULL, name, type);
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
	RID textureID = ds::createTexture(info, "Texture");
	stbi_image_free(data);
	UnlockResource(myResourceData);
	FreeResource(myResourceData);
	return textureID;
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

void debug(const LogLevel& level, const char* message) {
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

// ---------------------------------------------------------------
// update background data
// ---------------------------------------------------------------
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
		GM_MAIN,
		GM_TEST
	};
};

struct ButtonDefinition {

	enum Enum {
		NONE,
		LEFT,
		RIGHT
	};

};

ButtonDefinition::Enum handle_buttons(int* buttonDown) {
	ButtonDefinition::Enum buttonClicked = ButtonDefinition::NONE;
	if (ds::isMouseButtonPressed(0)) {
		buttonDown[0] = 1;
	}
	else {
		if (buttonDown[0] == 1) {
			buttonDown[0] = 0;
			buttonClicked = ButtonDefinition::LEFT;
		}
	}
	if (ds::isMouseButtonPressed(1)) {
		buttonDown[1] = 1;
	}
	else {
		if (buttonDown[1] == 1) {
			buttonDown[1] = 0;
			buttonClicked = ButtonDefinition::RIGHT;
		}
	}
	return buttonClicked;
}
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
#ifdef DEBUG
	rs.logHandler = debug;
	rs.supportDebug = true;
#endif
	ds::init(rs);
	//
	// load the one and only texture
	//
	RID textureID = loadImageFromResource(MAKEINTRESOURCE(IDB_PNG1), "PNG");
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
	ctx.tiles = 0;
	array_resize(ctx.tiles, MAX_X * MAX_Y);
	ctx.levels = new LevelData;
	load_levels(ctx.levels);
	ctx.pick_colors();
	ctx.currentBlock = new Block;
	initialise_block(ctx.currentBlock);
	ctx.nextBlock = new Block;
	initialise_block(ctx.nextBlock);
	ctx.currentBlock->position = p2i(640, 560);
	pick_block_colors(ctx.currentBlock);
	ctx.nextBlock->position = p2i(640, 660);
	for (int i = 0; i < 6; ++i) {
		pick_block_colors(ctx.nextBlock);
	}
	copy_block_colors(ctx.currentBlock, ctx.nextBlock);
	pick_block_colors(ctx.nextBlock);
	//ctx.nextBlock->startFlashing();

	dialog::FontInfo fontInfo;
	prepareFontInfo(&fontInfo);
	dialog::init(&spriteBuffer, fontInfo);

	//MainGameState* mainState = new MainGameState(&ctx);
	//mainState->activate();

	BackgroundData backgroundData;
	backgroundData.current = 0;
	backgroundData.next = 1;
	backgroundData.firstAlpha = 1.0f;
	backgroundData.secondAlpha = 1.0f;
	backgroundData.timer = 0.0f;
	backgroundData.ttl = 2.0f;

	float loadTimer = 0.0f;

	int settingsStates[6] = { 0 };

	GameMode::Enum mode = GameMode::GM_TEST;

	bool showGUI = true;
	bool guiKeyPressed = false;

	int buttonDown[2] = { 0 };
	
	Laser laser;
	initialize_laser(&laser);

	copy_level(ctx.levels, ctx.levelIndex, ctx.tiles);

	SparkleEffect* sparkleEffect = new SparkleEffect(&ctx);
	sparkleEffect->reset();

	int dbgSparkCol = 2; // FIXME: remove 

	while (ds::isRunning() && rendering) {

		ds::begin();

		float dt = static_cast<float>(ds::getElapsedSeconds());

		ButtonDefinition::Enum buttonClicked = handle_buttons(buttonDown);
		
		if (buttonClicked == ButtonDefinition::RIGHT) {
			if (!ctx.currentBlock->rotating) {
				ctx.currentBlock->rotating = true;
				ctx.currentBlock->rotationTimer = 0.0f;
			}
		}
		if (buttonClicked == ButtonDefinition::LEFT) {
			if (copy_block(ctx.tiles, ctx.currentBlock)) {
				copy_block_colors(ctx.currentBlock,ctx.nextBlock);
				pick_block_colors(ctx.nextBlock);
				ctx.nextBlock->flashing = true;
				ctx.nextBlock->flashTimer = 0.0f;
				calculate_fill_rate(ctx.tiles);
			}
		}

		if (ds::isKeyPressed('U') ) {
			if (!pressed) {
				update = !update;
				pressed = true;
			}
		}
		else {
			pressed = false;
		}
		if (ds::isKeyPressed('D')) {
			if (!guiKeyPressed) {
				showGUI = !showGUI;
				guiKeyPressed = true;
			}
		}
		else {
			guiKeyPressed = false;
		}

		if (update) {
			if (mode == GameMode::GM_MAIN) {
				//mainState->tick(static_cast<float>(ds::getElapsedSeconds()));
			}
			else if (mode == GameMode::GM_TEST) {
				sparkleEffect->update(dt);
				follow_mouse(ctx.currentBlock);
				rotate_block(ctx.currentBlock,dt);
				flash_block_scale(ctx.nextBlock, dt, 0.2f);
				tick_laser(&laser, dt);
				flash_laser(&laser, dt, ctx.settings->laser.minAlpha, ctx.settings->laser.maxAlpha, ctx.settings->laser.alphaTTL);
				//l.tick(ds::getElapsedSeconds());
				if (laser.active) {
					if (move_laser(&laser, dt, ctx.settings->laser.startDelay, ctx.settings->laser.waitDelay, ctx.settings->laser.moveDelay)) {
						// column has changed
					}
					//if (l.isRunning()) {
						//l.move(ds::getElapsedSeconds(), &laser_col);
					//}
				}
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
			int r = show_map_selection(ctx.tiles, &ctx);
			if (r == 1) {
				//mainState->activate();
				mode = GameMode::GM_MAIN;
			}
		}
		else if (mode == GameMode::GM_MAIN) {
			//mainState->render();
		}
		else if (mode == GameMode::GM_TEST) {
			render_tiles(ctx.tiles, &spriteBuffer, SQUARE_SIZE, 1.0f, ctx.colors, ctx.settings);
			render_block_boxed(ctx.currentBlock,&spriteBuffer, ctx.colors);
			render_block(ctx.nextBlock,&spriteBuffer, ctx.colors);			
			render_laser(&laser,&spriteBuffer);
			sparkleEffect->render();
			ds::dbgPrint(0, 0, "LC %d", laser.column);
		}
		
		spriteBuffer.flush();
		if (showGUI) {
			p2i dp(10, 710);
			int state = 1;
			gui::start(&dp, 300);
			int nc = twk_num_categories();
			for (int i = 0; i < nc; ++i) {
				show_tweakable_gui(twk_get_category_name(i), &settingsStates[i]);
			}
			if (gui::begin("Laser",&state)) {
				//gui::Value("Idle", l.getIdleSeconds());
				if (gui::Button("Start")) {
					start_laser(&laser, 4.0f);// ctx.settings->laser.startDelay);
				}
			}
			gui::Input("Spark Col", &dbgSparkCol);
			if (gui::Button("Sparkles")) {
				int colors[MAX_Y];
				get_colors(ctx.tiles, dbgSparkCol, colors);
				for (int i = 0; i < MAX_Y; ++i) {
					if (colors[i] != -1) {
						sparkleEffect->start(p2i(dbgSparkCol, i), colors[i]);
					}
				}
			}
			gui::end();
		}
		ds::dbgPrint(0, 34, "FPS: %d", ds::getFramesPerSecond());

		ds::end();
	}	
	ds::shutdown();
	gui::shutdown();
	twk_shutdown();
	array_free(ctx.tiles);
	// FIXME: clean up GameContext
	delete sparkleEffect;
	delete ctx.currentBlock;
	delete ctx.nextBlock;
	delete[] ctx.levels->tiles;
	delete ctx.levels;
}