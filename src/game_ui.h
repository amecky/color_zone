#pragma once
#include <diesel.h>

struct GameContext;

void show_hud(GameContext* ctx);

int show_map_selection(GameContext* ctx);

void show_tweakable_gui(const char* category, int* state);