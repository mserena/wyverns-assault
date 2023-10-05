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
#ifndef __SOUND_H__
#define __SOUND_H__

#include <Ogre.h>
#include "..\..\externals\Fmod\include\fmod.hpp"

using namespace Ogre;

namespace WyvernsAssault
{
	typedef enum
	{
		SOUND_TYPE_INVALID = 0,
		SOUND_TYPE_3D_SOUND,
		SOUND_TYPE_3D_SOUND_LOOPED,
		SOUND_TYPE_2D_SOUND,
		SOUND_TYPE_2D_SOUND_LOOPED,
	} SOUND_TYPE;

	enum SoundTypes
	{
		Intro01  = 0,
		Intro02  = 1,
		Menu01	 = 2,
		Attack01 = 3,
		Attack02 = 4,
		Attack03 = 5
	};

	class SoundInstance
	{
	public:
		SoundInstance(){};
		virtual ~SoundInstance(){};
		
		void           clear(void);

	public:		
		String         fileName;
		Archive*       fileArchive;
		DataStreamPtr  streamPtr;
		SOUND_TYPE     soundType;
		FMOD::Sound*   fmodSound;

	public:
		static SoundTypes StringToType(Ogre::String typeStr);
	};

	typedef boost::shared_ptr<SoundInstance> SoundInstancePtr;
}

#endif // __SOUND_H__