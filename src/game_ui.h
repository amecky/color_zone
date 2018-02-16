#pragma once
#include <diesel.h>

struct GameContext;
struct Tile;
class TileMap;

void show_hud(GameContext* ctx);

int show_map_selection(TileMap& tiles, GameContext* ctx);

void show_tweakable_gui(const char* category, int* state);