#pragma once
#include "Map.h"
#include <base\GameObject.h>

class Block : public ds::GameObject {

public:
	Block();
	~Block(void);
	void init();
	void update( float elapsed ) {}
	const int getColor(int idx) const {
		return m_Colors[idx];
	}
	void pickColors();
	void rotateColors();
	bool convert(Map* map);
	void setPosition(int mx,int my);
	void setActive(bool active) {
		for ( int i = 0; i < 4; ++i ) {
			ds::Actor& actor = m_World->getActor(m_Pieces[i]);
			actor.active = active;
		}
		ds::Actor& border = m_World->getActor(m_Border);
		border.active = active;
	}
	void setColor(int idx,int color) {
		m_Colors[idx] = color;
		int x = color * 34;
		ds::Actor& actor = m_World->getActor(m_Pieces[idx]);
		actor.textureRect = ds::Rect(72,x,32,32);
	}
	void copyColors(Block* block);
	void render() {}
	void updatePosition(const Vector2f& pos,bool checkBounds = true);
	void setTileMap(TileMap* map) {
		m_Map = map;
	}
private:	
	Vector2i m_GridPosition;
	TileMap* m_Map;
	Vector2f m_Position;
	ID m_Border;
	ID m_Pieces[4];
	int m_Colors[4];
};

