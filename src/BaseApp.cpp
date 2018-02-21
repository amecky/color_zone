#include "BaseApp.h"
#include "EventStream.h"
#include <Windows.h>
#include <SpriteBatchBuffer.h>
#include <ds_tweakable.h>
#include <ds_imgui.h>

void debug(const LogLevel& level, const char* message) {
#ifdef DEBUG
	OutputDebugString(message);
	OutputDebugString("\n");
#endif
}

BaseApp::BaseApp() : _activeScene(0) {
	_settings.useIMGUI = true;
	_settings.useGPUProfiling = false;
	_settings.screenWidth = 1280;
	_settings.screenHeight = 720;
	_settings.windowTitle = "ColorZone";
	_settings.clearColor = ds::Color(0.9f,0.9f,0.9f,1.0f);
	_events = new ds::EventStream;
	_loadTimer = 0.0f;
	_useTweakables = false;
}

BaseApp::~BaseApp() {
	if (_useTweakables) {
		twk_shutdown();
	}
	if (_settings.useIMGUI) {
		gui::shutdown();
	}
	delete _events;
}

void BaseApp::init() {
	//
	// prepare application
	//
	ds::RenderSettings rs;
	rs.width = _settings.screenWidth;
	rs.height = _settings.screenHeight;
	rs.title = _settings.windowTitle;
	rs.clearColor = _settings.clearColor;
	rs.multisampling = 4;
	rs.useGPUProfiling = _settings.useGPUProfiling;
#ifdef DEBUG
	rs.logHandler = debug;
	rs.supportDebug = true;
#endif
	ds::init(rs);

	if (_settings.useIMGUI) {
		gui::init();
	}
}

void BaseApp::initializeSettings(const char* settingsFileName) {
	_settingsFileName = settingsFileName;
	_useTweakables = true;
#ifdef DEBUG
		twk_init(_settingsFileName);
#else
		twk_init();
#endif
}

void BaseApp::loadSettings() {
	if (_useTweakables) {
#ifdef DEBUG
		twk_load();
#else
		const char* txt = loadTextFile(_settingsFileName);
		if (txt != 0) {
			twk_parse(txt);
			delete[] txt;
		}
#endif
	}
}

void BaseApp::tick(float dt) {

	if (_useTweakables) {
#ifdef DEBUG
		_loadTimer += ds::getElapsedSeconds();
		if (_loadTimer >= 1.0f) {
			_loadTimer -= 1.0f;
			twk_load();
		}
#endif
	}

	_events->reset();
	if (_activeScene != 0) {
		// update
		_activeScene->update(dt);
		// render
		_buffer->begin();
		_activeScene->render(_buffer);
		_buffer->flush();
	}
}
