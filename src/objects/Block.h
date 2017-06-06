#pragma once
#include <diesel.h>
#include <SpriteBatchBuffer.h>
#include "..\Common.h"

class Block {

public:
	Block();
	~Block() {}
	void render(SpriteBatchBuffer* buffer,ds::Color* colors);
	void render_boxed(SpriteBatchBuffer* buffer, ds::Color* colors);
	void pick_colors();
	void flash_scale(float dt, float flashTTL);
	void copy_colors(const Block* other);
	void start_rotating();
	void update(float dt);
	void follow_mouse();
	int getColor(int idx) const;
	void setPosition(const p2i& pos);
	void startFlashing();
	bool isFlashing() const {
		return _flashing;
	}
	const p2i& getPosition() const {
		return _position;
	}
private:
	p2i _position;
	int _colors[4];
	int _texture;
	int _boxTexture;
	bool _rotating;
	float _rotationTimer;
	float _rotationRadius;
	bool _flashing;
	float _flashTimer;
	ds::vec2 _scale;
};
