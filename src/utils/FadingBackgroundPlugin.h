#pragma once
#include <core\plugin\Plugin.h>
#include "..\Common.h"

class FadingBackgroundPlugin : public ds::Plugin {

public:
	FadingBackgroundPlugin(GameContext* context);
	~FadingBackgroundPlugin();
	virtual void tick(float dt);
	virtual void preRender();
	virtual void activate();
private:
	ds::Texture _texture;
	GameContext* _context;
	float _timer;
	float _ttl;
	float _minValue;
	float _maxValue;
};

