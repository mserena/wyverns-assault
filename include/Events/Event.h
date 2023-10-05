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
#ifndef __EVENT_H_
#define __EVENT_H_

#include <boost/shared_ptr.hpp>

#include "..\Entity\Enemy\Enemy.h"
#include "..\Entity\Player\Player.h"
#include "..\Entity\Item\Item.h"
#include "..\Entity\Projectile\Projectile.h"
#include "..\Scenario\Object.h"
#include "..\Weather\Weather.h"
#include "..\CutScene\CutScene.h"

namespace WyvernsAssault
{
	/** Event types */
	enum EventTypes
	{
		PlayerHit = 0,
		PlayerKilled,
		PlayerRemove,
		PlayerAttack,
		PlayerAttackSpecial,
		PlayerStatusUpdate,
		EnemyAttack,
		EnemyHit,
		EnemyKilled,
		EnemyDead,
		EnemyRemove,
		EnemyCustom,
		EnemyCreation,
		EnemyPhysics,
		EnemyCreateItem,
		ObjectHit,
		ObjectKilled,
		ObjectRemove,
		ObjectCustom,
		Collision,
		ItemCreation,
		ItemCatch,
		ItemRemove,
		ItemPhysics,
		GameAreaChanged,
		GameAreaCleared,
		GameAreaEnemiesDeath,
		ProjectileFire,
		ProjectileUpdate,
		ProjectileHit,
		ProjectileRemove,
		LevelComplete,
		WeatherChanged,
		GameAreaFlashCounter,
		SpecialEffect,
		CutSceneStart,
		CutSceneEnd
	};

	/** Event priority, used to put the event in the correct queue */
	enum EventPriorities
	{
		Highest = 0,
		High,
		Normal,
		Low,
		Lower
	};

	typedef float EventTimer;
	typedef void* EventData;

	class Event
	{
	public:
		Event(EventTypes type, EventPriorities priority);
		virtual ~Event() = 0;

	public:
		/** Returns the event type */
		EventTypes getType();
		/** Returns the event priority */
		EventPriorities getPriority();
		/** Returens the event data */
		EventData getData();
		/** Sets the event data */
		void setData(EventData data);

		// Set the internal timer
		void setTimer(const float timoutInSeconds);
		// Update internal timer. Internal timer will be decremented 
		// of given amount of time in seconds 
		void updateTimer(const float elapsedSeconds);
		// Return if timer has expired (<= 0)
		bool hasExpired();

	private:
		EventTypes mType;
		EventPriorities mPriority;
		EventData mData;
		EventTimer mTimer;
	};

	typedef boost::shared_ptr<Event> EventPtr;

	/** Functor class used to compare two event pointers */
	class EventComparator
	{
	public:
		/** Compare two events checking their priorities:
		    The event with the lower value will have higher priority */
		bool operator()(EventPtr& evt1, EventPtr& evt2);
	};

	// --------------------------------
	class CollisionEvent: public Event
	{
	public:
		CollisionEvent();
		~CollisionEvent(){};
	};

	typedef boost::shared_ptr<CollisionEvent> CollisionEventPtr;

	// --------------------------------	
	class EnemyAttackEvent : public Event
	{
	public:
		EnemyAttackEvent(EnemyPtr e);
		~EnemyAttackEvent(){};
		
		EnemyPtr getEnemy(){return mEnemy;}

	private:		
		EnemyPtr mEnemy;
	};

	typedef boost::shared_ptr<EnemyAttackEvent> EnemyAttackEventPtr;

	// --------------------------------
	class EnemyHitEvent : public Event
	{
	public:
		EnemyHitEvent(EnemyPtr e, PlayerPtr p);
		~EnemyHitEvent(){};
		
		EnemyPtr getEnemy(){return mEnemy;}
		PlayerPtr getPlayer(){return mPlayer;}
		
		void setDamage(Ogre::Real damage){mDamage = damage;}
		Ogre::Real getDamage(){return mDamage;}

	private:
		EnemyPtr mEnemy;
		PlayerPtr mPlayer;

		Ogre::Real mDamage;
	};

	typedef boost::shared_ptr<EnemyHitEvent> EnemyHitEventPtr;

	// --------------------------------
	class EnemyKilledEvent : public Event
	{
	public:
		EnemyKilledEvent(EnemyPtr e, PlayerPtr p);
		~EnemyKilledEvent(){};
		
		EnemyPtr getEnemy(){return mEnemy;}
		PlayerPtr getPlayer(){return mPlayer;}

	private:
		EnemyPtr mEnemy;
		PlayerPtr mPlayer;
	};

	typedef boost::shared_ptr<EnemyKilledEvent> EnemyKilledEventPtr;

	// --------------------------------
	class EnemyDeadEvent : public Event
	{
	public:
		EnemyDeadEvent(EnemyPtr e);
		~EnemyDeadEvent(){};
		
		EnemyPtr getEnemy(){return mEnemy;}

	private:
		EnemyPtr mEnemy;
	};

	typedef boost::shared_ptr<EnemyDeadEvent> EnemyDeadEventPtr;

	// --------------------------------
	class EnemyRemoveEvent : public Event
	{
	public:
		EnemyRemoveEvent(EnemyPtr e);
		~EnemyRemoveEvent(){};
		
		EnemyPtr getEnemy(){return mEnemy;}

	private:
		EnemyPtr mEnemy;
	};

	typedef boost::shared_ptr<EnemyRemoveEvent> EnemyRemoveEventPtr;

	// --------------------------------	
	class EnemyCustomEvent : public Event
	{
	public:
		EnemyCustomEvent(EnemyPtr e);
		~EnemyCustomEvent(){};
		
		EnemyPtr getEnemy(){return mEnemy;}

	private:		
		EnemyPtr mEnemy;
	};

	typedef boost::shared_ptr<EnemyCustomEvent> EnemyCustomEventPtr;

	// --------------------------------	
	class EnemyCreationEvent : public Event
	{
	public:
		EnemyCreationEvent(Enemy::EnemyTypes type, int difficult, Vector3 position, int gameArea);
		~EnemyCreationEvent(){};
		
		Enemy::EnemyTypes getType(){return mType;}
		int getDifficult(){return mDifficult;}
		Vector3 getPosition(){return mPosition;}
		int getGameArea(){return mGameArea;}

	private:
		Enemy::EnemyTypes mType;
		int mDifficult;
		Vector3 mPosition;
		int mGameArea;
	};

	typedef boost::shared_ptr<EnemyCreationEvent> EnemyCreationEventPtr;

	// --------------------------------	
	class EnemyPhysicsEvent : public Event
	{
	public:
		EnemyPhysicsEvent(EnemyPtr enemy, int gameArea);
		~EnemyPhysicsEvent(){};
		
		EnemyPtr getEnemy(){return mEnemy;}
		int getGameArea(){return mGameArea;}

	private:
		EnemyPtr mEnemy;
		int mGameArea;
	};

	typedef boost::shared_ptr<EnemyPhysicsEvent> EnemyPhysicsEventPtr;

	// --------------------------------	
	class EnemyCreateItemEvent : public Event
	{
	public:
		EnemyCreateItemEvent(EnemyPtr e, int gameArea);
		~EnemyCreateItemEvent(){};
		
		EnemyPtr getEnemy(){return mEnemy;}
		int getGameArea(){return  mGameArea;}

	private:		
		EnemyPtr mEnemy;
		int mGameArea;
	};

	typedef boost::shared_ptr<EnemyCreateItemEvent> EnemyCreateItemEventPtr;

	// --------------------------------
	class PlayerHitEvent : public Event
	{
	public:
		PlayerHitEvent(EnemyPtr e, PlayerPtr p);
		~PlayerHitEvent(){};
		
		PlayerPtr getPlayer(){return mPlayer;}
		EnemyPtr getEnemy(){return mEnemy;}		

	private:		
		PlayerPtr mPlayer;
		EnemyPtr mEnemy;

		Ogre::Real damage;
	};

	typedef boost::shared_ptr<PlayerHitEvent> PlayerHitEventPtr;

	// --------------------------------
	class PlayerKilledEvent : public Event
	{
	public:
		PlayerKilledEvent(PlayerPtr p);
		~PlayerKilledEvent(){};
		
		PlayerPtr getPlayer(){return mPlayer;}

	private:		
		PlayerPtr mPlayer;

		Ogre::Real damage;
	};

	typedef boost::shared_ptr<PlayerKilledEvent> PlayerKilledEventPtr;

	// --------------------------------
	class PlayerAttackEvent : public Event
	{
	public:
		PlayerAttackEvent(PlayerPtr p);
		
		PlayerPtr getPlayer(){return mPlayer;}

	private:		
		PlayerPtr mPlayer;
	};

	typedef boost::shared_ptr<PlayerAttackEvent> PlayerAttackEventPtr;

	// --------------------------------
	class PlayerAttackSpecialEvent : public Event
	{
	public:
		PlayerAttackSpecialEvent(PlayerPtr p);
		~PlayerAttackSpecialEvent(){};
		
		PlayerPtr getPlayer(){return mPlayer;}

	private:		
		PlayerPtr mPlayer;
	};

	typedef boost::shared_ptr<PlayerAttackSpecialEvent> PlayerAttackSpecialEventPtr;

	
	// --------------------------------
	class PlayerStatusUpdateEvent : public Event
	{
	public:
		PlayerStatusUpdateEvent(PlayerPtr player);
		~PlayerStatusUpdateEvent(){};
		
		PlayerPtr getPlayer(){return mPlayer;}

	private:
		PlayerPtr mPlayer;
	};

	typedef boost::shared_ptr<PlayerStatusUpdateEvent> PlayerStatusUpdateEventPtr;

	// --------------------------------
	class ItemCreationEvent : public Event
	{
	public:
		ItemCreationEvent(ItemPtr item);
		~ItemCreationEvent(){};
		
		ItemPtr getItem(){return mItem;}

	private:
		ItemPtr mItem;
	};

	typedef boost::shared_ptr<ItemCreationEvent> ItemCreationEventPtr;

	// --------------------------------
	class ItemCatchEvent : public Event
	{
	public:
		ItemCatchEvent(PlayerPtr p, ItemPtr item);
		~ItemCatchEvent(){};
		
		PlayerPtr getPlayer(){return mPlayer;}
		ItemPtr getItem(){return mItem;}

	private:		
		PlayerPtr mPlayer;
		ItemPtr mItem;
	};

	typedef boost::shared_ptr<ItemCatchEvent> ItemCatchEventPtr;

	// --------------------------------
	class ItemRemoveEvent : public Event
	{
	public:
		ItemRemoveEvent(ItemPtr item);
		~ItemRemoveEvent(){};
		
		ItemPtr getItem(){return mItem;}

	private:
		ItemPtr mItem;
	};

	typedef boost::shared_ptr<ItemRemoveEvent> ItemRemoveEventPtr;

	// --------------------------------	
	class ItemPhysicsEvent : public Event
	{
	public:
		ItemPhysicsEvent(ItemPtr i, int gameArea);
		~ItemPhysicsEvent(){};
		
		ItemPtr getItem(){return mItem;}
		int getGameArea(){return mGameArea;}

	private:
		ItemPtr mItem;
		int mGameArea;
	};

	typedef boost::shared_ptr<ItemPhysicsEvent> ItemPhysicsEventPtr;

	// --------------------------------
	class ObjectHitEvent : public Event
	{
	public:
		ObjectHitEvent(ObjectPtr o, PlayerPtr p);
		~ObjectHitEvent(){};
		
		ObjectPtr getObject(){return mObject;}
		PlayerPtr getPlayer(){return mPlayer;}
		
		void setDamage(Ogre::Real damage){mDamage = damage;}
		Ogre::Real getDamage(){return mDamage;}

	private:
		ObjectPtr mObject;
		PlayerPtr mPlayer;

		Ogre::Real mDamage;
	};

	typedef boost::shared_ptr<ObjectHitEvent> ObjectHitEventPtr;

		// --------------------------------
	class ObjectKilledEvent : public Event
	{
	public:
		ObjectKilledEvent(ObjectPtr o, PlayerPtr p);
		~ObjectKilledEvent(){};
		
		ObjectPtr getObject(){return mObject;}
		PlayerPtr getPlayer(){return mPlayer;}

	private:
		ObjectPtr mObject;
		PlayerPtr mPlayer;
	};

	typedef boost::shared_ptr<ObjectKilledEvent> ObjectKilledEventPtr;

	// --------------------------------
	class ObjectRemoveEvent : public Event
	{
	public:
		ObjectRemoveEvent(ObjectPtr o);
		~ObjectRemoveEvent(){};
		
		ObjectPtr getObject(){return mObject;}

	private:
		ObjectPtr mObject;
	};

	typedef boost::shared_ptr<ObjectRemoveEvent> ObjectRemoveEventPtr;

	// --------------------------------	
	class ObjectCustomEvent : public Event
	{
	public:
		ObjectCustomEvent(ObjectPtr o);
		~ObjectCustomEvent(){};
		
		ObjectPtr getObject(){return mObject;}

	private:		
		ObjectPtr mObject;
	};

	typedef boost::shared_ptr<ObjectCustomEvent> ObjectCustomEventPtr;

	// --------------------------------
	class GameAreaChangedEvent : public Event
	{
	public:
		GameAreaChangedEvent(int level, int previousArea, int actualArea, bool isLast);
		~GameAreaChangedEvent(){};
		
		int getPreviousArea(){ return mPreviousArea; }
		int getActualArea(){ return mActualArea; }
		int getLevel(){ return mLevel; };
		int isLast(){ return mIsLast; }

	private:
		int mPreviousArea;
		int mActualArea;
		int mLevel;
		bool mIsLast;
	};

	typedef boost::shared_ptr<GameAreaChangedEvent> GameAreaChangedEventPtr;

	// --------------------------------
	class GameAreaClearedEvent : public Event
	{
	public:
		GameAreaClearedEvent(int level, int gameArea, int type, bool isLast);
		~GameAreaClearedEvent(){};
		
		int getGameArea(){ return mGameArea; }
		int getType(){ return mType; }
		int getLevel(){ return mLevel; };
		bool isLast(){ return mIsLast; }

	private:
		int mLevel;
		int mGameArea;
		int mType;
		int mIsLast;
	};

	typedef boost::shared_ptr<GameAreaClearedEvent> GameAreaClearedEventPtr;

	// --------------------------------
	class GameAreaEnemiesDeathEvent : public Event
	{
	public:
		GameAreaEnemiesDeathEvent(int level, int gameArea);
		~GameAreaEnemiesDeathEvent(){};
		
		int getLevel(){ return mLevel; }
		int getGameArea(){ return mGameArea; }

	private:
		int mGameArea;
		int mLevel;
	};

	typedef boost::shared_ptr<GameAreaEnemiesDeathEvent> GameAreaEnemiesDeathEventPtr;

	// --------------------------------
	class ProjectileFireEvent : public Event
	{
	public:
		ProjectileFireEvent( EnemyPtr e, Vector3 target );
		~ProjectileFireEvent(){};
		
		EnemyPtr getEnemy(){ return mEnemy; }
		Vector3 getTarget(){ return mTarget; }

	private:		
		EnemyPtr mEnemy;
		Vector3 mTarget;
	};

	typedef boost::shared_ptr<ProjectileFireEvent> ProjectileFireEventPtr;

	// --------------------------------
	class ProjectileUpdateEvent : public Event
	{
	public:
		ProjectileUpdateEvent( ProjectilePtr p, Enemy::EnemyTypes t );
		~ProjectileUpdateEvent(){};
		
		ProjectilePtr getProjectile(){return mProjectile;}
		Enemy::EnemyTypes getType(){return mType;}

	private:		
		ProjectilePtr mProjectile;
		Enemy::EnemyTypes mType;
	};

	typedef boost::shared_ptr<ProjectileUpdateEvent> ProjectileUpdateEventPtr;

	// --------------------------------
	class ProjectileRemoveEvent : public Event
	{
	public:
		ProjectileRemoveEvent( ProjectilePtr p);
		~ProjectileRemoveEvent(){};
		
		ProjectilePtr getProjectile(){return mProjectile;}

	private:		
		ProjectilePtr mProjectile;
	};

	typedef boost::shared_ptr<ProjectileRemoveEvent> ProjectileRemoveEventPtr;

	// --------------------------------
	class ProjectileHitEvent : public Event
	{
	public:
		ProjectileHitEvent( ProjectilePtr p, PlayerPtr player );
		~ProjectileHitEvent(){};
		
		ProjectilePtr getProjectile(){return mProjectile;}
		PlayerPtr getPlayer(){return mPlayer;}

	private:		
		ProjectilePtr mProjectile;
		PlayerPtr mPlayer;
	};

	typedef boost::shared_ptr<ProjectileHitEvent> ProjectileHitEventPtr;

	// --------------------------------
	class LevelCompleteEvent : public Event
	{
	public:
		LevelCompleteEvent( int previousLevel, int newLevel, bool isLast );
		~LevelCompleteEvent(){};
		
		int getPreviousLevel(){return mPreviousLevel;}
		int getNewLevel(){return mNewLevel;}
		bool isLast(){return mIsLast;}

	private:		
		int mPreviousLevel;
		int mNewLevel;
		int mIsLast;
	};

	typedef boost::shared_ptr<LevelCompleteEvent> LevelCompleteEventPtr;

	// --------------------------------
	class WeatherChangedEvent : public Event
	{
	public:
		WeatherChangedEvent( WeatherTypes weatherType );
		~WeatherChangedEvent(){};
		
		WeatherTypes getWeatherType(){return mWeatherType;}

	private:		
		WeatherTypes mWeatherType;
	};

	typedef boost::shared_ptr<WeatherChangedEvent> WeatherChangedEventPtr;

	// --------------------------------
	class GameAreaFlashCounterEvent : public Event
	{
	public:
		GameAreaFlashCounterEvent(int seconds);
		~GameAreaFlashCounterEvent(){};
		
		int getSeconds(){ return mSeconds; }

	private:
		int mSeconds;
	};

	typedef boost::shared_ptr<GameAreaFlashCounterEvent> GameAreaFlashCounterEventPtr;

	// --------------------------------
	class SpecialEffectEvent : public Event
	{
	public:

		enum EffectType
		{
			Explosion,
			Quake,
			Lightning
		};

		SpecialEffectEvent(EffectType type, float time, float amount);
		~SpecialEffectEvent(){};
		
		EffectType getType(){ return mType; }
		float getTime(){ return mTime; }
		float getAmount(){ return mAmount; }

	private:
		EffectType mType;
		float mTime;
		float mAmount;
	};

	typedef boost::shared_ptr<SpecialEffectEvent> SpecialEffectEventPtr;

	// --------------------------------	
	class CutSceneStartEvent : public Event
	{
	public:
		CutSceneStartEvent(CutScene::CutSceneId id);
		~CutSceneStartEvent(){};
		
		CutScene::CutSceneId getId(){return mId;}

	private:		
		CutScene::CutSceneId mId;
	};

	typedef boost::shared_ptr<CutSceneStartEvent> CutSceneStartEventPtr;

	// --------------------------------	
	class CutSceneEndEvent : public Event
	{
	public:
		CutSceneEndEvent(CutScene::CutSceneId id);
		~CutSceneEndEvent(){};
		
		CutScene::CutSceneId getId(){return mId;}

	private:		
		CutScene::CutSceneId mId;
	};

	typedef boost::shared_ptr<CutSceneEndEvent> CutSceneEndEventPtr;
}

#endif // __EVENT_H_