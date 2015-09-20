#include "TestMe.h"

ds::BaseApp *app = new TestMe();

TestMe::TestMe() {
	//_CrtSetBreakAlloc(12876);
	m_Width = 1024;
	m_Height = 768;
	m_ClearColor = ds::Color(0.0f, 0.0f, 0.0f, 1.0f);
	_map = new TileMap(MAX_X, MAX_Y);
}


TestMe::~TestMe() {
	delete _map;
}


//void initialize();
bool TestMe::loadContent() {
	_textureID = ds::renderer::loadTexture("Textures");
	_map->loadTextFile(2);
	return true;
}

void TestMe::OnChar(char ascii, unsigned int keyState) {

}

void TestMe::update(float dt) {
}

void TestMe::draw() {
	_map->render();
}

void TestMe::OnButtonUp(int button, int x, int y) {

}