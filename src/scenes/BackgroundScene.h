#pragma once
#include "..\BaseApp.h"

struct GameContext;

struct BackgroundData {
	int current;
	int next;
	float firstAlpha;
	float secondAlpha;
	float timer;
	float ttl;
	ds::Color color;
};

class BackgroundScene : public Scene {

public:
	BackgroundScene(GameContext* ctx) : Scene() , _ctx(ctx) {
		_backgroundData.current = 0;
		_backgroundData.next = 1;
		_backgroundData.firstAlpha = 1.0f;
		_backgroundData.secondAlpha = 1.0f;
		_backgroundData.timer = 0.0f;
		_backgroundData.ttl = 2.0f;
		_backgroundData.color = ds::Color(255,255,255, 255);
	}
	virtual ~BackgroundScene() {}
	void onActivation();
	void update(float dt);
	void render(SpriteBatchBuffer* buffer);
private:
	GameContext* _ctx;
	BackgroundData _backgroundData;
};