#pragma once
#include <Vector.h>
#include <renderer\render_types.h>
#include "..\utils\GameSettings.h"

class Block {

public:
	Block(GameSettings* settings, bool showBorder);
	~Block();
	void render();
	void pickColors();
	void setPosition(const v2& p);
	const v2& getPosition() const;
	void copyColors(const Block* other);
	int getColor(int index) const;
	void rotate();
	void update(float dt);
private:
	bool _showBorder;
	GameSettings* _settings;
	v2 _position;
	int _colors[4];
	ds::Texture _textures[5];
	bool _rotating;
	float _rotationTimer;
	float _rotationRadius;
};

