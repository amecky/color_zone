#pragma once
#include <vector>
#include <ui\IMGUI.h>
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
		addFloat("laserStartDelay", &laserStartDelay, 2.0f);
		addFloat("sparkle_gap", &sparkleGap, 2.0f);
		addFloat("sparkle_ttl", &sparkleTTL, 0.6f);
		addFloat("sparkle_start_scale", &sparkleStartScale, 1.0f);
		addFloat("sparkle_end_scale", &sparkleEndScale, 0.4f);
		addFloat("sparkle_velocity", &sparkleVelocity, 20.0f);
		addFloat("sparkle_velocity_variance", &sparkleVelocityVariance, 4.0f);
		addFloat("laser_start_delay", &laserStartDelay, 10.0f);
		addFloat("intro_warmup_timer", &introWarmupTimer, 2.0f);
		addFloat("intro_ttl", &introTTL, 3.0f);
		addFloat("intro_start_scale", &introStartScale, 1.0f);
		addFloat("intro_end_scale", &introEndScale, 0.1f);
		addFloat("intro_velocity", &introVelocity, 100.0f);
		addFloat("intro_velocity_variance", &introVelocityVariance, 30.0f);
	}
};
