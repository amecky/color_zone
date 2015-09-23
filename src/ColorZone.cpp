#include "ColorZone.h"
#include <sprites\SpriteBatch.h>

ds::BaseApp *app = new ColorZone();

ColorZone::ColorZone() {
	//_CrtSetBreakAlloc(12876);
	m_Width = 1024;
	m_Height = 768;
	//m_ClearColor = ds::Color(0.0f, 0.0f, 0.0f, 1.0f);
	_map = new TileMap(MAX_X, MAX_Y);
	_editor = std::make_unique<TileMapEditor>(MAX_X, MAX_Y);
}


ColorZone::~ColorZone() {
	//delete _editor;
	delete _map;
}

bool ColorZone::loadContent() {
	_textureID = ds::renderer::loadTexture("Textures");
	assert(_textureID != -1);
	//_map->loadTextFile(3);
	return true;
}

void ColorZone::OnChar(char ascii, unsigned int keyState) {
	_editor->OnChar(ascii);
}

void ColorZone::update(float dt) {
}

void ColorZone::draw() {
	ds::sprites::begin();
	_editor->render();
	ds::sprites::flush();
}

void ColorZone::OnButtonUp(int button, int x, int y) {
	_editor->click(button, x, y);
}