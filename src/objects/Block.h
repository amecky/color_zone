#pragma once
#include <Vector.h>
#include <renderer\render_types.h>
#include "..\Common.h"

class Block {

public:
	Block(GameContext* context, bool showBorder);
	~Block();
	void render();
	void pickColors();
	void setPosition(const p2i& p);
	const p2i& getPosition() const;
	void copyColors(const Block* other);
	int getColor(int index) const;
	void rotate();
	void update(float dt);
private:
	bool _showBorder;
	GameContext* _ctx;
	p2i _position;
	int _colors[4];
	int _texture;
	int _boxTexture;
	bool _rotating;
	float _rotationTimer;
	float _rotationRadius;
};

