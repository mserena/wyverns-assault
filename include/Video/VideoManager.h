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
#ifndef __VIDEO_MANAGER_H__
#define __VIDEO_MANAGER_H__

#include <Ogre.h>
#include <OgreSingleton.h>
#include <boost/enable_shared_from_this.hpp>

#include "TheoraVideoManager.h"
#include "TheoraVideoClip.h"

#include "FmodAudioInterface.h"

#include "..\Utils\Utils.h"
#include "..\Lua\LuaInterface.h"

#define VIDEO_FILE "intro.ogv"
#define VIDEO_NODE_NAME "VideoNode"

using namespace Ogre;

namespace WyvernsAssault 
{
	/**
	Class used to manage all the lights
	*/
	class VideoManager	: public Ogre::Singleton<VideoManager>
						, public boost::enable_shared_from_this<VideoManager>
						, public LuaInterface
	{
	public:
		VideoManager(SceneManager* sceneManager);
		~VideoManager();
		static VideoManager& getSingleton(void);
		static VideoManager* getSingletonPtr(void);

	public:
		/** Initialize the lights manager */
		void initialize();
		/** Finalize the lights manager */
		void finalize();
		/** Update and run lua scripts */
		void update(const float elapsedSeconds);

		Ogre::SceneNode* _getSceneNode() const { return mVideoNode; }

		void play();
		void stop();
		bool isDone();

	private:
		void createQuad(String name,String material_name,float left,float top,float right,float bottom);

	private:
		SceneManager* mSceneManager;
		SceneNode* mVideoNode;

		TheoraVideoManager* mTheoraVideoManager;
		TheoraVideoClip* mTheoraVideoClip;

		FmodAudioInterfaceFactory* mFmodAudioInterfaceFactory;

		bool mInitialized;

	// --------------------------------
	// BEGIN Lua Interface Declarations
	// --------------------------------
	public:
		LUA_INTERFACE();

		// Export library lib as "Video" to be called from Lua scripts
		LUA_LIBRARY("Video",videolib)

		// From Lua to C++
		LUA_FUNCTION(play)
		LUA_FUNCTION(stop)
	// ------------------------------
	// END Lua Interface Declarations
	// ------------------------------
	};

	typedef boost::shared_ptr<VideoManager> VideoManagerPtr;
}

#endif // __VIDEO_MANAGER_H__