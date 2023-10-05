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
#ifndef __INPUT_MANAGER_H_
#define __INPUT_MANAGER_H_

#include <vector>

#include <ois/OIS.h>

#include <Ogre.h>
#include <OgreRenderWindow.h>

#include <boost/enable_shared_from_this.hpp>

#include "InputListener.h"

#include "..\Lua\LuaInterface.h"

#define WA_CALL_LISTENERS(method) for ( InputListenersIterator it=mRegisteredListeners.begin() ; it < mRegisteredListeners.end(); it++ ) {(*it)->method;}

namespace Ogre
{
	class RenderWindow;
}

namespace WyvernsAssault
{
	/** The input listenrs list */
	typedef std::vector<InputListener*> InputListenersList;
	typedef InputListenersList::iterator InputListenersIterator;

	/**
		Class used to manager user input, from keyboard, mouse, joystick and so on...
	*/
	class InputManager	: public Ogre::Singleton<InputManager>
						, public boost::enable_shared_from_this<InputManager>
						, public OIS::MouseListener
						, public OIS::KeyListener
						, public OIS::JoyStickListener
						, public LuaInterface
	{
	public:
		InputManager();
		~InputManager();
		static InputManager& getSingleton(void);
		static InputManager* getSingletonPtr(void);

	public:
		/** Initialize the input manager */
		bool initialize( Ogre::RenderWindow* window, bool showDefaultMousePointer );
		/** Finalize the input manager */
		void finalize();
		/** Add input listener */
		void addListener(InputListener* listener);
		/** Remove input listener */
		void removeListener(InputListener* listener);

		/** Acquire all inputs */
		void acquireAll();
		/** Relese all inputs */
		void unacquireAll();
		/** Read all user inputs */
		void capture();

		/** Get Keyboard */
		OIS::Keyboard* getKeyboard();
		/** Get Mouse */
		OIS::Mouse* getMouse();
		/** Get Joystick */
		OIS::JoyStick* getJoyStick();
		
		/** Switches mouse mode (buffered/unbuffered) */
		virtual void switchMouseMode();
		/** Switches keyboard mode (buffered/unbuffered) */
		virtual void switchKeyMode();
		/** Switches joystick mode (buffered/unbuffered) */
		virtual void switchJoyStickMode();

		/** Set mouse mode (true:buffered/false:unbuffered) */
		virtual void setMouseMode(const bool isBuffered);
		/** Set keyboard mode (true:buffered/false:unbuffered) */
		virtual void setKeyMode(const bool isBuffered);
		/** Set joystick mode (true:buffered/false:unbuffered) */
		virtual void setJoyStickMode(const bool isBuffered);

		//
		// Keyboard listeners
		//
		/** Buffered input - keyboard key clicked */
		virtual bool keyClicked(const OIS::KeyEvent& e);
		/** Buffered input - keyboard key clicked */
		virtual bool keyPressed(const OIS::KeyEvent& e);
		/** Buffered input - keyboard key clicked */
		virtual bool keyReleased(const OIS::KeyEvent& e);

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

		//
		// Enable/disable input
		//
		void disable(){mEnabled = false;}
		void enable(){mEnabled = true;}
		bool isEnabled(){return mEnabled;}

	protected:
		OIS::InputManager* mInputManager;
		OIS::Keyboard* mKeyboard;
		OIS::Mouse* mMouse;
		OIS::JoyStick* mJoyStick;
		
		bool mUseBufferedInputMouse;
		bool mUseBufferedInputKeys;
		bool mUseBufferedInputJoyStick;
		bool mInputTypeSwitchingOn;

		InputListenersList mRegisteredListeners;

		bool mInitialized;
		bool mEnabled;

	public:
		// --------------------------------
		// BEGIN Lua Interface Declarations
		// --------------------------------
		public:
			LUA_INTERFACE();

			// Physics Lib (exported to Lua)
			LUA_LIBRARY("Input",inputlib);

			LUA_FUNCTION(enable);
			LUA_FUNCTION(disable);
			LUA_FUNCTION(isEnabled);
		// ------------------------------
		// END Lua Interface Declarations
		// ------------------------------
	};

	typedef boost::shared_ptr<InputManager> InputManagerPtr;
}

#endif // __INPUT_MANAGER_H_