#include "ColorZone.h"

ColorZone::ColorZone() : BaseApp() {
	_settings.screenWidth = 1280;
	_settings.screenHeight = 720;
	_settings.windowTitle = "ColorZone";
	_settings.clearColor = ds::Color(16,16,16,255);
}


ColorZone::~ColorZone() {
}

void ColorZone::initialize() {

}
