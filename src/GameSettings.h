#pragma once
#include <ds_tweakable.h>

struct GameSettings {

	float sparkleGap;
	float sparkleTTL;
	float sparkleStartScale;
	float sparkleEndScale;
	float sparkleVelocity;
	float sparkleVelocityVariance;

	struct Laser {
		float startDelay;
		float stepDelay;
	} laser;
	
	struct Grid {
		ds::Color borderColor;
		ds::Color backgroundColor;
		ds::Color filledColor;
	} grid;

	GameSettings() {
		twk_add("sparkle","gap", &sparkleGap);
		twk_add("sparkle", "ttl", &sparkleTTL);
		twk_add("sparkle", "start_scale", &sparkleStartScale);
		twk_add("sparkle", "end_scale", &sparkleEndScale);
		twk_add("sparkle", "velocity", &sparkleVelocity);
		twk_add("sparkle", "velocity_variance", &sparkleVelocityVariance);

		twk_add("laser", "start_delay", &laser.startDelay);
		twk_add("laser", "step_delay", &laser.stepDelay);

		twk_add("grid", "border_color", &grid.borderColor);
		twk_add("grid", "background_color", &grid.backgroundColor);
		twk_add("grid", "filled_color", &grid.filledColor);
	}
};
