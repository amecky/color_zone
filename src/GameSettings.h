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
		float waitDelay;
		float moveDelay;
		float minAlpha;
		float maxAlpha;
		float alphaTTL;
	} laser;
	
	struct Grid {
		ds::Color borderColor;
		ds::Color backgroundColor;
		ds::Color filledColor;
	} grid;

	struct Background {
		float min_intensity;
		float max_intensity;
		float min_ttl;
		float max_ttl;
	} background;

	struct HUD {
		ds::vec2 score_position;
		ds::vec2 coverage_position;
		ds::vec2 timer_position;
		ds::vec2 laser_idle_position;
	} hud;

	GameSettings() {
		twk_add("sparkle","gap", &sparkleGap);
		twk_add("sparkle", "ttl", &sparkleTTL);
		twk_add("sparkle", "start_scale", &sparkleStartScale);
		twk_add("sparkle", "end_scale", &sparkleEndScale);
		twk_add("sparkle", "velocity", &sparkleVelocity);
		twk_add("sparkle", "velocity_variance", &sparkleVelocityVariance);

		twk_add("laser", "start_delay", &laser.startDelay);
		twk_add("laser", "wait_delay", &laser.waitDelay);
		twk_add("laser", "move_delay", &laser.moveDelay);
		twk_add("laser", "min_alpha", &laser.minAlpha);
		twk_add("laser", "max_alpha", &laser.maxAlpha);
		twk_add("laser", "alpha_ttl", &laser.alphaTTL);

		twk_add("grid", "border_color", &grid.borderColor);
		twk_add("grid", "background_color", &grid.backgroundColor);
		twk_add("grid", "filled_color", &grid.filledColor);

		twk_add("background", "min_intensity", &background.min_intensity);
		twk_add("background", "max_intensity", &background.max_intensity);
		twk_add("background", "min_ttl", &background.min_ttl);
		twk_add("background", "max_ttl", &background.max_ttl);

		twk_add("hud", "score_position", &hud.score_position);
		twk_add("hud", "coverage_position", &hud.coverage_position);
		twk_add("hud", "timer_position", &hud.timer_position);
		twk_add("hud", "laser_idle_position", &hud.laser_idle_position);
	}
};
