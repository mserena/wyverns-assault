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
#ifndef __CAMERA_MANAGER_H_
#define __CAMERA_MANAGER_H_

#define GUI_CAMERA "GuiCamera"
#define GAME_CAMERA "GameCamera"

#define CAMERA_NODE_NAME "CameraNode"

#include <Ogre.h>
#include <OgreSingleton.h>
#include <ois/OIS.h>
#include <boost/enable_shared_from_this.hpp>

#include "..\Utils\Utils.h"
#include "..\Lua\LuaInterface.h"
#include "..\Events\EventsInterface.h"

#include "SdkCameraMan.h"

using namespace Ogre;

namespace WyvernsAssault
{
	// Fixed cameras struct, list, and map
	struct FixedCamera
	{			
		int mId;
		Ogre::Vector3 mPosition;
		Ogre::Vector3 mLookAt;
	};

	typedef std::vector<FixedCamera> FixedCameraList;
	typedef std::map<int, FixedCameraList> FixedCameraMapList;

	// Game camera segments struct, list, and map
	struct CameraSegment
	{
		Ogre::Vector3	mPositionBegin;
		Ogre::Vector3	mLookAtBegin;
		
		Ogre::Vector3	mPositionEnd;
		Ogre::Vector3	mLookAtEnd;
	};

	typedef std::vector<CameraSegment> CameraSegmentList;
	typedef std::map<int, CameraSegmentList> CameraSegmentMapList;	

	/**
		Class used to manage differents cameras
	*/
	class CameraManager : public Ogre::Singleton<CameraManager>
						, public boost::enable_shared_from_this<CameraManager>
						, public LuaInterface
						, public EventsInterface
	{
	public:
		/** List of camera modes */
		enum CameraModes
		{
			Game = 0,
			Fixed = 1,
			Free = 2,
			CutScene = 3
		};

	public:
		CameraManager();
		~CameraManager();
		static CameraManager& getSingleton(void);
		static CameraManager* getSingletonPtr(void);

		/** Initialize the camera manager */
		void initialize(SceneManager* sceneManager, RenderWindow* window);
		/** Finalize the camera manager */
		void finalize();

		void setNearClipping(int distance){ mGameCamera->setNearClipDistance(distance); }

		/** Start functions **/ 
		void startCameras();

		/** Camera functions */
		Vector3 getCameraPosition();
		Vector3 getCameraDirection(){ return mGameCamera->getDirection(); }
		Vector3 getCameraLookAt();
		CameraModes getCameraMode() { return mGameCameraMode ; }
		String getCameraModeString();

		Camera* getGameCamera(){return mGameCamera;}

		Viewport* getGameViewport(){return mGameViewport;}
		
		/** Camera moving direction */
		Vector3 getDirection(Vector3 direction);
		
		/** Update camera position in game mode */
		void updateCamera(Vector3 player, int gameArea, int gameLevel, Real elapsedSeconds);
		
		/** Create transition animation */
		void createTransition(Vector3 begin, Vector3 end, Vector3 lbegin, Vector3 lend, float duration);

		/** Add game area  and camera segment */
		void addCameraSegment(int level, Vector3 pbegin, Vector3 pend, Vector3 lbegin, Vector3 lend);

		/** Fixed cameras functions */
		void setFixedCamera(int level, int camera, Vector3 position, Vector3 lookAt);
		
		/** Camera effects */
		void zoom(Real duration, Real amount);
		void rumble(Real duration, Real amount);
		void tremor(Real duration, Real amount);
		void shake(Real duration, Real amount);
		void drunk(Real duration, Real amount);

		/** Camera modes */		
		void gameCamera();
		void freeCamera();
		void fixedCamera(int camera, int level);
		void cutSceneCamera();
		void resumeCamera(int level);

		/** Free camera functions */
		void freeCameraMouse(OIS::MouseEvent evt);
		void freeCameraKeyboardDown(OIS::KeyEvent evt);
		void freeCameraKeyboardUp(OIS::KeyEvent evt);

		/** Cut Scenes camera function **/
		void translate(Ogre::Vector3 position, Ogre::Vector3 lookAt)
		{ 
			createTransition(getCameraPosition(), (getCameraPosition() + position), getCameraLookAt(), (getCameraLookAt() + lookAt), 0.01f); 
		}
		void moveTo(Ogre::Vector3 position, Ogre::Vector3 lookAt)
		{ 
			//mGameCameraNode->setPosition(position);
			//mGameCameraNode->lookAt(lookAt,Ogre::Node::TransformSpace::TS_WORLD);
			createTransition(getCameraPosition(), position, getCameraLookAt(), lookAt, 0.01f); 
		}
		bool isMoving(){ return mMoving; }

		/**  Rendering queue */
		bool frameRenderingQueued(FrameEvent evt);		

		/** Debug camera functions */
		void setPolygonMode(PolygonMode pm){ mGameCamera->setPolygonMode(pm);}
		PolygonMode getPolygonMode(){ return mGameCamera->getPolygonMode(); }
		void showAxes();
		void hideAxes();

		Ogre::SceneNode* _getSceneNode() const { return mCameraNode; }

	protected:
		SceneManager*			mSceneManager;
		RenderWindow*			mWindow;
		Viewport*				mGameViewport;

	protected:
		SceneNode*				mCameraNode;

		Camera*					mGameCamera;
		SceneNode*				mGameCameraNode;
		SceneNode*				mGameCameraLookAtNode;

		CameraModes				mGameCameraMode;

		Entity*					mAxes;
		SceneNode*				mAxesNode;

		AnimationState*			mCameraTransition;
		AnimationState*			mLookAtTransition;
		AnimationState*			mAxesTransition;
		AnimationState*			mCameraEffect;
		AnimationState*			mCameraEffectLook;

		bool					mMoving;

		int						mCurrentGameArea;

		// Sdk Camera Manager for free look camera
		OgreBites::SdkCameraMan*		mCameraMan;

	private:	

		int						mFixedCamera;
		FixedCameraMapList		mFixedCameras;
		CameraSegmentMapList	mCameraSegments;

		// ----------------
		// Events interface
		// ----------------
		EVENTS_INTERFACE()

		EVENTS_HANDLER(PlayerAttack)
		EVENTS_HANDLER(EnemyKilled)
		EVENTS_HANDLER(ItemCatch)
		EVENTS_HANDLER(SpecialEffect)

		// --------------------------------
		// BEGIN Lua Interface Declarations
		// --------------------------------
	public:
		LUA_INTERFACE();

		//Camera Lib (exported to Lua)
		LUA_LIBRARY("Camera",cameralib);

		LUA_FUNCTION(setCurrent);
		LUA_FUNCTION(getCurrent);
		LUA_FUNCTION(translate);
		LUA_FUNCTION(moveTo);
		LUA_FUNCTION(flyTo);
		LUA_FUNCTION(hasArrived);
		LUA_FUNCTION(strife);
		LUA_FUNCTION(shake);
		LUA_FUNCTION(rumble);
		LUA_FUNCTION(tremor);
		LUA_FUNCTION(zoom);

		// ------------------------------
		// END Lua Interface Declarations
		// ------------------------------
	};

	typedef boost::shared_ptr<CameraManager> CameraManagerPtr;
}

#endif // __CAMERA_MANAGER_H_