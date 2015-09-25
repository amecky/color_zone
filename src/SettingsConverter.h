#pragma once
#include <compiler\Converter.h>
#include <data\DataTranslator.h>
#include <io\Serializer.h>

struct GameSettings {

	float laserStartDelay;
};

class SettingsLoader : public ds::Serializer, public ds::Converter {

public:
	SettingsLoader() : ds::Converter("SettingsLoader") {
		m_SettingsTranslator.add("laser_start_delay", &GameSettings::laserStartDelay);
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
