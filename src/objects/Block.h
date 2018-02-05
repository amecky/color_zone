#pragma once
#include <diesel.h>
#include <SpriteBatchBuffer.h>
#include "..\Common.h"
#include <ds_imgui.h>

struct Block {
	p2i position;
	int colors[4];
	bool rotating;
	float rotationTimer;
	float rotationRadius;
	bool flashing;
	float flashTimer;
	ds::vec2 scale;
};

void initialise_block(Block* block);

void pick_block_colors(Block* block);

void copy_block_colors(Block* current, Block* other);

void render_block(Block* block, SpriteBatchBuffer* buffer, ds::Color* colors);

void render_block_boxed(Block* block, SpriteBatchBuffer* buffer, ds::Color* colors);

void follow_mouse(Block* block);

void flash_block_scale(Block* block, float dt, float flashTTL);

void rotate_block(Block* block, float dt);


