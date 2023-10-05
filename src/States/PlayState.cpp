#include "..\include\States\PlayState.h"

using namespace WyvernsAssault;

PlayState::PlayState(GraphicsManagerPtr graphicsManager, 
					 InputManagerPtr inputManager, 
					 AudioManagerPtr audioManager, 
					 CameraManagerPtr cameraManager, 
					 GuiManagerPtr guiManager, 
					 LevelManagerPtr levelManager)
: BaseState(graphicsManager,inputManager,audioManager, cameraManager, guiManager, levelManager)
, mTrayMgr(NULL)
, mDetailsPanel(NULL)
, mPerformancesPanel(NULL)
, mRootSceneNode(0)
, mElapsedSeconds(0.0f)
, mDebugEnabled(false)
{
	//
	// TODO Constructor logic HERE
	//
	mLevel = levelManager->getCurrentLevelIndex();
}

PlayState::~PlayState()
{
	//
	// TODO Distructor logic HERE
	//
	finalize();
}

/** Initialize current state */
void PlayState::initialize()
{
	BaseState::initialize();

	mLevel = mLevelManager->getCurrentLevelIndex();

	// Set near clipping plane
	if(mLevel == 0 )
		mCameraManager->setNearClipping(50);
	else
		mCameraManager->setNearClipping(150);	

	mDebugEnabled = false;

	//
	// Use Game camera and viewport (by default Gui ones are used!)
	//
	mCamera = mCameraManager->getGameCamera();
	mViewport = mCameraManager->getGameViewport();

	mCameraManager->startCameras();

	mInputManager->setKeyMode(true);
	mInputManager->setMouseMode(true);
	mInputManager->setJoyStickMode(true);

	this->mNextGameStateId = this->getStateId();

	// Add Playstate as frame listener
	mRoot->addFrameListener(this);

	//
	// Particle Manager
	//
	mParticleManager = ParticleManagerPtr(new ParticleManager(mSceneManager));
	mParticleManager->initialize();

	//
	// Post Processing manager
	//
	mPostProcessManager = PostProcessManagerPtr(new PostProcessManager(mViewport));
	mPostProcessManager->initialize();
	
	//
	// Gui for this state
	//
	mGuiScreen = mGuiManager->createScreen(GuiScreenId::PlayGui, "PlayScreen");
	mPlayerUI = GuiUserInterfacePtr( new GuiUserInterface(mWindow->getViewport(0)->getCamera()->getAspectRatio(), GuiScreenId::PlayGui, GuiWidgetPlayId::UserInterface1) );
	mGuiScreen->addWidget(mPlayerUI,GuiWidgetPlayId::UserInterface1);

	GuiForegroundPtr foreground = GuiForegroundPtr(new GuiForeground());
	mGuiScreen->setForeground(foreground);

	GuiFramePtr frame = GuiFramePtr(new GuiFrame());
	mGuiScreen->setFrame(frame);

	// Player manager constructor
	mPlayerManager = PlayerManagerPtr(new PlayerManager(mSceneManager));
	mPlayerManager->initialize();

	// Create a single player (TEST!)
	PlayerPtr player1 = mPlayerManager->createPlayer("Player1","redWyvern.mesh");
	// Add particle systems
	player1->setFireBreath(mParticleManager->create("WyvernsAssault/DragonBreath"));
	player1->setDust(mParticleManager->create("WyvernsAssault/Dust"));
	// Set player gui
	player1->setGuiId(GuiWidgetPlayId::UserInterface1);

	// Lights manager constructor
	mLightsManager = LightsManagerPtr(new LightsManager(mSceneManager));
	mLightsManager->initialize();

	//
	// Physics Manager
	// 
	mPhysicsManager = PhysicsManagerPtr(new PhysicsManager(mSceneManager));
	mPhysicsManager->initialize();
	mPhysicsManager->addPhysicPlayer(player1);	// NEED FIX!

	//Enemies manager constructor
	mEnemyManager = EnemyManagerPtr(new EnemyManager(mSceneManager));
	mEnemyManager->initialize();

	// SdkTrays Manager
	mTrayMgr = new OgreBites::SdkTrayManager("DebugInterface", mWindow, mInputManager->getMouse());
	if(mTrayMgr->isCursorVisible()) mTrayMgr->hideCursor();
	
	// Create a params panel for displaying sample details
	StringVector items;
	items.push_back("Player X");
	items.push_back("Player Y");
	items.push_back("Player Z");
	items.push_back("");
	items.push_back("Mode");
	items.push_back("Camera X");
	items.push_back("Camera Y");
	items.push_back("Camera Z");
	items.push_back("Look At X");
	items.push_back("Look At Y");
	items.push_back("Look At Z");
	items.push_back("");	
	items.push_back("Filtering");
	items.push_back("Poly Mode");
	items.push_back("");	
	items.push_back("Game Area");
	items.push_back("Enemies");
	items.push_back("Finish Time");
	items.push_back("Time");
 
	mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
	mDetailsPanel->setParamValue(12, "Bilinear");
	mDetailsPanel->setParamValue(13, "Solid");
	mDetailsPanel->hide();

	// Create another panel to display FPS and performances related stuff
	StringVector fpsItems;
	fpsItems.push_back("Loop Time");
	fpsItems.push_back("Real FPS");
	fpsItems.push_back("");
	fpsItems.push_back("Update   %");
	fpsItems.push_back("CutScene %");
	fpsItems.push_back("Player   %");
	fpsItems.push_back("Physics  %");
	fpsItems.push_back("Logic    %");
	fpsItems.push_back("Enemy    %");
	fpsItems.push_back("Lua      %");
	fpsItems.push_back("Lights   %");
	fpsItems.push_back("Item     %");
	fpsItems.push_back("Camera   %");
	fpsItems.push_back("Audio    %");
	fpsItems.push_back("Scenario %");
	fpsItems.push_back("PostProc %");
	fpsItems.push_back("Events   %");
	fpsItems.push_back("GameArea %");
	fpsItems.push_back("Projectile %");
	fpsItems.push_back("Particles %");
	fpsItems.push_back("GUI		 %");

 
	mPerformancesPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "PerformancesPanel", 200, fpsItems);
	mPerformancesPanel->hide();

	//
	// Item Manager
	//
	mItemManager = ItemManagerPtr(new ItemManager(mSceneManager));
	mItemManager->initialize();

	//
	// ProjectileManager 
	//
	mProjectileManager = ProjectileManagerPtr(new ProjectileManager(mSceneManager));

	//
	// Logic Manager
	//
	mLogicManager = LogicManagerPtr(new LogicManager());
	mLogicManager->initialize();

	//
	// Scenario Manager
	//
	mScenarioManager = ScenarioManagerPtr(new ScenarioManager(mSceneManager));
	mScenarioManager->initialize();

	//
	// Cut Scene manager
	//
	mCutSceneManager = CutSceneManagerPtr(new CutSceneManager(mSceneManager));
	mCutSceneManager->initialize();
	//mCutSceneManager->play(CutSceneManager::CutSceneId::Intro);

	//
	// Game Area manager
	//
	mGameAreaManager = GameAreaManagerPtr(new GameAreaManager());
	mGameAreaManager->initialize();

	//
	// Weather Manager
	//
	mWeatherManager = WeatherManagerPtr(new WeatherManager());
	mWeatherManager->initialize();

	//
	// Events Manager 
	//
	mEventsManager = EventsManagerPtr(new EventsManager());
	
	//
	// Register all events listeners/callers
	//
	mEventsManager->registerInterface(mEnemyManager);
	mEventsManager->registerInterface(mPhysicsManager);
	mEventsManager->registerInterface(mParticleManager);
	mEventsManager->registerInterface(mAudioManager);
	mEventsManager->registerInterface(mItemManager);
	mEventsManager->registerInterface(mPlayerManager);
	mEventsManager->registerInterface(mCameraManager);
	mEventsManager->registerInterface(mGuiManager);
	mEventsManager->registerInterface(mPostProcessManager);
	mEventsManager->registerInterface(mCutSceneManager);
	mEventsManager->registerInterface(mScenarioManager);
	mEventsManager->registerInterface(mGameAreaManager);
	mEventsManager->registerInterface(mProjectileManager);
	mEventsManager->registerInterface(mLightsManager);
	mEventsManager->registerInterface(mWeatherManager);
	mEventsManager->registerInterface(mLevelManager);

	// -----------
	// Lua Manager
	// -----------
	// Create this manager after ALL other managers. Never alter
	// the oreder of the operations!
	//
	// FIRST : Instatniate the Lua Manager
	mLuaManager = LuaManagerPtr(new LuaManager());

	//
	// SECOND : Register all the LuaInterfaces you want. 
	//
	mLuaManager->registerInterface(mLightsManager);
	mLuaManager->registerInterface(mLogicManager);
	mLuaManager->registerInterface(mEnemyManager);
	mLuaManager->registerInterface(mPhysicsManager);
	mLuaManager->registerInterface(mPlayerManager);
	mLuaManager->registerInterface(mCutSceneManager);
	mLuaManager->registerInterface(mAudioManager);
	mLuaManager->registerInterface(mGuiManager);
	mLuaManager->registerInterface(mPostProcessManager);
	mLuaManager->registerInterface(mCameraManager);
	mLuaManager->registerInterface(mInputManager);
	mLuaManager->registerInterface(mWeatherManager);
	mLuaManager->registerInterface(mEnemyManager);
	mLuaManager->registerInterface(mItemManager);
	mLuaManager->registerInterface(mGameAreaManager);

	//
	// THIRD :	This call to 'initialize' will initialize Lua,
	//			open default Lua libs, add custom libs exported by
	//			our registered LuaInterfaces, setup the environment.
	//
	mLuaManager->initialize();

	//
	// Load scene!
	//
	boost::scoped_ptr<DotSceneLoader> dotSceneLoader ( new DotSceneLoader );
	// Game areas FIRST!
	dotSceneLoader->parseDotScene(mLevelManager->getCurrentLevel()->getGameAreasFile(),"General", mSceneManager, mPlayerManager, mScenarioManager, mCameraManager, mLightsManager, mEnemyManager, mPhysicsManager, mItemManager, mParticleManager, mGameAreaManager, mCameraManager->_getSceneNode());
	// Scenario, enemies and items
	dotSceneLoader->parseDotScene(mLevelManager->getCurrentLevel()->getSceneFile(),"General", mSceneManager, mPlayerManager, mScenarioManager, mCameraManager, mLightsManager, mEnemyManager, mPhysicsManager, mItemManager, mParticleManager, mGameAreaManager, mScenarioManager->_getSceneNode());
	// Fixed cameras and camera segments
	dotSceneLoader->parseDotScene(mLevelManager->getCurrentLevel()->getCamerasFile(),"General", mSceneManager, mPlayerManager, mScenarioManager, mCameraManager, mLightsManager, mEnemyManager, mPhysicsManager, mItemManager, mParticleManager, mGameAreaManager, mCameraManager->_getSceneNode());

	Debug::Out(mSceneManager->getRootSceneNode());

	//
	// Set game camera
	//
	//mCameraManager->gameCamera(); // Lua will do this from the cut scene

	Viewport* gameViewport = mCameraManager->getGameViewport();
	//
	// Audio: playState track
	//
	//mAudioManager->playSoundTrack("Level1_1.mp3"); // Lua will do this from the cut scene

	buttonTimer = 0.0;
	lastKey = OIS::KeyCode::KC_UNASSIGNED;
}

/** Load resources */
void PlayState::load()
{
	//
	// TODO: Load?
	//
}

/** Manage input */
void PlayState::input()
{
	//
	// TODO Read input
	//
	mInputManager->capture();
}

/** Rendering queue */
bool PlayState::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	mTrayMgr->frameRenderingQueued(evt);
	if (!mTrayMgr->isDialogVisible())
	{
		if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
		{
			PlayerPtr player1 = mPlayerManager->getPlayer(PLAYER1);
			mDetailsPanel->setParamValue(0, StringConverter::toString(player1->getPosition().x, 5));
			mDetailsPanel->setParamValue(1, StringConverter::toString(player1->getPosition().y, 5));
			mDetailsPanel->setParamValue(2, StringConverter::toString(player1->getPosition().z, 5));
			mDetailsPanel->setParamValue(4, mCameraManager->getCameraModeString().c_str());
			mDetailsPanel->setParamValue(5, StringConverter::toString(mCameraManager->getCameraPosition().x, 5));
			mDetailsPanel->setParamValue(6, StringConverter::toString(mCameraManager->getCameraPosition().y, 5));
			mDetailsPanel->setParamValue(7, StringConverter::toString(mCameraManager->getCameraPosition().z, 5));
			mDetailsPanel->setParamValue(8, StringConverter::toString(mCameraManager->getCameraLookAt().x, 5));
			mDetailsPanel->setParamValue(9, StringConverter::toString(mCameraManager->getCameraLookAt().y, 5));
			mDetailsPanel->setParamValue(10, StringConverter::toString(mCameraManager->getCameraLookAt().z, 5));
			mDetailsPanel->setParamValue(15, StringConverter::toString(mGameAreaManager->getGameArea()));
			mDetailsPanel->setParamValue(16, StringConverter::toString(mEnemyManager->getCount()));
			mDetailsPanel->setParamValue(17, StringConverter::toString(mGameAreaManager->getFinishTime()));
			mDetailsPanel->setParamValue(18, StringConverter::toString(mGameAreaManager->getTime()));
		}

		if(mPerformancesPanel->isVisible())
		{
			mPerformancesPanel->setParamValue(0,StringConverter::toString(mElapsedSeconds));
			mPerformancesPanel->setParamValue(1,StringConverter::toString(1.0f/ (mElapsedSeconds > 0 ? mElapsedSeconds : 0.0001f)));
			

			mPerformancesPanel->setParamValue(3,StringConverter::toString(TIMER_PERCENT(Update,Update))); // Update
			mPerformancesPanel->setParamValue(4,StringConverter::toString(TIMER_PERCENT(CutScene,Update))); // CutScene
			mPerformancesPanel->setParamValue(5,StringConverter::toString(TIMER_PERCENT(Player,Update))); // Player
			mPerformancesPanel->setParamValue(6,StringConverter::toString(TIMER_PERCENT(Physics,Update))); // Physics
			mPerformancesPanel->setParamValue(7,StringConverter::toString(TIMER_PERCENT(Logic,Update))); // Logic
			mPerformancesPanel->setParamValue(8,StringConverter::toString(TIMER_PERCENT(Enemy,Update))); // Enemy
			mPerformancesPanel->setParamValue(9,StringConverter::toString(TIMER_PERCENT(Lua,Update))); // Lua
			mPerformancesPanel->setParamValue(10,StringConverter::toString(TIMER_PERCENT(Lights,Update))); // Lights
			mPerformancesPanel->setParamValue(11,StringConverter::toString(TIMER_PERCENT(Item,Update))); // Item
			mPerformancesPanel->setParamValue(12,StringConverter::toString(TIMER_PERCENT(Camera,Update))); // Camera
			mPerformancesPanel->setParamValue(13,StringConverter::toString(TIMER_PERCENT(Audio,Update))); // Audio
			mPerformancesPanel->setParamValue(14,StringConverter::toString(TIMER_PERCENT(Scenario,Update))); // Scenario
			mPerformancesPanel->setParamValue(15,StringConverter::toString(TIMER_PERCENT(PostProcess,Update))); // PostProcess
			mPerformancesPanel->setParamValue(16,StringConverter::toString(TIMER_PERCENT(Events,Update))); // Events
			mPerformancesPanel->setParamValue(17,StringConverter::toString(TIMER_PERCENT(GameArea,Update))); // GameArea
			mPerformancesPanel->setParamValue(18,StringConverter::toString(TIMER_PERCENT(Projectile,Update))); // Projectile
			mPerformancesPanel->setParamValue(19,StringConverter::toString(TIMER_PERCENT(ParticleSystem,Update))); // Particle
			mPerformancesPanel->setParamValue(20,StringConverter::toString(TIMER_PERCENT(GUI,Update))); // GUI
		}
	}

	if(mCameraManager->getCameraMode() == CameraManager::CameraModes::Free)
	{
		mCameraManager->frameRenderingQueued(evt);
	}

	return true;
}

/** Update internal stuff */
void PlayState::update(const float elapsedSeconds)
{
	TIMER_START(Update);

	mElapsedSeconds = elapsedSeconds;

	//
	// Update CutScene Manager
	//
#ifndef SKIP_CUT_SCENES
	TIMER_START(CutScene);
	mCutSceneManager->update(elapsedSeconds);
	TIMER_STOP(CutScene);
#endif

	//
	// Player update
	//
	TIMER_START(Player);

	PlayerPtr player1 = mPlayerManager->getPlayer(PLAYER1);

	if ( !player1->isDeath() )
	{
		// Movement

		// Trick for key hit inaccuracies
		buttonTimer += elapsedSeconds;
		if ( buttonTimer >= 0.2)
		{
			buttonTimer = 0;
			lastKey = OIS::KeyCode::KC_UNASSIGNED;
		}

		// Special Attack
		if ( this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LCONTROL) && lastKey != OIS::KeyCode::KC_LCONTROL )
		{
			mPlayerManager->attackSpecial(PLAYER1);
			lastKey = OIS::KeyCode::KC_LCONTROL;		
		}
		// Main attack
		else if( this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_SPACE) && lastKey != OIS::KeyCode::KC_SPACE )
		{
			mPlayerManager->attack(PLAYER1);
			lastKey = OIS::KeyCode::KC_SPACE;				
		}

		// Movement if not attacking
		if( !player1->isAttacking() && !player1->isSpecial() )
		{
			// 8 directions move
			if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_RIGHT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
			{
				mPlayerManager->move(PLAYER1,mCameraManager->getDirection(UP_RIGHT_DIRECTION));
				lastKey = OIS::KeyCode::KC_UP;
			}
			else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_RIGHT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
			{
				mPlayerManager->move(PLAYER1,mCameraManager->getDirection(DOWN_RIGHT_DIRECTION));
				lastKey = OIS::KeyCode::KC_DOWN;
			}
			else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LEFT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
			{
				mPlayerManager->move(PLAYER1,mCameraManager->getDirection(UP_LEFT_DIRECTION));
				lastKey = OIS::KeyCode::KC_UP;
			}
			else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LEFT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
			{
				mPlayerManager->move(PLAYER1,mCameraManager->getDirection(DOWN_LEFT_DIRECTION));
				lastKey = OIS::KeyCode::KC_DOWN;
			}
			else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_RIGHT))
			{
				mPlayerManager->move(PLAYER1,mCameraManager->getDirection(RIGHT_DIRECTION));
				lastKey = OIS::KeyCode::KC_RIGHT;
			}
			else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LEFT))
			{
				mPlayerManager->move(PLAYER1,mCameraManager->getDirection(LEFT_DIRECTION));
				lastKey = OIS::KeyCode::KC_LEFT;
			}
			else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
			{
				mPlayerManager->move(PLAYER1,mCameraManager->getDirection(UP_DIRECTION));
				lastKey = OIS::KeyCode::KC_UP;
			}
			else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
			{
				mPlayerManager->move(PLAYER1,mCameraManager->getDirection(DOWN_DIRECTION));
				lastKey = OIS::KeyCode::KC_DOWN;
			}
			else
			{
				// Iddle
				mPlayerManager->move(PLAYER1,Vector3::ZERO);				
			}
		}
		else
		{
			// No movement
			mPlayerManager->stop(PLAYER1);
		}
	}
	else
	{
		mPlayerManager->stop(PLAYER1);

		if( !player1->isDying() )
			this->mNextGameStateId = GameStateId::GameOver;
	}

	//
	// Update animation state
	//
	mPlayerManager->update(elapsedSeconds);

	TIMER_STOP(Player);

	/** Game Updates **/

	//
	//Update Physic
	//
	TIMER_START(Physics);
	mPhysicsManager->update(elapsedSeconds);
	TIMER_STOP(Physics);

	//
	// Logic manager
	//
	TIMER_START(Logic);
	mLogicManager->update(elapsedSeconds);
	TIMER_STOP(Logic);

	//
	// LUA manager
	// 
	TIMER_START(Lua);
	mLuaManager->update(elapsedSeconds);
	TIMER_STOP(Lua);

	//
	// Lights manager
	//
	TIMER_START(Lights);
	mLightsManager->update(elapsedSeconds);
	TIMER_STOP(Lights);

	//
	// Projectile manager
	//
	TIMER_START(Projectile);
	mProjectileManager->update(elapsedSeconds);
	TIMER_STOP(Projectile);

	//
	// Enemy manager
	//
	TIMER_START(Enemy);
	mEnemyManager->update(elapsedSeconds);
	TIMER_STOP(Enemy);

	//
	// Item Manager
	//
	TIMER_START(Item);
	mItemManager->update(elapsedSeconds);
	TIMER_STOP(Item);

	//
	// Game Area Manager
	//
	TIMER_START(GameArea);
	mGameAreaManager->update(player1->getPosition(), elapsedSeconds);
	TIMER_STOP(GameArea);

	//
	// Update camera
	//
	TIMER_START(Camera);
	mCameraManager->updateCamera(player1->getPosition(), mGameAreaManager->getGameArea(), mLevelManager->getCurrentLevelIndex(), elapsedSeconds);
	TIMER_STOP(Camera);

	//
	// Update sounds
	//
	TIMER_START(Audio);
	mAudioManager->update(Vector3::ZERO, Quaternion::ZERO, elapsedSeconds);
	TIMER_STOP(Audio);

	//
	// Update scenario objects
	//
	TIMER_START(Scenario);
	mScenarioManager->update(elapsedSeconds);
	TIMER_STOP(Scenario);

	//
	// Update post processing compositors
	//
	TIMER_START(PostProcess);
	mPostProcessManager->update(elapsedSeconds);
	TIMER_STOP(PostProcess);
	
	//
	// Update particle systems
	//
	TIMER_START(ParticleSystem);
	mParticleManager->update(elapsedSeconds);
	TIMER_STOP(ParticleSystem);

	//
	// Update Level manager
	//
	TIMER_START(Level);
	mLevelManager->update(elapsedSeconds);
	TIMER_STOP(Level);

	//
	// Weather Manager
	//
	TIMER_START(Weather);
	mWeatherManager->update(elapsedSeconds);
	TIMER_STOP(Weather);

	//
	// Update GUI Manager
	//
	TIMER_START(GUI);	
	mGuiManager->update(elapsedSeconds, mEnemyManager->getCount());		
	TIMER_STOP(GUI);

	//
	// Dispatch events. Managers have probably raised events, that are now in the 
	// EventsManager queue. The events manager will then dispatch them, calling
	// for each of them the registered handler(s).
	//
	TIMER_START(Events);
	mEventsManager->update(elapsedSeconds);
	TIMER_STOP(Events);

	//
	// Check for level/game ending
	// 
	checkLevelEnd();

	TIMER_STOP(Update);
}

/** Render */
void PlayState::render(const float elapsedSeconds)
{
	//
	// TODO
	//
}

/** Unload resources */
void PlayState::unload() 
{
	//
	// Audio manager
	//
	mAudioManager->playSoundTrack("hard_track.mp3");

	if(mGuiScreen)
	{
		//
		// Remove gui
		//
		mGuiManager->removeScreen(GuiScreenId::PlayGui);
	}
	if(mTrayMgr)
	{
		//
		// Remove all sdktrays
		//
		mTrayMgr->hideAll();
		mTrayMgr->clearAllTrays();
	}
	if(mDebugEnabled)
	{
		mCameraManager->hideAxes();
	}
	if(mRoot)
	{
		//
		// Remove frame listener
		//
		mRoot->removeFrameListener(this);
	}
}

/** Destroy the state */
void PlayState::finalize()
{	
	////// FIRST!
	mEventsManager.reset();

	mParticleManager.reset();

	mScenarioManager.reset();

	mLightsManager.reset();

	mLuaManager.reset();

	mPlayerManager.reset();

	mEnemyManager.reset();

	if(mTrayMgr)
	{
		delete mTrayMgr;
		mTrayMgr = NULL;
	}

	mGuiScreen.reset();

	mGuiManager->reset();

	mPhysicsManager.reset();

	mItemManager.reset();

	mLogicManager.reset();

	mPostProcessManager.reset();

	mCutSceneManager.reset();

	mGameAreaManager.reset();

	mProjectileManager.reset();

	mWeatherManager.reset();

	//mGraphicsManager->clearScene();

	BaseState::finalize();

	Debug::Out("**************************");
}

/** Get state Id */
GameStateId PlayState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::Play;
}

/** Called when the state has to be paused */
void PlayState::pause()
{
	//
	// Hide gui without removing it
	//
	mGuiScreen->hide();

	mGuiManager->hide();

	// Hide sdktrays if visible
	mTrayMgr->hideAll();
	if(mTrayMgr->isCursorVisible()) 
		mTrayMgr->hideCursor();

	// Hide axes if visible
	if(mDebugEnabled)
		mCameraManager->hideAxes();

	// Disable all post process
	mPostProcessManager->pause();

	// Pause all particles

}

/** Called when the state has to be resumed (from pause) */
void PlayState::resume()
{
	//
	// Set next state to this state (Play)	
	//
	this->mNextGameStateId = this->getStateId();
	//
	// Show gui (hided when pause)
	//
	mGuiScreen->show(); 

	mGuiManager->show();

	// Show sdktrays if was visible
	mTrayMgr->showAll();
	if(mTrayMgr->isCursorVisible())
		mTrayMgr->hideCursor();

	// Show axes if visible
	if(mDebugEnabled)
		mCameraManager->showAxes();

	// Resume previous camera
	mCameraManager->resumeCamera(mLevelManager->getCurrentLevelIndex());

	// Resume all post process previous state
	mPostProcessManager->resume();
}

/** Buffered input - keyboard key clicked */
bool PlayState::keyPressed(const OIS::KeyEvent& e)
{
	String newVal;
	PolygonMode pm;
	TextureFilterOptions tfo;
	unsigned int aniso;
	PlayerPtr player1 = mPlayerManager->getPlayer(PLAYER1);

	switch(e.key)
	{
	case OIS::KeyCode::KC_B:
		this->mPlayerManager->toggleGodMode();
		this->mPlayerUI->setGodMode(this->mPlayerManager->getPlayer(PLAYER1)->isGodModeOn());
		break;
	// Player Die
	case OIS::KeyCode::KC_O:
		player1->die();		
		break;
	// Pause menu
	case OIS::KeyCode::KC_P:		
		this->mNextGameStateId = GameStateId::Pause;
		break;
	// Camera keys
	case OIS::KeyCode::KC_1:		
		mCameraManager->gameCamera();
		mGuiScreen->show();
		mGuiManager->show();
		break;
	case OIS::KeyCode::KC_2:
		mCameraManager->freeCamera();
		mGuiScreen->hide();
		mGuiManager->hide();
		break;
	case OIS::KeyCode::KC_3:
		mCameraManager->fixedCamera(0, mLevelManager->getCurrentLevelIndex());
		break;
	case OIS::KeyCode::KC_4:
		mCameraManager->fixedCamera(1, mLevelManager->getCurrentLevelIndex());
		break;
	case OIS::KeyCode::KC_5:
		mCameraManager->fixedCamera(2, mLevelManager->getCurrentLevelIndex());
		break;
	case OIS::KeyCode::KC_6:
		mCameraManager->fixedCamera(3, mLevelManager->getCurrentLevelIndex());
		break;
	case OIS::KeyCode::KC_7:
		mCameraManager->fixedCamera(4, mLevelManager->getCurrentLevelIndex());
		break;
	case OIS::KeyCode::KC_8:
		mCameraManager->fixedCamera(5, mLevelManager->getCurrentLevelIndex());
		break;
	// Toogle visibility of debugging details
	case OIS::KeyCode::KC_G:		
		if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
		{
			mDebugEnabled = true;

			mTrayMgr->moveWidgetToTray(mPerformancesPanel, OgreBites::TL_TOPLEFT, 0);
			mPerformancesPanel->show();
			mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
			mDetailsPanel->show();
			mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
			mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
			mCameraManager->showAxes();

			mEnemyManager->setDebugEnabled(true);
			mPlayerManager->setDebugEnabled(true);
			mPhysicsManager->setDebugEnabled(true);
			mItemManager->setDebugEnabled(true);
			mScenarioManager->setDebugEnabled(true);
		}
		else
		{
			mDebugEnabled = false;

			mTrayMgr->removeWidgetFromTray(mPerformancesPanel);
			mPerformancesPanel->hide();
			mTrayMgr->removeWidgetFromTray(mDetailsPanel);
			mDetailsPanel->hide();
			mTrayMgr->hideFrameStats();
			mTrayMgr->hideLogo();
			mCameraManager->hideAxes();

			mEnemyManager->setDebugEnabled(false);
			mPlayerManager->setDebugEnabled(false);
			mPhysicsManager->setDebugEnabled(false);
			mItemManager->setDebugEnabled(false);
			mScenarioManager->setDebugEnabled(false);
		}		
		break;
	// Cycle filtering mode
	case OIS::KeyCode::KC_T:
		switch (mDetailsPanel->getParamValue(12).asUTF8()[0])
		{
		case 'B':
			newVal = "Trilinear";
			tfo = Ogre::TFO_TRILINEAR;
			aniso = 1;
			break;
		case 'T':
			newVal = "Anisotropic";
			tfo = Ogre::TFO_ANISOTROPIC;
			aniso = 8;
			break;
		case 'A':
			newVal = "None";
			tfo = Ogre::TFO_NONE;
			aniso = 1;
			break;
		default:
			newVal = "Bilinear";
			tfo = Ogre::TFO_BILINEAR;
			aniso = 1;
		} 
		MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
		MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
		mDetailsPanel->setParamValue(12, newVal);	
		break;
	// Cycle polygon rendering mode
	case OIS::KeyCode::KC_R: 
		switch (mCameraManager->getPolygonMode())
		{
		case Ogre::PM_SOLID:
			newVal = "Wireframe";
			pm = Ogre::PM_WIREFRAME;
			break;
		case Ogre::PM_WIREFRAME:
			newVal = "Points";
			pm = Ogre::PM_POINTS;
			break;
		default:
			newVal = "Solid";
			pm = Ogre::PM_SOLID;
		} 
		mCameraManager->setPolygonMode(pm);
		mDetailsPanel->setParamValue(13, newVal);
		break;
	// Reload all textures
	case OIS::KeyCode::KC_F11:
		TextureManager::getSingleton().reloadAll();
		break;
	// Reload all Lua scripts
	case OIS::KeyCode::KC_F12:
		mLuaManager->reload();
		break;
	// Toggle Lua manager active state
	case OIS::KeyCode::KC_U:
		if(mLuaManager->isEnabled())
			mLuaManager->disable();
		else
			mLuaManager->enable();
		break;
	// Depth of Field
	case OIS::KeyCode::KC_K:
		mPostProcessManager->depthOfField();
		break;
	// Radial Blur
	case OIS::KeyCode::KC_L:
		mPostProcessManager->radialBlur(2);
		break;
	// Normals On/Off
	case OIS::KeyCode::KC_HOME:
		mPostProcessManager->showNormal();
		break;
	// Depth On/Off
	case OIS::KeyCode::KC_END:
		mPostProcessManager->showDepth();
		break;
	// Skip cutscene
	case OIS::KeyCode::KC_ESCAPE:
		mCutSceneManager->skip();
		break;
	// Move Player to a game area -- Teleport HACK!
	case OIS::KeyCode::KC_F1:
		player1->setPosition(Vector3(798, 23, 712)); // Go to previous wheat		
		break;
	case OIS::KeyCode::KC_F2:
		player1->setPosition(Vector3(1024, 23, 547)); // Go to the wheat		
		break;
	case OIS::KeyCode::KC_F3:
		player1->setPosition(Vector3(1228, 23, 282)); // Go to the village
		break;
	case OIS::KeyCode::KC_F4:
		player1->setPosition(Vector3(1177, 40, -25)); // Go to the bridge
		break;
	case OIS::KeyCode::KC_F5:
		player1->setPosition(Vector3(527, 23, -533)); // Go to the wooden wall
		break;
	case OIS::KeyCode::KC_F6:
		player1->setPosition(Vector3(-420, 44, -167)); // Go to the castle
		break;
	case OIS::KeyCode::KC_F7:
		player1->setPosition(Vector3(-605, 143, 38)); // Go to the final portal
		break;
	// Kill all enemies in current game area -- HACK!
	case OIS::KeyCode::KC_RSHIFT:
		mEnemyManager->killAllEnemies(player1);
		break;
	// Switch to next level -- HACK!
	case OIS::KeyCode::KC_N:
		//
		// Pretend the level has ended, jump to next one
		// 
		bool wasLastLevel = mLevelManager->next();

		//
		// If that was the last level, game has ended...
		//
		if(!wasLastLevel)
		{
			this->mNextGameStateId = GameStateId::LevelLoading;
		}
		else
		{
			this->mNextGameStateId = GameStateId::Ending;
		}		
		break;
	}

	// Free camera mode move
	if(mCameraManager->getCameraMode() == CameraManager::CameraModes::Free)
	{
		if( e.key != OIS::KC_UP && e.key != OIS::KC_DOWN &&	e.key != OIS::KC_RIGHT && e.key != OIS::KC_LEFT && e.key != OIS::KC_SPACE && e.key != OIS::KC_LCONTROL )
			mCameraManager->freeCameraKeyboardDown(e);
	}

	return true;
}

bool PlayState::keyReleased(const OIS::KeyEvent& e)
{
	// Free camera mode move
	if(mCameraManager->getCameraMode() == CameraManager::CameraModes::Free)
	{
		if( e.key != OIS::KC_UP && e.key != OIS::KC_DOWN &&	e.key != OIS::KC_RIGHT && e.key != OIS::KC_LEFT && e.key != OIS::KC_SPACE && e.key != OIS::KC_LCONTROL )
			mCameraManager->freeCameraKeyboardUp(e);
	}

	return true;
}

/** Mouse input */
bool PlayState::mouseMoved(const OIS::MouseEvent& e)
{
	mCameraManager->freeCameraMouse(e);
	return true;
}

void PlayState::checkLevelEnd()
{
	int newLevel = mLevelManager->getCurrentLevelIndex();
	if(mLevel != newLevel)
	{
		if(mLevelManager->isLast())
		{
			mNextGameStateId = GameStateId::Ending;
			// HACK : we do it here, but it should be automatic
			mLevelManager->change(0);
			mLevel = 0;
		}
		else
		{
			mNextGameStateId = GameStateId::LevelLoading;
			mLevel = newLevel;
		}
	}
}