#include "Block.h"
#include <utils\Log.h>
#include "..\Constants.h"
#include <math\GameMath.h>

Block::Block() : ds::GameObject() , m_Map(0) {}

Block::~Block(void) {}

// -------------------------------------------------------
// Init
// -------------------------------------------------------
void Block::init() {
	for ( int i = 0; i < 4; ++i ) {
		m_Pieces[i] = m_World->create(2,100,Vector2f(200,200),ds::Rect(70,0,32,32));
	}	
	m_Border = m_World->create(2,100,Vector2f(200,200),ds::Rect(72,200,75,75));
	updatePosition(Vector2f(497,25),false);
	m_GridPosition = Vector2i(0,0);
	pickColors();
	
}

// -------------------------------------------------------
// Set position of block
// -------------------------------------------------------
void Block::setPosition(int mx,int my) {
	Vector2i gp = map::convertMousePosition(mx,my);
	if ( m_GridPosition != gp ) {
		Vector2f p = map::convert(gp);
		if ( m_Map != 0 ) {
			bool valid = true;
			for ( int i = 0; i < 4; ++i ) {
				if ( valid ) {
					int gx = gp.x + BLOCK_X[i];
					int gy = gp.y + BLOCK_Y[i];
					if ( !m_Map->isValid(gx,gy) ) {
						valid = false;
					}
					else if ( m_Map->get(gx,gy).state == TS_EMPTY ) {
						valid = false;
					}
				}
			}	
			if ( valid ) {
				m_GridPosition = gp;
				updatePosition(p);
			}
		}
		else {
			updatePosition(p);
		}
	}
}

// -------------------------------------------------------
// Pick random colors
// -------------------------------------------------------
void Block::pickColors() {
	int firstColor = ds::math::random(0.0f,4.0f);
	int secondColor = ds::math::random(0.0f,4.0f);
	int type = ds::math::random(0.0f,5.0f);
	int offset = type * 4;
	for ( int i = 0; i < 2; ++i ) {	
		setColor(BLOCK_FILL_ARRAY[offset+i],firstColor);
		setColor(BLOCK_FILL_ARRAY[offset+i+2],secondColor);
	}	
}

// -------------------------------------------------------
// Update position
// -------------------------------------------------------
void Block::updatePosition(const Vector2f& pos,bool checkBounds) {
	m_Position = pos;	
	if ( checkBounds ) {
		if ( m_Position.x < START_X ) {
			m_Position.x = START_X;
		}
		if ( m_Position.y < START_Y ) {
			m_Position.y = START_Y;
		}
	}
	for ( int i = 0; i < 4; ++i ) {
		Vector2f sp = m_Position;
		sp.x += ADD_X[i];
		sp.y += ADD_Y[i];
		ds::Actor& actor = m_World->getActor(m_Pieces[i]);
		actor.position = sp;
	}	
	Vector2f bp = m_Position;
	bp.x += 17;
	bp.y += 17;
	ds::Actor& border = m_World->getActor(m_Border);
	border.position = bp;
}

// -------------------------------------------------------
// Rotate colors clock wise
// -------------------------------------------------------
void Block::rotateColors() {
	int lf = getColor(0);
	for ( int i = 0; i < 3; ++i ) {
		setColor(i,getColor(i+1));
	}
	setColor(3,lf);
}

// -------------------------------------------------------
// Copy colors
// -------------------------------------------------------
void Block::copyColors(Block* block) {
	for ( int i = 0; i < 4; ++i ) {
		setColor(i,block->getColor(i));
	}
}
// -------------------------------------------------------
// Convert
// -------------------------------------------------------
bool Block::convert(Map* map) {
	if ( map->addBlock(m_Position,m_Colors[0],m_Colors[1],m_Colors[2],m_Colors[3]) ) {
		//m_Core->hud().setColors(this);
		//m_Core->hud().pickColors();
		//pickColors();
		return true;
	}
	return false;
}
