#include "Laser.h"

Laser::~Laser(void) {}

// -------------------------------------------------------
// Init
// -------------------------------------------------------
void Laser::init() {
	for ( int i = 0; i < LASER_PIECES; ++i ) {
		m_Entities[i] = m_World->create(4,201,Vector2f(200,200),ds::Rect(150,60,36,36));
	}
	m_Row = 0;
	reset();
}

// -------------------------------------------------------
// Start
// -------------------------------------------------------
void Laser::start() {
	m_Mode = RUNNING;
	m_Timer = 0.0f;
	setActive(true);
	for ( int i = 0; i < LASER_PIECES; ++i ) {
		ds::Actor& laser = m_World->getActor(m_Entities[i]);
		laser.active = true;
		laser.position = Vector2f(START_X,START_Y + i * 36);
	}
	m_Row = 0;
}

// -------------------------------------------------------
// Update
// -------------------------------------------------------
void Laser::update(float elapsed) {		
	if ( m_Mode == RUNNING ) {			
		m_Timer += elapsed;
		if ( m_Timer > 0.5f ) {
			++m_Row;
			m_Timer = 0.0f;
			for ( int i = 0; i < LASER_PIECES; ++i ) {
				ds::Actor& laser = m_World->getActor(m_Entities[i]);
				laser.position = Vector2f(START_X + m_Row * 36.0f,START_Y + i * 36.0f);
			}
			if ( m_Row == 26 ) {
				m_Row = 0;
				reset();
			}
		}		
	}
}
