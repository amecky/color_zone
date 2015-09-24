#pragma once
#include <Vector.h>
#include <renderer\render_types.h>

class Block {

public:
	Block();
	~Block();
	void render();
	void pickColors();
	void setPosition(const v2& p);
	const v2& getPosition() const;
	void copyColors(const Block& other);
	int getColor(int index) const;
	void rotate();
private:
	v2 _position;
	int _colors[4];
	ds::Texture _textures[5];
};

