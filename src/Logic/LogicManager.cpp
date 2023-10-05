#include "..\..\include\Logic\LogicManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> LogicManager* Ogre::Singleton<LogicManager>::ms_Singleton = 0;
LogicManager* LogicManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
LogicManager& LogicManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

LogicManager::LogicManager()
: mEnabled(0)
{
	//
	// TODO Constructor logic HERE
	//
}

LogicManager::~LogicManager()
{
	//
	// TODO Distructor logic HERE
	//
}

bool LogicManager::initialize()
{
	return true;
}

void LogicManager::finalize()
{
}

bool LogicManager::update(const float elapsedSeconds)
{
	if(!mEnabled)
		return false;

	LUA_PROPERTY_SET(LogicManager,ElapsedSeconds,elapsedSeconds);

	float totalSeconds = LUA_PROPERTY_GET(LogicManager,TotalSeconds);

	LUA_PROPERTY_SET(LogicManager,TotalSeconds, totalSeconds + elapsedSeconds);

	return true;
}

// --------------------------------
// Lua Game Lib
// --------------------------------
LUA_BEGIN_BINDING(LogicManager,logiclib)
LUA_BIND(LogicManager,enable)
LUA_BIND(LogicManager,disable)
LUA_BIND(LogicManager,isEnabled)
LUA_BIND(LogicManager,getTotalSeconds)
LUA_BIND(LogicManager,getElapsedSeconds)
LUA_END_BINDING()

//
// Load lua scripts that will be used by this manager
//
LUA_BEGIN_LOAD_SCRIPTS(LogicManager)
// 
// TODO : Load scripts if needed
//
LUA_END_LOAD_SCRIPTS()

void LogicManager::luaInitialize(lua_State *L)
{
	LuaInterface::luaInitialize(L);

	LUA_PROPERTY_SET(LogicManager,TotalSeconds,0);
	LUA_PROPERTY_SET(LogicManager,ElapsedSeconds,0);
}

LUA_DEFINE_FUNCTION(LogicManager,enable)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	LogicManager::getSingleton().enable();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(LogicManager,disable)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	LogicManager::getSingleton().disable();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(LogicManager, isEnabled)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	bool isEnabled = LogicManager::getSingleton().isEnabled();

	lua_pushboolean(L, isEnabled);

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(LogicManager,getTotalSeconds)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	/* push the total seconds */
	lua_pushnumber(L, LUA_PROPERTY_GET(LogicManager,TotalSeconds));

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(LogicManager,getElapsedSeconds)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	/* push the total seconds */
	lua_pushnumber(L, LUA_PROPERTY_GET(LogicManager,ElapsedSeconds));

	/* return the number of results */
	return 1;
}