#include "MainMenuScene.h"
#include "..\Common.h"
#include <ds_game_ui.h>
#include <ds_imgui.h>
#include "..\TileMap.h"
#include "..\EventStream.h"
#include <ds_tweening.h>

MainMenuScene::MainMenuScene(GameContext* ctx) : Scene(), _ctx(ctx) {
	_animations[MenuItem::LEFT_LOGO]    = { 0.0f, 4.0f, ds::vec2(200, 1500), ds::vec2(200, 340) };
	_animations[MenuItem::RIGHT_LOGO]   = { 0.0f, 4.0f, ds::vec2(1000, -1500), ds::vec2(1000, 340) };
	_animations[MenuItem::START_BUTTON] = { 0.0f, 4.0f, ds::vec2(-400, 350), ds::vec2(640, 350) };
	_animations[MenuItem::EXIT_BUTTON]  = { 0.0f, 4.0f, ds::vec2(1680, 270), ds::vec2(640, 270) };
	_animations[MenuItem::HEADER]       = { 0.0f, 4.0f, ds::vec2(120, 1500), ds::vec2(120, 650) };
}

// --------------------------------------------
// render
// --------------------------------------------
void MainMenuScene::render(SpriteBatchBuffer* buffer) {
	dialog::begin();
	dialog::Text(getPosition(MenuItem::HEADER), "Main Menu", true, ds::vec2(1.5f));

	dialog::Image(getPosition(MenuItem::LEFT_LOGO), ds::vec4(800, 210, 105, 410));
	dialog::Image(getPosition(MenuItem::RIGHT_LOGO), ds::vec4(908, 210, 105, 330));
	// main menu button
	if (dialog::Button(getPosition(MenuItem::START_BUTTON), ds::vec4(0, 380, 300, 50), "Start game", ds::Color(0,180,0,255))) {
		_events->add(101);
	}
	if (dialog::Button(getPosition(MenuItem::EXIT_BUTTON), ds::vec4(0, 380, 300, 50), "Exit", ds::Color(180, 0, 0, 255))) {
		_events->add(102);
	}
	dialog::end();
}

void MainMenuScene::update(float dt) {
	for (int i = 0; i < 5; ++i) {
		if (_animations[i].timer < _animations[i].ttl) {
			_animations[i].timer += dt;
		}
	}
}

void MainMenuScene::onActivation() {
	for (int i = 0; i < 5; ++i) {
		_animations[i].timer = 0.0f;
	}
}

// --------------------------------------------
// showGUI
// --------------------------------------------
void MainMenuScene::showGUI() {
	p2i dp(10, 710);
	int state = 1;
	gui::start(&dp, 300);
	gui::begin("Menu", 0);
	if (gui::Button("Restart")) {
		for (int i = 0; i < 5; ++i) {
			_animations[i].timer = 0.0f;
		}
	}
	gui::end();
}

ds::vec2 MainMenuScene::getPosition(int animIndex) {
	const MenuAnimation& anim = _animations[animIndex];
	ds::vec2 p = anim.target;
	if (anim.timer <= anim.ttl) {
		p = tweening::interpolate(tweening::easeOutElastic, anim.start, anim.target, anim.timer, anim.ttl);
	}
	return p;
}
