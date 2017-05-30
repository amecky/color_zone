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
// ---------------------------------------------------------------
// load image using stb_image
// ---------------------------------------------------------------
RID loadImage(const char* name) {
	int x, y, n;
	unsigned char *data = stbi_load(name, &x, &y, &n, 4);
	ds::TextureInfo info = { x,y,n,data,ds::TextureFormat::R8G8B8A8_UNORM , ds::BindFlag::BF_SHADER_RESOURCE };
	RID textureID = ds::createTexture(info, name);
	stbi_image_free(data);
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

	GameContext ctx;
	ctx.pick_colors();

	MainGameState mainGameState(&spriteBuffer, &ctx);

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
			mainGameState.tick(static_cast<float>(ds::getElapsedSeconds()));
			//stateMachine->tick(static_cast<float>(ds::getElapsedSeconds()));
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
			
		//stateMachine->render();


		mainGameState.render();

		// let us see how we are doing
		ds::dbgPrint(0, 0, "FPS: %d", ds::getFramesPerSecond());
		ds::dbgPrint(0, 1, "Running: %s", update ? "YES" : "NO");
		ds::end();
	}	
	ds::shutdown();
}