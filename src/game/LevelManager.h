#pragma once
#include "..\Common.h"
#include "..\..\..\math\Vector.h"
#include <renderer\Renderer.h>

struct Level {
	int index;
	char name[32];
	TileMap mapDefinition;
};

namespace levels {

	uint32 loadTextFile(Level* levels);

	void convert(const Level& level,TileMap& map);

}

namespace map {

	Vector2i convertMousePosition(int mx,int my);

	Vector2f convert(int gridX,int gridY);

	Vector2f convert(const Vector2i& gridPos);

	void debug(const TileMap& map);

	bool isDefined(const TileMap& definition,int x,int y);

	int determineCorners(const TileMap& definition,int x,int y);

	void render(ds::Renderer* renderer,const TileMap& map);
}
