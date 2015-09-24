#include "ColorZone.h"
#include <sprites\SpriteBatch.h>

ds::BaseApp *app = new ColorZone();

ColorZone::ColorZone() {
	//_CrtSetBreakAlloc(12876);
	m_Width = 1024;
	m_Height = 768;
	//m_ClearColor = ds::Color(0.0f, 0.0f, 0.0f, 1.0f);
	_editor = std::make_unique<TileMapEditor>();
	_game = std::make_unique<MainGame>();
}


ColorZone::~ColorZone() {
}

bool ColorZone::loadContent() {
	_textureID = ds::renderer::loadTexture("Textures");
	assert(_textureID != -1);
	ds::debug::loadSystemFont("Verdana", "Verdana", 14, true);	
	startGame();
	return true;
}

void ColorZone::startGame() {
	_mode = GM_MAIN;
	_game->start();
}

void ColorZone::OnChar(char ascii, unsigned int keyState) {
	if (_mode == GM_EDITOR) {
		_editor->OnChar(ascii);
	}
	else {
		_game->OnChar(ascii);
	}
}

void ColorZone::update(float dt) {
	if (_mode == GM_EDITOR) {
		_editor->update(dt);
	}
	else {
		_game->update(dt);
	}
}

void ColorZone::draw() {
	ds::sprites::begin();
	if (_mode == GM_EDITOR) {
		_editor->render();
	}
	else {
		_game->render();
	}
	ds::sprites::flush();
	if (_mode == GM_EDITOR) {
		ds::debug::drawDebugMessages();
	}
}

void ColorZone::OnButtonUp(int button, int x, int y) {
	if (_mode == GM_EDITOR) {
		_editor->click(button, x, y);
	}
	else {
		_game->click(button, x, y);
	}
	
}