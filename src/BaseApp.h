#pragma once
#include <diesel.h>
#include "EventStream.h"
#include <Windows.h>
class SpriteBatchBuffer;

struct ApplicationSettings {
	bool useIMGUI;	
	bool useGPUProfiling;
	int screenWidth;
	int screenHeight;
	const char* windowTitle;
	ds::Color clearColor;	
};

class Scene {

public:
	Scene() : _active(false) , _initialized(false) {}
	virtual ~Scene() {}
	virtual void render(SpriteBatchBuffer* buffer) {}
	virtual void update(float dt) {}
	void prepare(ds::EventStream* events) {
		if (!_initialized) {
			_events = events;
			_initialized = true;
		}
	}
	virtual void initialize() {}
	void setActive(bool active) {
		if (active) {
			onActivation();
		}
		else {
			onDeactivation();
		}
		_active = active;
	}
	virtual void onActivation() {}
	virtual void onDeactivation() {}
	virtual void showGUI() {}
	bool isActive() const {
		return _active;
	}
protected:
	ds::EventStream* _events;
private:
	bool _active;
	bool _initialized;
};

class BaseApp {

public:
	BaseApp();
	~BaseApp();
	const ApplicationSettings& getSettings() const {
		return _settings;
	}
	void init();
	virtual void initialize() = 0;
	virtual void handleEvents(ds::EventStream* events) {}
	void tick(float dt);
	void setActiveScene(Scene* scene) {
		if (_activeScene != 0) {
			_activeScene->onDeactivation();
		}
		_activeScene = scene;
		_activeScene->prepare(_events);
		_activeScene->initialize();
		_activeScene->onActivation();
	}	
	void setSpriteBatchBuffer(SpriteBatchBuffer* buffer) {
		_buffer = buffer;
	}
	void initializeSettings(const char* settingsFileName);
	void loadSettings();
protected:	
	RID loadImageFromResource(LPCTSTR name, LPCTSTR type);
	ApplicationSettings _settings;
	ds::EventStream* _events;
	SpriteBatchBuffer* _buffer;	
private:
	Scene* _activeScene;
	float _loadTimer;
	const char* _settingsFileName;
	bool _useTweakables;
	bool _guiKeyPressed;
	bool _guiActive;
};

