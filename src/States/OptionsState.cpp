#include "..\include\States\OptionsState.h"

using namespace WyvernsAssault;

OptionsState::OptionsState(GraphicsManagerPtr graphicsManager, 
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

OptionsState::~OptionsState()
{
	//
	// TODO Distructor logic HERE
	//
	finalize();
}

/** Initialize current state */
void OptionsState::initialize()
{
	BaseState::initialize();
}

/** Manage input */
void OptionsState::input()
{
	//
	// TODO Read input
	//
	this->mInputManager->capture();
}

/** Load resources */
void OptionsState::load()
{
	//
	// Gui Screen for this state
	//
	mGuiScreen = mGuiManager->createScreen(GuiScreenId::OptionsGui, "OptionsScreen");
	
	GuiBackgroundPtr guiBackground = GuiBackgroundPtr(new GuiBackground());
	
	// Common aspect ratio is 4/3
	if( mWindow->getViewport(0)->getCamera()->getAspectRatio() > 1.34 )
		guiBackground->setImage("wide/Options.png","OptionsBackground","General");
	else
		guiBackground->setImage("normal/Options.png","OptionsBackground","General");

	mGuiScreen->setBackground(guiBackground);

	// Gui Widgets for this state
	mMenu = new GuiMenu(mWindow->getViewport(0)->getCamera()->getAspectRatio(), GuiScreenId::OptionsGui);
	
	// Add menu to screen
	mGuiScreen->addMenu(mMenu);
}

/** Update internal stuff */
void OptionsState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
}

/** Render */
void OptionsState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void OptionsState::unload() 
{
	if(mGuiScreen)
	{
		//
		// Remove gui
		//
		mGuiManager->removeScreen(GuiScreenId::OptionsGui);
		mGuiScreen.reset();
	}
}

/** Destroy the state */
void OptionsState::finalize()
{
	BaseState::finalize();
}

/** Get state Id */
GameStateId OptionsState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::Options;
}

/** Called when the state has to be paused */
void OptionsState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void OptionsState::resume()
{
	//
	// TODO : Resume state
	//
}

/** Buffered input - keyboard key clicked */
bool OptionsState::keyReleased(const OIS::KeyEvent& e)
{
	switch(e.key)
	{
	case OIS::KC_RETURN:
		// We can reach the Options state from different states. So we just tell the manager to 
		// switch back to previous one. NOTE: we should always enter this state via POP! 
		this->mNextGameStateId = GameStateId::Previous;
		mAudioManager->playSound("Select.wav", -1);
		break;
	}

	return true;
}