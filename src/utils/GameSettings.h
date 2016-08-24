#pragma once
#include <vector>
#include <imgui\IMGUI.h>
#include <data\DynamicSettings.h>

// -------------------------------------------------------
// game settings
// -------------------------------------------------------
struct GameSettings : public ds::DynamicGameSettings {

	// laser
	float laserStartDelay;
	float laserStepDelay;

	float sparkleGap;
	float sparkleTTL;
	float sparkleStartScale;
	float sparkleEndScale;
	float sparkleVelocity;
	float sparkleVelocityVariance;

	float introWarmupTimer;
	float introTTL;
	float introStartScale;
	float introEndScale;
	float introVelocity;
	float introVelocityVariance;

	// HUD
	struct HUD {
		int padding;
		v2 scorePosition;
		v2 coveragePosition;
		v2 timerPosition;
	} hud;

	struct Background {
		float minIntensity;
		float maxIntensity;
		float minTTL;
		float maxTTL;
	} background;

	GameSettings() : ds::DynamicGameSettings("content\\settings.json") {
		// sparkles
		add("sparkle.gap", &sparkleGap, 2.0f);
		add("sparkle.ttl", &sparkleTTL, 0.6f);
		add("sparkle.start_scale", &sparkleStartScale, 1.0f);
		add("sparkle.end_scale", &sparkleEndScale, 0.4f);
		add("sparkle.velocity", &sparkleVelocity, 20.0f);
		add("sparkle.velocity_variance", &sparkleVelocityVariance, 4.0f);
		// laser
		add("laser.start_delay", &laserStartDelay, 10.0f);
		add("laser.step_delay", &laserStepDelay, 10.0f);
		// intro
		add("intro.warmup_timer", &introWarmupTimer, 2.0f);
		add("intro.ttl", &introTTL, 3.0f);
		add("intro.start_scale", &introStartScale, 1.0f);
		add("intro.end_scale", &introEndScale, 0.1f);
		add("intro.velocity", &introVelocity, 100.0f);
		add("intro.velocity_variance", &introVelocityVariance, 30.0f);
		// HUD
		add("hud.padding", &hud.padding, 5);
		add("hud.score_position", &hud.scorePosition, v2(100, 100));
		add("hud.coverage_position", &hud.coveragePosition, v2(100, 400));
		add("hud.timer_position", &hud.timerPosition, v2(100, 400));
		// background
		add("background.min_intensity", &background.minIntensity, 0.6f);
		add("background.max_intensity", &background.maxIntensity, 0.8f);
		add("background.min_ttl", &background.minTTL, 2.0f);
		add("background.max_ttl", &background.maxTTL, 5.0f);
	}

	const char* getFileName() const {
		return "settings.json";
	}
};
