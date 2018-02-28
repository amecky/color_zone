#include "BaseApp.h"
#include "EventStream.h"
#include <Windows.h>
#include <SpriteBatchBuffer.h>
#include <ds_tweakable.h>
#include <ds_imgui.h>
#include <stb_image.h>
#include "..\resource.h"

void debug(const LogLevel& level, const char* message) {
#ifdef DEBUG
	OutputDebugString(message);
	OutputDebugString("\n");
#endif
}

BaseApp::BaseApp() {
	_settings.useIMGUI = true;
	_settings.useGPUProfiling = false;
	_settings.screenWidth = 1280;
	_settings.screenHeight = 720;
	_settings.windowTitle = "ColorZone";
	_settings.clearColor = ds::Color(0.9f,0.9f,0.9f,1.0f);
	_events = new ds::EventStream;
	_loadTimer = 0.0f;
	_useTweakables = false;
	_guiKeyPressed = false;
	_guiActive = true;
	_running = true;
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

// ---------------------------------------------------------------
// load image using stb_image
// ---------------------------------------------------------------
RID BaseApp::loadImageFromResource(LPCTSTR name, LPCTSTR type) {
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
		ds::log(LogLevel::LL_DEBUG, "=> IMGUI is enabled");
		gui::init();
	}

	ds::log(LogLevel::LL_DEBUG,"=> Press 'D' to toggle GUI");
	initialize();
}

void BaseApp::initializeSettings(const char* settingsFileName) {
	_settingsFileName = settingsFileName;
	_useTweakables = true;
	ds::log(LogLevel::LL_DEBUG, "=> Tweakables are enabled");
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

	if (ds::isKeyPressed('D')) {
		if (!_guiKeyPressed) {
			_guiActive = !_guiActive;
			_guiKeyPressed = true;
		}
	}
	else {
		_guiKeyPressed = false;
	}

	_events->reset();

	_buffer->begin();
	ScenesIterator it = _scenes.begin();
	while (it != _scenes.end()) {
		(*it)->update(dt);
		++it;
	}
	it = _scenes.begin();
	while (it != _scenes.end()) {
		(*it)->render(_buffer);
		++it;
	}
	_buffer->flush();

	it = _scenes.begin();
	while (it != _scenes.end()) {
		(*it)->showGUI();
		++it;
	}
	/*
	if (_activeScene != 0) {
		// update
		_activeScene->update(dt);
		// render
		_buffer->begin();
		_activeScene->render(_buffer);
		_buffer->flush();

		if (_settings.useIMGUI && _guiActive) {
			_activeScene->showGUI();
		}
	}
	*/
	handleEvents(_events);
}
