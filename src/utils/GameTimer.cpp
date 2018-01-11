#include "GameTimer.h"


void start_timer(TimerData * data, int minutes, int seconds, TimerMode::Enum mode) {
	data->timer = 0.0f;
	data->minutes = minutes;
	data->seconds = seconds;
	data->mode = mode;
}

void tick_timer(TimerData* data, float dt) {
	data->timer += dt;
	if (data->timer > 1.0f) {
		data->timer -= 1.0f;
		if (data->mode == TimerMode::TM_INC) {
			++data->seconds;
			if (data->seconds >= 60) {
				data->seconds = 0;
				++data->minutes;
			}
		}
		else {
			--data->seconds;
			if (data->seconds < 0) {
				data->seconds = 59;
				--data->minutes;
			}
		}
	}
}