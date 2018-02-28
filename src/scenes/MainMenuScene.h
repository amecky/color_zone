#pragma once
#include "..\BaseApp.h"

struct GameContext;

struct MenuAnimation {
	float timer;
	float ttl;
	ds::vec2 start;
	ds::vec2 target;
};

class MainMenuScene : public Scene {

	struct MenuItem {
		enum ENUM {
			LEFT_LOGO,RIGHT_LOGO,START_BUTTON,EXIT_BUTTON,HEADER
		};
	};

public:
	MainMenuScene(GameContext* ctx);
	virtual ~MainMenuScene() {}
	void render(SpriteBatchBuffer* buffer);
	void update(float dt);
	void onActivation();
	void showGUI();
private:
	ds::vec2 getPosition(int animIndex);
	MenuAnimation _animations[5];
	GameContext* _ctx;
};