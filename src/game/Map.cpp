#include "Map.h"
#include "..\Constants.h"
#include <algorithm>
#include <particles\ParticleSystem.h>
#include "Block.h"

const int XM[4] = {1,-1,0,0};
const int YM[4] = {0,0,1,-1};
const int LD[4] = {1,0,3,2};


Map::Map() : ds::GameObject() {
	m_Tiles.create(MAX_X,MAX_Y);
}


Map::~Map(void) {	
}

// -------------------------------------------------------
// Init
// -------------------------------------------------------
void Map::init() {
	clear();
}
// --------------------------------------------
//
// --------------------------------------------
bool Map::addBlock(const Vector2f& pos,int firstColor,int secondColor,int thirdColor,int fourthColor) {
	int x = ( pos.x - START_X ) / SQUARE_SIZE;
	int y = ( pos.y - START_Y ) / SQUARE_SIZE;	
	if ( m_Tiles.isValid(x,y) && m_Tiles.isBlockAvailable(x,y) ) {
		setColor(x,y,firstColor);
		setColor(x+1,y,secondColor);
		setColor(x+1,y+1,thirdColor);
		setColor(x,y+1,fourthColor);	
		//gEngine->getAudioManager().play("Move",100,false);
		for ( int i = 0; i < 4; ++i ) {
			int cnt = checkSurrounding(x + BLOCK_X[i], y + BLOCK_Y[i]);			
		}				
		return true;		
	}
	else {
		m_Audio->play("NoMove",30,false);
	}	
	return false;
}



// --------------------------------------------
//
// --------------------------------------------
void Map::markBlocks() {
	for ( int x = 0; x < MAX_X; ++x ) {
		for ( int y = 0; y < MAX_Y; ++y ) {
			if ( m_Tiles.isCoherent(x,y)) {
				markSingleBlock(x,y);
			}
		}
	}
}

// --------------------------------------------
// Mark single block
// --------------------------------------------
void Map::markSingleBlock(int gx,int gy) {
	for ( int i = 0; i < 4; ++i ) {
		markSingleEntry( gx + BLOCK_X[i], gy + BLOCK_Y[i]);		
	}	
}

// --------------------------------------------
// Mark single entry
// --------------------------------------------
bool Map::markSingleEntry(int gx,int gy) {
	Tile& t = m_Tiles.get(gx,gy);
	if ( t.state != TS_EMPTY ) {			
		t.state = TS_FILLED;
		return true;
	}		
	return false;
}



// --------------------------------------------
// Set color of grid
// --------------------------------------------
void Map::setColor(int gx,int gy,int color,bool filled) {
	Tile& t = m_Tiles.get(gx,gy);
	if ( color == -2 ) {
		t.state = TS_EMPTY;
	}
	else if ( color == -1 ) {
		t.state = TS_AVAILABLE;
	}
	else {
		if ( filled ) {
			t.state = TS_FILLED;
		}
		else {
			t.state = TS_MARKED;
		}
	}
	t.color = color;
}

// -------------------------------------------------------
// Clear entire map
// -------------------------------------------------------
void Map::clear() {
	for ( int x = 0; x < MAX_X; ++x ) {
		for ( int y = 0; y < MAX_Y; ++y) {
			Tile& t = m_Tiles.get(x,y);
			t.state = TS_EMPTY;
			t.corners = 0;
			t.cleared = false;
		}
	}
	m_BorderPieces.clear();
}

// --------------------------------------------
//
// --------------------------------------------
void Map::cleanMarkedBlocks() {
	for ( int x = 0; x < MAX_X; ++x ) {
		for ( int y = 0; y < MAX_Y; ++y ) {
			Tile& t = m_Tiles.get(x,y);
			t.state = TS_AVAILABLE;
			t.color = -1;
		}
	}
}

// -------------------------------------------------------
// Get coverage
// -------------------------------------------------------
int Map::getCoverage() {
	int covered = 0;
	for ( int y = 0; y < MAX_Y; ++y ) {	
		for ( int x = 0; x < MAX_X; ++x ) {
			if ( isValidEntry(x,y)) {
				++covered;
			}
		}
	}
	LOG << "covered " << covered << " total " << m_TotalTiles;
	float percentage = static_cast<float>(covered) / static_cast<float>(m_TotalTiles) * 100.0f;
	return static_cast<int>(percentage); 
}

// --------------------------------------------
//
// --------------------------------------------
int Map::cleanRow(int row) {
	int cnt = 0;
	if ( m_Tiles.isValid(row,0)) {		
		for ( int y = 0; y < MAX_Y; ++y ) {
			Tile& t = m_Tiles.get(row,y);
			if ( t.state == TS_MARKED || t.state == TS_FILLED ) {
				if ( t.state == TS_FILLED ) {					
					if ( !t.cleared ) {
						++cnt;
					}
					setEntryCorners(row,y);					
					t.cleared = true;
				}
				t.color = -1;				
				t.state = TS_AVAILABLE;
				setEntryCorners(row-1,y);
				setEntryCorners(row,y+1);
				++cnt;
			}	
		}
	}
	return cnt;
}

void Map::setEntryCorners(int x,int y) {
	if ( x >= 0 && y >= 0 && x < 26 && y < 16 ) {
		Tile& t = m_Tiles.get(x,y);
		if ( t.state == TS_FILLED || t.cleared ) {
			t.corners = map::determineCorners(m_Tiles,x,y);
		}
	}
}

// -------------------------------------------------------
// Determine if it is a valid tile
// -------------------------------------------------------
bool Map::isValidEntry(int x,int y) {
	if ( x >= 0 && y >= 0 && x < 26 && y < 16 ) {
		Tile& t = m_Tiles.get(x,y);
		return t.state == TS_FILLED;
	}
	return false;
}
// --------------------------------------------
//
// --------------------------------------------
void Map::update( float elapsed ) {		
}

int Map::checkSurrounding(int xp,int yp) {
	std::vector<Vector2f> colors;
	std::vector<int> used;
	int cnt = 0;
	check(xp,yp,-1,&colors,true);	
	if ( colors.size() > 2 ) {
		int idx = xp + yp * MAX_X;
		used.push_back(idx);
		if ( markSingleEntry(xp,yp) ) {
			++cnt;
		}
		for ( size_t i = 0; i < colors.size(); ++i ) {
			Vector2f p = colors[i];
			idx = p.x + p.y * MAX_X;
			std::vector<int>::iterator it = find(used.begin(), used.end(), idx);
			if ( it == used.end() ) {
				++cnt;
				used.push_back(idx);
			}
			markSingleEntry(p.x,p.y);
		}			
		return cnt;
	}
	return 0;	
}

// -------------------------------------------------------------
//
// -------------------------------------------------------------
void Map::check(int xp,int yp,int lastDir,std::vector<Vector2f> *list,bool rec) {
	if ( m_Tiles.isValid(xp,yp) ) {
		Tile& t = m_Tiles.get(xp,yp);
		int color = t.color;
		for ( int i = 0 ; i < 4; ++i ) {
			if ( i != lastDir ) {
				int sx = xp + XM[i];
				int sy = yp + YM[i];
				if ( m_Tiles.isValid(sx,sy)) {
					Tile& nt = m_Tiles.get(sx,sy);
					int nc = nt.color;
					while ( color == nc ) {												
						bool recheck = true;
						for ( int r = 0; r < list->size(); ++r ) {
							Vector2f v = list->at(r);
							if ( v.x == sx && v.y == sy ) {
								recheck = false;
							}
						}
						list->push_back(Vector2f(sx,sy));
						if ( recheck && rec ) {
							check(sx,sy,LD[i],list,rec);
						}												
						sx += XM[i];
						sy += YM[i];
						if ( m_Tiles.isValid(sx,sy)) {
							Tile& npe = m_Tiles.get(sx,sy);						
							nc = npe.color;
						}
						else {
							nc = -1;
						}						
					}
				}
			}
		}
	}
}

// -------------------------------------------------------
// Render
// -------------------------------------------------------
void Map::render() {
	for ( size_t i = 0; i < m_BorderPieces.size(); ++i ) {
		Border& b = m_BorderPieces[i];
		m_Renderer->draw(b.position,0,b.textureRect,0.0f,2.0f,2.0f);
	}
	
	map::render(m_Renderer,m_Tiles);

	
}

// --------------------------------------------
// Reset grid
// --------------------------------------------
void Map::reset(const Level& level,Block* block) {
	clear();
	levels::convert(level,m_Tiles);
	LOG << "Level: " << level.name;
	m_TotalTiles = 0;
	int clr = 0;
	for ( int x = 0; x < MAX_X; ++x ) {
		for ( int y = 0; y < MAX_Y; ++y ) {
			if ( m_Tiles.get(x,y).state == TS_AVAILABLE ) {
				clr = -1;
				++m_TotalTiles;

			}
			else {
				clr = -2;				
			}
			setColor(x,y,clr);
		}
	}
	block->setTileMap(&m_Tiles);

	for ( int x = 0; x < HALF_MAX_X; ++x ) {
		for ( int y = 0; y < HALF_MAX_Y; ++y ) {
			int bits = map::determineCorners(level.mapDefinition,x,y);			
			if ( bits != 0 && bits != 15 ) {
				Border b;
				b.position = Vector2f(START_X + (x * SQUARE_SIZE)*2+18,START_Y + (y * SQUARE_SIZE)*2+18);
				b.textureRect = ds::Rect(192,bits*40,40,40);
				m_BorderPieces.push_back(b);
			}
		}
	}
	LOG << "Border pieces: " << m_BorderPieces.size();
	LOG << "Total tiles: " << m_TotalTiles;
}
