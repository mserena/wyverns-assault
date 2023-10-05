#include "..\..\..\include\Entity\Player\PlayerManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> PlayerManager* Ogre::Singleton<PlayerManager>::ms_Singleton = 0;
PlayerManager* PlayerManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
PlayerManager& PlayerManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

PlayerManager::PlayerManager(Ogre::SceneManager* sceneManager)
: mSceneManager(0)
, mIsDebugEnabled(false)
, mPlayerNode(0)
, mEnabled(true)
{
	mSceneManager = sceneManager;
}

void PlayerManager::initialize()
{
	mPlayerNode = mSceneManager->getRootSceneNode()->createChildSceneNode(PLAYER_NODE_NAME);

	mRefillTimer = 0.0f;
}

PlayerManager::~PlayerManager()
{
	finalize();
}

void PlayerManager::finalize()
{
	mPlayerList.clear();
	mPlayerMap.clear();

	Utils::Destroy(mSceneManager, PLAYER_NODE_NAME);
	mPlayerNode = NULL;
}

void PlayerManager::update(const float elapsedSeconds)
{
	if(mEnabled)
	{
		for(int i = 0; i < mPlayerList.size() ; i++)
		{		
			PlayerPtr player =  mPlayerList[i];
			player->updateEntity(elapsedSeconds);

			// Physic debugg control
			player->setDebugEnabled(mIsDebugEnabled);

			// Check if dead
			if( player->getLife() == 0 )
			{
				PlayerKilledEventPtr playerKilledEventPtr = PlayerKilledEventPtr(new PlayerKilledEvent(player));
				EVENTS_FIRE(playerKilledEventPtr);
			}

			mRefillTimer += elapsedSeconds;
			// Every 0.1 seconds, fill special bar 1 units
			if( mRefillTimer > 0.05 )
			{
				player->setSpecial( player->getSpecial() + 1 );
				mRefillTimer = 0.0f;
				PlayerStatusUpdateEventPtr playerStatusUpdateEventPtr = PlayerStatusUpdateEventPtr(new PlayerStatusUpdateEvent(player));
				EVENTS_FIRE(playerStatusUpdateEventPtr);
			}
		}
	}
}

PlayerPtr PlayerManager::createPlayer(Ogre::String name, Ogre::String mesh)
{
	// Player node
	Ogre::Entity* playerMesh = mSceneManager->createEntity(name, mesh);
	playerMesh->setQueryFlags(SceneManager::ENTITY_TYPE_MASK);
	Ogre::SceneNode* playerSceneNode = mPlayerNode->createChildSceneNode(name);
	playerSceneNode->attachObject(playerMesh);

	PlayerPtr player = PlayerPtr(new Player(name));

	player->initializeEntity( playerMesh, playerSceneNode, mSceneManager );

	mPlayerList.push_back(player);
	mPlayerMap[name] = player;

	return player;
}


PlayerPtr PlayerManager::getPlayer(Ogre::String name)
{
	return mPlayerMap[name];
}

bool PlayerManager::removePlayer(Ogre::String name)
{
	//
	// TODO : maybe we don't really need a list, and we can just use a map...
	//
	PlayerPtr playerToErase = mPlayerMap[name];

	mPlayerMap.erase(name);
	
	PlayerListIterator it = find(mPlayerList.begin(), mPlayerList.end(), playerToErase);
	
	if( it != mPlayerList.end() )
		mPlayerList.erase(it);

	mSceneManager->destroyEntity(name);//getRootSceneNode()->removeChild(name);

	return true;
}

/* Set player direction of new move
*/
void PlayerManager::move(Ogre::String playerName, Vector3 direction)
{
	if(mEnabled)
	{
		PlayerPtr player = mPlayerMap[playerName];

		player->setDirection(direction);

		player->setMoving(direction != Vector3::ZERO);

		if(direction != Vector3::ZERO)
		{
			Quaternion q1 = player->getOrientation();
			// Get current direction where player is facing
			Vector3 currentDirection = q1 * Vector3::UNIT_Z;
			Quaternion q2 = currentDirection.getRotationTo(direction);
			player->setOrientation( q1*q2 );
		}
	}
}

void PlayerManager::attack(Ogre::String name)
{
	if(mEnabled)
	{
		PlayerPtr player = getPlayer(name);

		player->attackA();

		if( player->attackStart() )
		{		
			PlayerAttackEventPtr evt = PlayerAttackEventPtr(new PlayerAttackEvent(player));
			EVENTS_FIRE(evt);
			player->attackFinished();
			player->setAttackHited(false);
		}
	}
}

void PlayerManager::attackSpecial(Ogre::String name)
{
	if(mEnabled)
	{
		PlayerPtr player = getPlayer(name);

		player->attackSpecial();

		if( player->attackStart() )
		{		
			PlayerAttackSpecialEventPtr evt = PlayerAttackSpecialEventPtr(new PlayerAttackSpecialEvent(player));
			EVENTS_FIRE(evt);
			player->attackFinished();
			player->setAttackHited(false);
		}
	}
}

void PlayerManager::stop(Ogre::String playerName)
{
	if(mEnabled)
	{
		PlayerPtr player = mPlayerMap[playerName];
		player->setDirection(Vector3::ZERO);
	}
}

void PlayerManager::setDebugEnabled(bool isDebugEnabled)
{
	if(mIsDebugEnabled != isDebugEnabled)
	{
		mIsDebugEnabled = isDebugEnabled;
	}
}

void PlayerManager::toggleGodMode()
{
	for(int i = 0; i < mPlayerList.size() ; i++)
	{
		PlayerPtr player =  mPlayerList[i];
		player->toggleGodMode();

		if(player->isGodModeOn())
		{
			player->setLife(PLAYER_MAX_LIFE); // Max life 
			player->setSpecial(PLAYER_MAX_SPECIAL); // Max special
		}
	}
}

// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(PlayerManager)
	EVENTS_REGISTER_HANDLER(PlayerManager,PlayerHit)
	EVENTS_REGISTER_HANDLER(PlayerManager,PlayerKilled)
	EVENTS_REGISTER_HANDLER(PlayerManager,ItemCatch)
	EVENTS_REGISTER_HANDLER(PlayerManager,EnemyKilled)
	EVENTS_REGISTER_HANDLER(PlayerManager,ObjectKilled)
	EVENTS_REGISTER_HANDLER(PlayerManager,ProjectileHit)
	EVENTS_REGISTER_HANDLER(PlayerManager,GameAreaChanged)
	EVENTS_REGISTER_HANDLER(PlayerManager,GameAreaCleared)
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(PlayerManager)
	EVENTS_UNREGISTER_HANDLER(PlayerManager,PlayerHit)
	EVENTS_UNREGISTER_HANDLER(PlayerManager,PlayerKilled)
	EVENTS_UNREGISTER_HANDLER(PlayerManager,ItemCatch)
	EVENTS_UNREGISTER_HANDLER(PlayerManager,EnemyKilled)
	EVENTS_UNREGISTER_HANDLER(PlayerManager,ObjectKilled)
	EVENTS_UNREGISTER_HANDLER(PlayerManager,ProjectileHit)
	EVENTS_UNREGISTER_HANDLER(PlayerManager,GameAreaChanged)
	EVENTS_UNREGISTER_HANDLER(PlayerManager,GameAreaCleared)
EVENTS_END_UNREGISTER_HANDLERS()

EVENTS_DEFINE_HANDLER(PlayerManager, PlayerHit)
{
	Debug::Out("PlayerManager : handlePlayerHitEvent");

	PlayerPtr player = evt->getPlayer();
	EnemyPtr enemy = evt->getEnemy();
	
	if(!player->isGodModeOn()) // DEBUG : God mode!
	{
		if( enemy->isSpecial() )
			player->hurt( enemy->getSpecialHitDamage() );
		else
			player->hurt( enemy->getHitDamage() );
	}
}

EVENTS_DEFINE_HANDLER(PlayerManager, PlayerKilled)
{
	Debug::Out("PlayerManager : handlePlayerKilledEvent");

	PlayerPtr player = evt->getPlayer();

	player->die();
}

EVENTS_DEFINE_HANDLER(PlayerManager, ItemCatch)
{
	Debug::Out("PlayerManager : handleItemCatchEvent");

	PlayerPtr player = evt->getPlayer();
	ItemPtr item = evt->getItem();

	player->setLife( player->getLife() + item->getLife() );
	player->addPoints(item->getPoints());

	if ( item->getDrunkTime() > 0 )
	{
		player->drunk(item->getDrunkTime() / 2, item->getDrunkTime());
		player->setSpecial(100.0f);
	}
}

EVENTS_DEFINE_HANDLER(PlayerManager, EnemyKilled)
{
	Debug::Out("PlayerManager : handleEnemyKilledEvent");

	PlayerPtr player = evt->getPlayer();
	EnemyPtr enemy = evt->getEnemy();

	player->addPoints(enemy->getPoints());
}

EVENTS_DEFINE_HANDLER(PlayerManager, ObjectKilled)
{
	Debug::Out("PlayerManager : handleObjectKilledEvent");

	PlayerPtr player = evt->getPlayer();
	ObjectPtr object = evt->getObject();

	player->addPoints(object->getPoints());
}

EVENTS_DEFINE_HANDLER(PlayerManager, ProjectileHit)
{
	Debug::Out("PlayerManager : handleProjectileHitEvent");

	PlayerPtr player = evt->getPlayer();
	ProjectilePtr projectile = evt->getProjectile();
	
	if(!player->isGodModeOn()) // DEBUG : God mode!
	{
		player->hurt( projectile->getProjectileDamage() );
	}
}

EVENTS_DEFINE_HANDLER(PlayerManager, GameAreaChanged)
{
	Debug::Out("PlayerManager : handleGameAreaChangedEvent");

	int level = evt->getLevel();
	int previousArea = evt->getPreviousArea();
	int actualArea = evt->getActualArea();

	if( level == 0)
	{
		// Bonus area, double players speed
		if( previousArea == 2 && actualArea == 3 )
		{
			for(int i = 0; i < mPlayerList.size() ; i++)
			{
				mPlayerList[i]->doubleSpeed();
			}
		}			
	}
}

EVENTS_DEFINE_HANDLER(PlayerManager, GameAreaCleared)
{
	Debug::Out("PlayerManager : handleGameAreaClearedEvent");

	int level = evt->getLevel();
	int gameArea = evt->getGameArea();
	
	if( level == 0)
	{
		// Return players to normal speed when bonus finished
		if( gameArea == 3 )
		{
			for(int i = 0; i < mPlayerList.size() ; i++)
			{
				mPlayerList[i]->resetSpeed();
			}
		}			
	}
}

// --------------------------------
// Lua Player Lib
// --------------------------------
LUA_BEGIN_BINDING(PlayerManager, playerlib)
LUA_BIND(PlayerManager, getPlayerPosition)
LUA_BIND(PlayerManager, getNumPlayers)
LUA_BIND(PlayerManager, enable)
LUA_BIND(PlayerManager, disable)
LUA_BIND(PlayerManager, isEnabled)
LUA_END_BINDING()

//
// Load lua scripts that will be used by this manager
//
LUA_BEGIN_LOAD_SCRIPTS(PlayerManager)
// 
// TODO : Load scripts if needed
//
LUA_END_LOAD_SCRIPTS()

LUA_DEFINE_FUNCTION(PlayerManager, getPlayerPosition)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	Ogre::String playerName = lua_tostring(L,1);

	//Player player = PlayerManager::getSingleton().getPlayer(playerName);

	//Vector3 position = player->getPosition();

	Vector3 position = Vector3(1,2,3);

	/* push the X */
	lua_pushnumber(L, position.x);
	/* push the Y */
	lua_pushnumber(L, position.y);
	/* push the Z */
	lua_pushnumber(L, position.z);

	/* return the number of results */
	return 3;
}

LUA_DEFINE_FUNCTION(PlayerManager, getNumPlayers)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	int numPlayers = 0;

	PlayerList list = PlayerManager::getSingleton().getPlayerList();
	
	for(int i = 0; i < list.size() ; i++)
	{
		PlayerPtr player =  list[i];
		if(!player->isDeath())
			numPlayers++;
	}

	lua_pushnumber(L, numPlayers);

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(PlayerManager, disable)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	PlayerManager::getSingleton().disable();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(PlayerManager, enable)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	PlayerManager::getSingleton().enable();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(PlayerManager, isEnabled)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	bool isEnabled = PlayerManager::getSingleton().isEnabled();

	lua_pushboolean(L, isEnabled);

	/* return the number of results */
	return 1;
}
