#include "..\include\States\EndingState.h"

using namespace WyvernsAssault;

EndingState::EndingState(GraphicsManagerPtr graphicsManager, 
						 InputManagerPtr inputManager, 
						 AudioManagerPtr audioManager, 
						 CameraManagerPtr cameraManager, 
						 GuiManagerPtr guiManager,
						 LevelManagerPtr levelManager)
: BaseState(graphicsManager,inputManager,audioManager, cameraManager, guiManager, levelManager)
{
	//
	// TODO Constructor logic HERE
	//
}

EndingState::~EndingState()
{
	//
	// TODO Distructor logic HERE
	//
	exit();
}

/** Initialize current state */
void EndingState::initialize()
{
	BaseState::initialize();
}

/** Load resources */
void EndingState::load()
{
	//
	// Gui Screen for this state
	//
	mGuiScreen = mGuiManager->createScreen(GuiScreenId::EndingGui, "EndingScreen");
	
	GuiBackgroundPtr guiBackground = GuiBackgroundPtr(new GuiBackground());

	// Common aspect ratio is 4/3
	if( mWindow->getViewport(0)->getCamera()->getAspectRatio() > 1.34 )
		guiBackground->setImage("wide/GameEnd.png","EndingBackground","General");
	else
		guiBackground->setImage("normal/GameEnd.png","EndingBackground","General");

	mGuiScreen->setBackground(guiBackground);

	// Gui Widgets for this state
	mMenu = new GuiMenu(mWindow->getViewport(0)->getCamera()->getAspectRatio(), GuiScreenId::EndingGui);
	
	// Add menu to screen
	mGuiScreen->addMenu(mMenu);
	
	//
	// Audio manager
	//
	mAudioManager->playSoundTrack("GameEnd.mp3");
}

/** Manage input */
void EndingState::input()
{
	//
	// TODO Read input
	//
	this->mInputManager->capture();
}

/** Update internal stuff */
void EndingState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
}

/** Render */
void EndingState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void EndingState::unload() 
{
	if(mGuiScreen)
	{
		//
		// Remove gui
		//
		mGuiManager->removeScreen(GuiScreenId::EndingGui);
		mGuiScreen.reset();
	}
}

/** Destroy the state */
void EndingState::finalize()
{
	BaseState::finalize();
}

/** Get state Id */
GameStateId EndingState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::Ending;
}

/** Called when the state has to be paused */
void EndingState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void EndingState::resume()
{
	//
	// TODO : Resume state
	//
}

/** Buffered input - keyboard key clicked */
bool EndingState::keyReleased(const OIS::KeyEvent& e)
{
	switch(e.key)
	{
	case OIS::KC_LEFT:
		mMenu->previousOption();
		mAudioManager->playSound("ChangeOption.wav", -2);
		break;
	case OIS::KC_RIGHT:
		mMenu->nextOption();
		mAudioManager->playSound("ChangeOption.wav", -2);
		break;
	case OIS::KC_RETURN:
		switch(mMenu->getCurrentOption())
		{
		case GuiWidgetEndingId::EndingToMenu:
			this->mNextGameStateId = GameStateId::SplashScreen;
			mAudioManager->playSound("Select.wav", -1);
			break;
		case GuiWidgetEndingId::EndingToCredits:
			this->mNextGameStateId = GameStateId::Credits;
			mAudioManager->playSound("Select.wav", -1);
			break;
		}		
		break;
	}

	return true;
}