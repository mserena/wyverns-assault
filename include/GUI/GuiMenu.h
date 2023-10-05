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
#ifndef __GUI_MENU_H_
#define __GUI_MENU_H_

#include <Ogre.h>
#include <OgreRenderWindow.h>

#include "OgreTextAreaOverlayElement.h"
#include "GuiWidget.h"

using namespace Ogre;

namespace WyvernsAssault
{
	class GuiMenu
	{
	public:
		GuiMenu(Real aspectRatio, int screenId);
		~GuiMenu();

		void initialize(Real aspectRatio);
		void finalize();

	public:
		/** Show/Hide functions */
		void show();
		void hide();

		/** Destroy menu function*/
		void destroyMenu();
	
	public:
		/** Selecting options */
		void nextOption();
		void previousOption();
		/** Select current option */
		GuiWidgetId getCurrentOption();

	protected:		
		Overlay*			mOverlay;
		OverlayContainer*	mMenu;
		OverlayElement*		mCurrent;

	protected:
		int					mScreenId;
		GuiWidgetId			mSelectedOption;
	};
}

#endif // __GUI_MENU_H_