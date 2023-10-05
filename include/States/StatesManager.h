/*
-----------------------------------------------------------------------------
Wyverns' Assault 3D Videgame.
Copyright (C) 2010  Giorgio Tino, Javier Soto Huesa, Jordi Carreras Ribot, 
					Marc Serena, Elm Oliver Torres

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

-----------------------------------------------------------------------------
*/
#ifndef __GAME_STATES_MANAGER_H_
#define __GAME_STATES_MANAGER_H_

#include <Ogre.h>

#include <boost/enable_shared_from_this.hpp>

#include "OISEvents.h"
#include "OISInputManager.h"
#include "OISMouse.h"
#include "OISKeyboard.h"
#include "OISJoyStick.h"

#include <map>

//
// Game States
//
#include "BaseState.h"
#include "SplashScreenState.h"
#include "IntroState.h"
#include "EndingState.h"
#include "GameoverState.h"
#include "MainMenuState.h"
#include "OutroState.h"
#include "PlayState.h"
#include "SplashScreenState.h"
#include "CreditsState.h"
#include "OptionsState.h"
#include "PauseState.h"
#include "LevelLoadingState.h"
#include "SelectionState.h"

//
// Audio
//
#include "..\Graphics\GraphicsManager.h"

//
// User input listener
//
#include "..\Input\InputManager.h"
#include "..\Input\InputListener.h"

//
// Audio
//
#include "..\Audio\AudioManager.h"

//
// GUI
//
#include "..\GUI\GuiManager.h"

//
// Level
//
#include "..\Level\LevelManager.h"

using namespace std;

namespace WyvernsAssault
{
	/** The games states map type */
	typedef std::map<GameStateId, BaseState*> StatesMap;
	typedef StatesMap::iterator StatesMapIterator;

	/** The games states stack type */
	typedef std::vector<BaseState*> StatesStack;
	typedef StatesStack::iterator StatesStackIterator;

	/** Game options */

	/** Game levels */

	/** Game score */

	/**
		Class used to manage the game states
	*/
	class StatesManager : public InputListener
						, public Ogre::Singleton<StatesManager>
						, public boost::enable_shared_from_this<StatesManager>
	{
	public:
		StatesManager();
		~StatesManager();
		static StatesManager& getSingleton(void);
		static StatesManager* getSingletonPtr(void);

	public:
		/** Initialize the manager */
		void initialize(GraphicsManagerPtr graphicsManager, InputManagerPtr gameInputManager, AudioManagerPtr audioManager, GuiManagerPtr guiManager, CameraManagerPtr cameraManager, LevelManagerPtr levelManager);
		/** Destroy and release all resources used by the manager */
		void finalize();
		/** Run the current state (main loop)*/
		bool loop(const float elapsedSeconds);

		//
		// Keyboard listeners
		//
		/** Buffered input - keyboard key clicked */
		bool keyClicked(const OIS::KeyEvent& e);
		/** Buffered input - keyboard key clicked */
		bool keyPressed(const OIS::KeyEvent& e);
		/** Buffered input - keyboard key clicked */
		bool keyReleased(const OIS::KeyEvent& e);

		//
		// MouseListener
		//
		/** Buffered input - mouse moved */
		bool mouseMoved(const OIS::MouseEvent &evt);
		/** Buffered input - mouse button pressed */
		bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID);
		/** Buffered input - mouse button released */
		bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID);
	
		//
		// JoyStickListener
		//
		/** Buffered input - joystick button pressed */
		bool buttonPressed(const OIS::JoyStickEvent &evt, int index);
		/** Buffered input - joystick button released */
		bool buttonReleased(const OIS::JoyStickEvent &evt, int index);
		/** Buffered input - axis pad moved */
		bool axisMoved(const OIS::JoyStickEvent &evt, int index);
		/** Buffered input - pov moved */
		bool povMoved(const OIS::JoyStickEvent &evt, int index);

	private:
		/** Change state */
		void changeState(BaseState* state);
		/** Push a state on the top of the stack */
		void pushState(BaseState* state);
		/** Pop a state from the top of the stack */
		void popState();
		/** Retrive a game state reference by its ID */
		BaseState* getGameStateById(const GameStateId id);

	private:
		/** Map of game states [ID - State]*/ 
		StatesMap mStates;
		/** States hitory stack (for push-pop info) */
		StatesStack mStatesStack;

		/** Current (active) game state */
		BaseState* mCurrentState;
		/** Id of the current (active) game state */
		GameStateId mCurrentStateId;

		/** Graphics Manager */
		GraphicsManagerPtr mGraphicsManager;
		/** Input Manager */
		InputManagerPtr mInputManager;
		/** Audio Manager */
		AudioManagerPtr mAudioManager;
		/** Gui Manager */
		GuiManagerPtr mGuiManager;
		/** Camera Manager */
		CameraManagerPtr mCameraManager;
		/** Level Manager */
		LevelManagerPtr mLevelManager;

		bool mInitialized;
	};

	typedef boost::shared_ptr<StatesManager> StatesManagerPtr;
}

#endif // __GAME_STATES_MANAGER_H_