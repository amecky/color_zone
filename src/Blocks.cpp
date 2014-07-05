#include "Blocks.h"
#include <io\FileWatcher.h>
#include <renderer\BitmapFont.h>

ds::BaseApp *app = new Blocks(); 

const ds::Color HUD_YELLOW = ds::Color(0.9f,0.7f,0.0f,1.0f);
const ds::Color HUD_BLUE = ds::Color(0.0f,0.39f,0.9f,1.0f);

Blocks::Blocks() : ds::BaseApp() {
	//_CrtSetBreakAlloc(12876);
	m_Width = 1024;
	m_Height = 768;
	m_ClearColor = ds::Color(0.0f,0.0f,0.0f,1.0f);		
	m_CurrentMap.create(MAX_X,MAX_Y);
}

// -------------------------------------------------------
// Load content
// -------------------------------------------------------
bool Blocks::loadContent() {	
	m_TextureID = renderer->loadTexture("Textures");
	assert( m_TextureID != -1 );

	ds::BitmapFont* font = renderer->createBitmapFont("xscale");
	assets.load("xscale",font,ds::CVT_FONT);
	renderer->initializeBitmapFont(*font,m_TextureID);
	//initializeGUI();

	createGameObject<ds::HUD>(&m_HUD);
	m_HUD.init(0,"xscale");
	assets.load("hud",&m_HUD,ds::CVT_HUD);
	
	int id = 0;
	assets.load("background",&m_Background,ds::CVT_SPRITE);
	createGameObject<Laser>(&m_Laser);
	createGameObject<Block>(&m_Block);
	createGameObject<Block>(&m_NextPiece);
	createGameObject<Map>(&m_Map);

	m_NextPiece.updatePosition(Vector2f(630,700),false);
	m_AddBS = renderer->createBlendState(ds::BL_ONE,ds::BL_ONE,true);	

	audio->loadSound("NoMove");

	//m_World.addNewParticleSystemEntity(3,0,"block_explosion",&m_BlockExplosion,1024,m_AddBS);

	m_LevelIndex = 0;
	m_LevelCount = levels::loadTextFile(m_Levels);


	resetGrid();
	m_CurrentRow = -1;
	//stopGame(false);
	//m_Mode = START_MENU;
	m_Mode = RUNNING;
	resetGrid();
	//gui.activate("MainMenu");
	m_ExpTimer = 0.0f;
	return true;
}

// -------------------------------------------------------
// Stop running game
// -------------------------------------------------------
void Blocks::stopGame(bool showDialog) {
	m_Map.clear();
	/*
	m_HUD.stopTimer(1);
	m_HUD.setActive(false);
	m_Laser.reset();
	m_Block.setActive(false);
	m_NextPiece.setActive(false);
	if ( showDialog ) {
		std::string str;
		ds::string::formatInt(m_MyHUD.score,str,6);
		m_GameOverDlg.updateText(22,str);
		ds::string::formatInt(m_MyHUD.coverage,str);
		str += "%";
		m_GameOverDlg.updateText(20,str);		
		ds::GameTimer* timer = m_HUD.getTimer(1);
		ds::string::formatTime(timer->getMinutes(),timer->getSeconds(),str);
		m_GameOverDlg.updateText(21,str);		
		m_GameOverDlg.activate();
	}
	*/
	m_Mode = GAME_OVER;
}

// -------------------------------------------------------
// Reset grid
// -------------------------------------------------------
void Blocks::resetGrid() {

	LOG << "Resetting grid using level index " << m_LevelIndex;
	Level& level = m_Levels[m_LevelIndex];
	m_Map.reset(level,&m_Block);
	m_HUD.setCounterValue(1,0);
	m_HUD.setText(4,"0%");
	// FIXME: center level name
	m_HUD.setText(5,level.name);
	m_HUD.setTimer(1,0,0);
	m_HUD.setActive(true);
	m_HUD.startTimer(1);
	m_MyHUD.score = 0;
	m_MyHUD.coverage = 0;
	m_NextPiece.pickColors();
	m_Block.pickColors();
	m_LaserTimer = 5.0f;
	m_Laser.reset();
	/*
	m_NextPiece.setActive(true);
	m_Block.setActive(true);
	m_Laser.reset();
	m_LevelMode = LM_COVERAGE;
	*/
	m_Mode = RUNNING;
}

void Blocks::OnChar( char ascii,unsigned int keyState ) {
	if ( ascii == 's' ) {
		m_Laser.start();
	}
	if ( ascii == 'e' ) {
		stopGame();
	}
	if ( ascii == 'd' ) {
		m_Map.debug();
	}
	if ( ascii == '1' ) {
		m_LevelIndex = 0;
		resetGrid();
	}
	if ( ascii == '2' ) {
		m_LevelIndex = 1;
		resetGrid();
	}
	if ( ascii == '3' ) {
		m_LevelIndex = 2;
		resetGrid();
	}
	if ( ascii == '4' ) {
		m_LevelIndex = 3;
		resetGrid();
	}
	if ( ascii == '5' ) {
		m_LevelIndex = 4;
		resetGrid();
	}
	if ( ascii == '6' ) {
		m_LevelIndex = 5;
		resetGrid();
	}
	if ( ascii == 'r' ) {
		//m_BlockExplosion.start(Vector2f(512,384));
	}

	if ( ascii == '+' ) {
		++m_LevelIndex;
		if ( m_LevelIndex > m_LevelCount ) {
			m_LevelIndex = 0;
		}
		resetGrid();
	}
	if ( ascii == '-' ) {
		--m_LevelIndex;
		if ( m_LevelIndex < 0 ) {
			m_LevelIndex = m_LevelCount - 1;
		}
		resetGrid();
	}
}

// -------------------------------------------------------
// Draw
// -------------------------------------------------------
void Blocks::draw(const ds::GameTime& gameTime) {
	renderer->draw(m_Background);	
	if ( m_Mode == RUNNING ) {
		m_Map.render();
		renderer->setTexture(m_TextureID);
		world.render();
		m_HUD.render();		
	}
}

// -------------------------------------------------------
// Update
// -------------------------------------------------------
void Blocks::update( const ds::GameTime& gameTime ) {
	if ( m_Mode == RUNNING ) {
		Vector2f mp = getMousePos();
		m_Block.setPosition(mp.x,mp.y);
		if ( m_Laser.isRunning() ) {
			int row = m_Laser.getRow();
			if ( row >= 0 && m_CurrentRow != row ) {
				m_CurrentRow = row;
				int p = m_Map.cleanRow(row);
				m_MyHUD.score += p * p * 10;
				int coverage = m_Map.getCoverage();
				LOG << "coverage " << coverage;
				m_HUD.setCounterValue(1,m_MyHUD.score);
				
				if ( m_MyHUD.coverage != coverage ) {
					m_MyHUD.coverage = coverage;
					std::string txt;
					ds::string::formatInt(m_MyHUD.coverage,txt);
					txt += "%";
					m_HUD.setText(4,txt);
				}
			}
		}
		/*
		else {
			m_LaserTimer -= gameTime.elapsed;
			if ( m_LaserTimer <= 0.0f ) {
				m_Laser.start();
				m_LaserTimer = 2.0f;
			}
		}
		*/
		// check if we are done
		if ( isLevelFinished() ) {
			LOG << "we are done";
			stopGame();
			m_Mode = GAME_OVER;
		}
	}

	// DEBUG
	/*
	m_ExpTimer += gameTime.elapsed;
	if ( m_ExpTimer > 2.0f ) {
		m_ExpTimer = 0.0f;
		m_BlockExplosion.start(Vector2f(512,384));
	}
	*/
}

// -------------------------------------------------------
// Check if level is done
// -------------------------------------------------------
bool Blocks::isLevelFinished() {
	/*
	if ( m_LevelMode == LM_COVERAGE ) {
		if ( m_MyHUD.coverage >= 80 ) {
			return true;
		}
	}
	else if ( m_LevelMode == LM_TWO_MINUTES ) {
		ds::GameTimer* timer = m_HUD.getTimer(1);
		if ( timer->getTimeInMillis() > 120000 ) {
			return true;
		}
	}
	*/
	return false;
}

// -------------------------------------------------------
// On button up
// -------------------------------------------------------
void Blocks::OnButtonUp( int button,int x,int y ) {
	LOG << "button pressed " << button;
	if ( m_Mode == RUNNING ) {
		if ( button == 0 ) {
			// FIXME: check if allowed!
			if ( m_Block.convert(&m_Map) ) {
				m_Block.copyColors(&m_NextPiece);
				m_NextPiece.pickColors();		
			}
			else {
				// play warning sound
			}
		}
		if ( button == 1 ) {
			m_Block.rotateColors();
		}
	}
}

// -------------------------------------------------------
// On GUI button
// -------------------------------------------------------
void Blocks::onGUIButton( ds::DialogID dlgID,int button ) {
	LOG << "dialog: " << dlgID << " button: " << button;
	/*
	if ( dlgID == m_GameOverDlg.getDialogID() && button == 1 ) {
		m_GameOverDlg.deactivate();
		m_StartDlg.activate();
	}
	if ( dlgID == m_CreditsDlg.getDialogID() ) {
		m_CreditsDlg.deactivate();
		m_StartDlg.activate();
	}
	if ( dlgID == m_HighscoreDlg.getDialogID() ) {
		m_HighscoreDlg.deactivate();
		m_StartDlg.activate();
	}
	if ( dlgID == m_StartDlg.getDialogID() && button == 2 ) {
		m_HighscoreDlg.activate();
		m_StartDlg.deactivate();
	}
	if ( dlgID == m_StartDlg.getDialogID() && button == 3 ) {
		m_CreditsDlg.activate();
		m_StartDlg.deactivate();
	}
	*/
	if ( dlgID == 1 && button == 1 ) {
		shutdown();
	}
	if ( dlgID == 1 && button == 0 ) {
		gui.deactivate("MainMenu");
		resetGrid();
	}

}


