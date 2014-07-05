#pragma once
#include "..\Constants.h"
#include <base\GameObject.h>

class Laser : public ds::GameObject {

enum LaserMode {
	PREPARING,
	RUNNING,
	STOPPED
};

public:
	Laser() : ds::GameObject() , m_Delay(5.0f) , m_Timer(0.0f) {}
	~Laser(void);
	void init();
	void update(float elapsed);
	int getRow() const {
		return m_Row;
	}
	const bool isRunning() const {
		return m_Mode == RUNNING;
	}
	void start();
	void reset() {
		m_Timer = 0.0f;
		for ( int i = 0; i < LASER_PIECES; ++i ) {
			ds::Actor& actor = m_World->getActor(m_Entities[i]);
			actor.active = false;
		}
		m_Mode = STOPPED;
	}
	void render() {}
private:
	ID m_Entities[LASER_PIECES];
	LaserMode m_Mode;
	int m_Row;
	float m_XPos;
	float m_Delay;
	float m_Timer;
};

