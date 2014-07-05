#include "LevelManager.h"
#include "..\Constants.h"
#include<iostream>
#include<fstream>
#include <string>
#include <utils\FileUtils.h>
#include <utils\Log.h>
#include "Map.h"
#include <utils\Log.h>

namespace levels {

	// -------------------------------------------------------
	// load all text levels
	// -------------------------------------------------------
	uint32 loadTextFile(Level* levels) {
		uint32 cnt = 0;
		std::vector<std::string> files;
		std::string line;
		ds::FileUtils::listDirectory("levels",files);
		TileState state;
		for ( size_t i = 0; i < files.size(); ++i ) {
			std::ifstream myfile("levels\\"+files[i]);
			Level* level = &levels[cnt];
			int y = 0;
			bool first = true;
			level->mapDefinition.create(HALF_MAX_X,HALF_MAX_Y);
			if (myfile.is_open()) {
				while ( myfile ) {
					getline (myfile,line);
					if ( first ) {
						sprintf(level->name,"%s",line.c_str());
						LOG << "reading level: " << line;
						first = false;
					}
					else {
						if ( line.length() > 0 ) {
							assert(line.length() == HALF_MAX_X);
							if ( y < HALF_MAX_Y ) {
								for ( int x = 0; x < HALF_MAX_X; ++x ) {
									char c = line[x];
									if ( c == 'x' ) {
										state = TS_AVAILABLE;
									}
									else {
										state = TS_EMPTY;
									}
									Tile t;
									t.state = state;
									level->mapDefinition.set(x,y,t);
								}
								++y;
							}
						}
					}
				}
			}
			++cnt;
		}
		return cnt;
	}

	// -------------------------------------------------------
	// convert level into tilemap
	// -------------------------------------------------------
	void convert(const Level& level,TileMap& map) {
		int clr = 0;
		for ( int x = 0; x < HALF_MAX_X; ++x ) {
			for ( int y = 0; y < HALF_MAX_Y; ++y ) {
				const Tile& type = level.mapDefinition.get(x,y);
				if ( type.state == TS_AVAILABLE ) {
					clr = -1;
				}
				else {
					clr = -2;				
				}
				Tile n;
				n.color = clr;
				n.state = type.state;
				map.set(x * 2,y * 2,n);
				map.set(x * 2 + 1,y * 2,n);
				map.set(x * 2,y * 2 + 1,n);
				map.set(x * 2 + 1,y * 2 + 1,n);			
			}
		}
	}

}

namespace map {
	// -------------------------------------------------------
	// determine corners
	// -------------------------------------------------------
	int determineCorners(const TileMap& definition,int x,int y) {
		int set = 0;
		if ( isDefined(definition,x,y)) {
			if ( isDefined(definition,x-1,y)) {
				set |= 1;
			}
			if ( isDefined(definition,x+1,y)) {
				set |= 2;
			}
			if ( isDefined(definition,x,y-1)) {
				set |= 4;
			}
			if ( isDefined(definition,x,y+1)) {
				set |= 8;
			}
		}
		return set;
	}

	// -------------------------------------------------------
	// is defined
	// -------------------------------------------------------
	bool isDefined(const TileMap& definition,int x,int y) {
		if ( !definition.isValid(x,y)) {
			return false;
		}
		const Tile& t = definition.get(x,y);
		return t.state == TS_FILLED || t.cleared;
	}

	// -------------------------------------------------------
	// debug tilemap
	// -------------------------------------------------------
	void debug(const TileMap& map) {
		for ( int y = map.height - 1; y >= 0; --y ) {	
			std::string txt = "";
			for ( int x = 0; x < map.width; ++x ) {
				std::string str;
				const Tile& t = map.get(x,y);
				switch ( t.state ) {
					case TS_EMPTY : txt += "E";break;
					case TS_AVAILABLE : txt += "A";break;
					case TS_MARKED: txt += "M";break;
					case TS_FILLED : txt += "F";break;
					default: txt += "?";
				}
				txt += " ";
			}
			LOG << txt;
		}
	}

	// -------------------------------------------------------
	// convert mouse pos to grid pos
	// -------------------------------------------------------
	Vector2i convertMousePosition(int mx,int my) {
		int x = ( mx - START_X ) / SQUARE_SIZE;
		int y = ( (768 - my) - START_Y ) / SQUARE_SIZE;
		return Vector2i(x,y);
	}

	// -------------------------------------------------------
	// convert grid coordinates to screen pos
	// -------------------------------------------------------
	Vector2f convert(int gridX,int gridY) {
		return Vector2f(START_X + gridX * SQUARE_SIZE,START_Y + gridY * SQUARE_SIZE);
	}

	// -------------------------------------------------------
	// convert grid pos to screen pos
	// -------------------------------------------------------
	Vector2f convert(const Vector2i& gridPos) {
		return Vector2f(START_X + gridPos.x * SQUARE_SIZE,START_Y + gridPos.y * SQUARE_SIZE);
	}

	// -------------------------------------------------------
	// Render tilemap
	// -------------------------------------------------------
	void render(ds::Renderer* renderer,const TileMap& map) {
		for ( int x = 0; x < map.width; ++x ) {
			for ( int y = 0; y < map.height; ++y) {
				const Tile& t = map.get(x,y);
				Vector2f p = convert(x,y);
				if ( t.state == TS_AVAILABLE ) {
					renderer->draw(p,0,ds::Rect(0,0,36,36));
				}
				else if ( t.state == TS_MARKED ) {
					int offset = t.color * 34;
					renderer->draw(p,0,ds::Rect(72,offset,32,32));
				}
				else if ( t.state == TS_FILLED ) {
					int offset = t.color * 38;
					renderer->draw(p,0,ds::Rect(110,offset,36,36));
				}
			}
		}
		// draw tile borders
		for ( int x = 0; x < map.width; ++x ) {
			for ( int y = 0; y < map.height; ++y) {
				const Tile& t = map.get(x,y);
				if ( t.corners > 0 )  {
					Vector2f p = convert(x,y);
					int offset = t.corners * 36;
					renderer->draw(p,0,ds::Rect(240,offset,36,36));			
				}
			}
		}
	}
}
