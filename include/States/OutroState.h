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
#ifndef __OUTRO_STATE_H_
#define __OUTRO_STATE_H_

#include "BaseState.h"

namespace WyvernsAssault
{
	/**
		Outro game state class
	*/
	class OutroState : public BaseState
	{
	public:
		OutroState(	GraphicsManagerPtr graphicsManager, 
					InputManagerPtr inputManager, 
					AudioManagerPtr audioManager, 
					CameraManagerPtr cameraManager, 
					GuiManagerPtr guiManager,
					LevelManagerPtr levelManager);
		~OutroState();

	public:
		/** Initialize current state */
		void initialize();
		/** Load resources */
		void load();
		/** Manage input - INPUT */
		void input();
		/** Update internal stuff - PROCESS */
		void update(const float elapsedSeconds);
		/** Render - OUTPUT */
		void render(const float elapsedSeconds);
		/** Unload resources */
		void unload();
		/** Destroy the state */
		void finalize();
		/** Called when the state has to be paused */
		void pause();
		/** Called when the state has to be resumed (from pause) */
		void resume();

		/** Get state Id */
		GameStateId getStateId();
	};
}
#endif // __OUTRO_STATE_H_