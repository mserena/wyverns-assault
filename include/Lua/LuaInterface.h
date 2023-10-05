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
#ifndef __LUA_INTERFACE_H__
#define __LUA_INTERFACE_H__

#include <Ogre.h>

#include "LuaDefines.h"

namespace WyvernsAssault
{
	/**
	Interface to export Lua scripting methods
	*/
	class LuaInterface
	{
	public:
		virtual const char* luaGetLibraryName() const = 0;
		virtual const struct luaL_reg* luaGetLibrary() = 0;
		virtual void luaLoadScripts() = 0;
		
		virtual void luaFinalize(){};
		virtual void luaReload(){};
		virtual bool luaLoadScript(const char* name);
		virtual void luaInitialize(lua_State* L){this->L = L;}
		virtual void luaEnable(){mLuaEnabled = true;}
		virtual void luaDisable(){mLuaEnabled = false;}

	protected:
		/* the Lua interpreter */
		lua_State* L;

		bool mLuaEnabled;
	};

	typedef boost::shared_ptr<LuaInterface> LuaInterfacePtr;
}

#endif // __LUA_INTERFACE_H__