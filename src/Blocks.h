#pragma comment(lib, "Diesel2D.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr.lib")
#pragma warning(disable : 4995)

#pragma once
#include <base\BaseApp.h>
#include "game\Map.h"
#include "game\LevelManager.h"
#include "game\Laser.h"
#include "game\Block.h"
#include <io\Serializer.h>
#include <objects\HUD.h>

class Blocks : public ds::BaseApp {

enum GameMode {
	START_MENU,
	RUNNING,
	GAME_OVER,
	CREDITS
};

enum LevelMode {
	LM_COVERAGE,
	LM_TWO_MINUTES,
	LM_THREE_MINUTES,
	LM_FIVE_MINUTES
};

struct MyHUD {
	int score;
	int coverage;
};

public:	
	Blocks();
	virtual ~Blocks() {
	}
	const char* getTitle() {
		return "Blocks";
	}
	//void initialize();
	bool loadContent();	
	virtual void OnChar( char ascii,unsigned int keyState );
	virtual void update( const ds::GameTime& gameTime );	
	void draw(const ds::GameTime& gameTime);
	virtual void OnButtonUp( int button,int x,int y );
	virtual void onGUIButton( ds::DialogID dlgID,int button );
private:
	bool isLevelFinished();	
	void resetGrid();
	void createDialogs(int id);
	void stopGame(bool showDialog = true);

	GameMode m_Mode;
	ds::Sprite m_Background;

	//ds::NewParticlesystemEntity m_BlockExplosion;
	LevelMode m_LevelMode;
	Block m_NextPiece;
	Block m_Block;
	
	Level m_Levels[32];
	uint32 m_LevelCount;
	int m_LevelIndex;

	TileMap m_CurrentMap;
	Map m_Map;
	Laser m_Laser;
	float m_LaserTimer;
	int m_CurrentRow;
	ds::HUD m_HUD;
	MyHUD m_MyHUD;
	uint32 m_AddBS;
	float m_ExpTimer;

	int m_TextureID;

	
};