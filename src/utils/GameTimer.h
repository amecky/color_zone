#pragma once

struct TimerMode {

	enum Enum {
		TM_INC,TM_DEC
	};

};

struct TimerData {
	int minutes;
	int seconds;
	float timer;
	TimerMode::Enum mode;
};

void start_timer(TimerData * data, int minutes, int seconds, TimerMode::Enum mode);

void tick_timer(TimerData* data, float dt);
