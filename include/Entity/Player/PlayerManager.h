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
#ifndef __PLAYER_MANAGER_H__
#define __PLAYER_MANAGER_H__

#define PLAYER_NODE_NAME "PlayerNode"

#include <Ogre.h>
#include <OgreSingleton.h>

#include <boost/enable_shared_from_this.hpp>

#include "..\..\Debug\Debug.h"

#include "..\..\Utils\Utils.h"
#include "..\..\..\include\Events\EventsInterface.h"
#include "..\..\..\include\Lua\LuaInterface.h"
#include "..\..\..\include\Entity\EntityManager.h"

#include "Player.h"

using namespace Ogre;

namespace WyvernsAssault
{
	typedef std::map<Ogre::String,PlayerPtr> PlayerMap;
	typedef std::map<Ogre::String,PlayerPtr>::iterator PlayerMapIterator;

	typedef std::vector<PlayerPtr> PlayerList;
	typedef std::vector<PlayerPtr>::iterator PlayerListIterator;

	/**
	Class used to manage all the enemies
	*/
	class PlayerManager : public Ogre::Singleton<PlayerManager>, public boost::enable_shared_from_this<PlayerManager>, public EntityManager, public LuaInterface, public EventsInterface
	{
	public:
		PlayerManager(Ogre::SceneManager* sceneManager);
		~PlayerManager();
		static PlayerManager& getSingleton(void);
		static PlayerManager* getSingletonPtr(void);

		void initialize();
		void finalize();
		/** Update and run lua scripts, animations... */
		void update(const float elapsedSeconds);

		PlayerPtr createPlayer(Ogre::String name, Ogre::String mesh);

		PlayerPtr getPlayer(Ogre::String name);
		bool removePlayer(Ogre::String name);

		void move(Ogre::String playerName, Vector3 direction);
		void attack(Ogre::String name);
		void attackSpecial(Ogre::String name);
		void stop(Ogre::String playerName);

		// Enable Debug Stuff
		void setDebugEnabled(bool isDebugEnabled);
		bool getDebugEnabled(){return mIsDebugEnabled;};
		void toggleGodMode();

		Ogre::SceneNode* _getSceneNode() const { return mPlayerNode; }

		//
		// Enable/disable the manager
		//
		void disable(){mEnabled = false;}
		void enable(){mEnabled = true;}
		bool isEnabled(){return mEnabled;}

		// ----------------
		// Events interface
		// ----------------
		EVENTS_INTERFACE()

		EVENTS_HANDLER(PlayerHit)
		EVENTS_HANDLER(PlayerKilled)
		EVENTS_HANDLER(ItemCatch)
		EVENTS_HANDLER(EnemyKilled)
		EVENTS_HANDLER(ObjectKilled)
		EVENTS_HANDLER(ProjectileHit)
		EVENTS_HANDLER(GameAreaChanged)
		EVENTS_HANDLER(GameAreaCleared)

	private:
		PlayerList getPlayerList(){ return mPlayerList; }

	private:
		SceneManager *mSceneManager;
		SceneNode *mPlayerNode;

		PlayerList mPlayerList;
		PlayerMap mPlayerMap;

		bool mIsDebugEnabled;

		float mRefillTimer;
		bool mEnabled;

	// --------------------------------
	// BEGIN Lua Interface Declarations
	// --------------------------------
	public:
		LUA_INTERFACE();

		// Player Lib (exported to Lua)
		LUA_LIBRARY("Player",playerlib);

		LUA_FUNCTION(getPlayerPosition)
		LUA_FUNCTION(getNumPlayers)
		LUA_FUNCTION(enable);
		LUA_FUNCTION(disable);
		LUA_FUNCTION(isEnabled);
	// ------------------------------
	// END Lua Interface Declarations
	// ------------------------------
	};

	typedef boost::shared_ptr<PlayerManager> PlayerManagerPtr;
}

#endif // __PLAYER_MANAGER_H__