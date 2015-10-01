#pragma once
#include <compiler\Converter.h>
#include <data\DataTranslator.h>
#include <io\Serializer.h>

struct GameSettings {

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
};

class SettingsLoader : public ds::Serializer, public ds::Converter {

public:
	SettingsLoader() : ds::Converter("SettingsLoader") {
		m_SettingsTranslator.add("laser_start_delay", &GameSettings::laserStartDelay);
		m_SettingsTranslator.add("sparkle_gap", &GameSettings::sparkleGap);
		m_SettingsTranslator.add("sparkle_ttl", &GameSettings::sparkleTTL);
		m_SettingsTranslator.add("sparkle_start_scale", &GameSettings::sparkleStartScale);
		m_SettingsTranslator.add("sparkle_end_scale", &GameSettings::sparkleEndScale);
		m_SettingsTranslator.add("sparkle_velocity", &GameSettings::sparkleVelocity);
		m_SettingsTranslator.add("sparkle_velocity_variance", &GameSettings::sparkleVelocityVariance);
		m_SettingsTranslator.add("intro_warmup_timer", &GameSettings::introWarmupTimer);
		m_SettingsTranslator.add("intro_ttl", &GameSettings::introTTL);
		m_SettingsTranslator.add("intro_start_scale", &GameSettings::introStartScale);
		m_SettingsTranslator.add("intro_end_scale", &GameSettings::introEndScale);
		m_SettingsTranslator.add("intro_velocity", &GameSettings::introVelocity);
		m_SettingsTranslator.add("intro_velocity_variance", &GameSettings::introVelocityVariance);
	}
	void load(BinaryLoader* loader);
	void convert(JSONReader& reader, BinaryWriter& writer);
	const char* getResourceDirectory() {
		return "content\\settings";
	}
	GameSettings* get() {
		return &m_Settings;
	}
private:
	GameSettings m_Settings;
	ds::DataTranslator<GameSettings> m_SettingsTranslator;
};
