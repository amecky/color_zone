#pragma once
#include <vector>
#include <imgui\IMGUI.h>
#include <data\DynamicSettings.h>

// -------------------------------------------------------
// game settings
// -------------------------------------------------------
struct MyGameSettings : public ds::DynamicGameSettings {

	float laserStartDelay;
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

	MyGameSettings() : ds::DynamicGameSettings() {
		add("sparkle_gap", &sparkleGap, 2.0f);
		add("sparkle_ttl", &sparkleTTL, 0.6f);
		add("sparkle_start_scale", &sparkleStartScale, 1.0f);
		add("sparkle_end_scale", &sparkleEndScale, 0.4f);
		add("sparkle_velocity", &sparkleVelocity, 20.0f);
		add("sparkle_velocity_variance", &sparkleVelocityVariance, 4.0f);
		add("laser_start_delay", &laserStartDelay, 10.0f);
		add("intro_warmup_timer", &introWarmupTimer, 2.0f);
		add("intro_ttl", &introTTL, 3.0f);
		add("intro_start_scale", &introStartScale, 1.0f);
		add("intro_end_scale", &introEndScale, 0.1f);
		add("intro_velocity", &introVelocity, 100.0f);
		add("intro_velocity_variance", &introVelocityVariance, 30.0f);
	}

	const char* getFileName() const {
		return "settings.json";
	}
};
