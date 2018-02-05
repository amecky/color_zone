#pragma once
#include <diesel.h>

struct GameContext;
struct Tile;

void show_hud(GameContext* ctx);

int show_map_selection(Tile* tiles, GameContext* ctx);

void show_tweakable_gui(const char* category, int* state);