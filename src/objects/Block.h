#pragma once
#include <Vector.h>
#include <renderer\render_types.h>
#include "..\Common.h"

struct Block {

	p2i position;
	int colors[4];
	int texture;
	int boxTexture;
	bool rotating;
	float rotationTimer;
	float rotationRadius;
	bool flashing;
	float flashTimer;
	v2 scale;
};

namespace block {

	void init(Block* block);

	void render(Block* block, ds::Color* colors);

	void render_boxed(Block* block, ds::Color* colors);

	void pick_colors(Block* block);

	void flash_scale(Block* block, float dt, float flashTTL);

	void copy_colors(Block* block,const Block* other);

	void start_rotating(Block* block);

	void update(Block* block,float dt);

	void follow_mouse(Block* block);

}
/*
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

*/