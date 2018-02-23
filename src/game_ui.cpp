#include "game_ui.h"
#include "Common.h"
#include <ds_game_ui.h>
#include "objects\Levels.h"
#include <ds_tweakable.h>
#include <ds_imgui.h>
#include "GameSettings.h"
#include "TileMap.h"
#include "Common.h"
#include "EventStream.h"

void show_tweakable_gui(const char* category, int* state) {
	Tweakable  vars[256];
	if (gui::begin(category, state)) {
		int num = twk_get_tweakables(category, vars, 256);
		for (int i = 0; i < num; ++i) {
			if (vars[i].type == TweakableType::ST_FLOAT) {
				gui::Input(vars[i].name, vars[i].ptr.fPtr);
			}
			else if (vars[i].type == TweakableType::ST_INT) {
				gui::Input(vars[i].name, vars[i].ptr.iPtr);
			}
			else if (vars[i].type == TweakableType::ST_VEC2) {
				gui::Input(vars[i].name, vars[i].ptr.v2Ptr);
			}
			else if (vars[i].type == TweakableType::ST_VEC3) {
				gui::Input(vars[i].name, vars[i].ptr.v3Ptr);
			}
			else if (vars[i].type == TweakableType::ST_VEC4) {
				gui::Input(vars[i].name, vars[i].ptr.v4Ptr);
			}
			else if (vars[i].type == TweakableType::ST_COLOR) {
				gui::Input(vars[i].name, vars[i].ptr.cPtr);
			}
		}
	}
}



