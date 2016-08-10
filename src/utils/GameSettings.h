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

	GameSettings() : ds::DynamicGameSettings() {
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
	}

	const char* getFileName() const {
		return "settings.json";
	}
};
