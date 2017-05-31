#define DS_IMPLEMENTATION
#include <diesel.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define SPRITE_IMPLEMENTATION
#include <SpriteBatchBuffer.h>
#define DS_STATEMACHINE_IMPLEMENTATION
#include <StateMachine.h>
#define GAMESETTINGS_IMPLEMENTATION
#include <ds_tweakable.h>
#include "gamestates/MainGameState.h"
#include "GameSettings.h"
#include "..\resource.h"
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


	//unsigned char *data = stbi_load(name, &x, &y, &n, 4);
	unsigned char *data = stbi_load_from_memory((const unsigned char*)pMyBinaryData,imageSize,&x, &y, &n, 4);
	ds::TextureInfo info = { x,y,n,data,ds::TextureFormat::R8G8B8A8_UNORM , ds::BindFlag::BF_SHADER_RESOURCE };
	RID textureID = ds::createTexture(info, name);
	stbi_image_free(data);

	UnlockResource(myResourceData);
	FreeResource(myResourceData);

	return textureID;
}

void debug(const char* message) {
	OutputDebugString(message);
	OutputDebugString("\n");
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
	ds::init(rs);
	//
	// load the one and only texture
	//
	RID textureID = loadImage("content\\Textures.png");
	SpriteBatchBufferInfo sbbInfo = { 2048, textureID };
	SpriteBatchBuffer spriteBuffer(sbbInfo);

	bool rendering = true;
	bool update = true;
	bool pressed = false;

	twk_init("content\\settings.json");

	GameSettings settings;

	twk_load();

	GameContext ctx;
	ctx.settings = &settings;
	ctx.buffer = &spriteBuffer;
	ctx.pick_colors();
	ctx.score = 0;
	ctx.fillRate = 0;
	ctx.levelIndex = 1;
	StateMachine stateMachine;

	stateMachine.add(new MainGameState(&ctx));
	stateMachine.activate("MainState");

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
			stateMachine.tick(static_cast<float>(ds::getElapsedSeconds()));
		}
		//
		// handle all the events that might have occurred in one frame
		//
		//uint32_t num = stateMachine->numEvents();
		//for (uint32_t i = 0; i < num; ++i) {
			// the "get ready" message has elapsed so deactivate the state
			//if (stateMachine->getEventType(i) == ET_PREPARE_ELAPSED) {
				//stateMachine->deactivate("PrepareState");
				//mainState->startSpawning();
			//}
			
		stateMachine.render();
		// let us see how we are doing
		ds::dbgPrint(0, 0, "FPS: %d", ds::getFramesPerSecond());
		ds::dbgPrint(0, 1, "Running: %s", update ? "YES" : "NO");
		ds::dbgPrint(0, 2, "Score: %d Fillrate: %d Level: %d", ctx.score, ctx.fillRate, ctx.levelIndex);
		ds::end();
	}	
	ds::shutdown();
}