#include "..\include\GUI\GuiManager.h"

using namespace Ogre;
using namespace WyvernsAssault;
using namespace Hikari;

// BEGIN SINGLETON
template<> GuiManager* Ogre::Singleton<GuiManager>::ms_Singleton = 0;
GuiManager* GuiManager::getSingletonPtr(void)
{
	return ms_Singleton;
}
GuiManager& GuiManager::getSingleton(void)
{  
	assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

GuiManager::GuiManager()
: flashCount(false)
, cutScene(false)
, goGo(false)
, level(true)
, showGui(true)
{
	mHikariManager = new HikariManager("\\data\\flash");
}

GuiManager::~GuiManager()
{
	finalize();
}

void GuiManager::update(float const elapsedSeconds, int enemyCount)
{
	mHikariManager->update();

	if(!mFlashCounterEnemyUI)
		mFlashCounterEnemyUI = GuiFlashCounterPtr( new GuiFlashCounter(GuiFlashCounter::CounterTypes::Manual ,mWindow->getViewport(0), GuiScreenId::FlashCounterEnemyGui, 0, mHikariManager) );
	else
		mFlashCounterEnemyUI->update(elapsedSeconds, enemyCount);

	if( showGui && level )
	{
		if(flashCount)
		{
			flashCount = mFlashCounterUI->update(elapsedSeconds, enemyCount);
			mFlashCounterEnemyUI->hide();
			if(mFlashGoGo)
				mFlashGoGo->hide();
		}
		else if(goGo && level)
		{
			mFlashGoGo->show();
			mFlashCounterEnemyUI->hide();
			if(mFlashCounterUI)
				mFlashCounterUI->hide();
		}
		else
		{
			mFlashCounterEnemyUI->show();
			if(mFlashGoGo)
				mFlashGoGo->hide();
			if(mFlashCounterUI)
				mFlashCounterUI->hide();
		}		
	}
}

bool GuiManager::initialize(Ogre::Root* root, Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window)
{
	mRoot = root;
	mSceneManager = sceneManager;
	mWindow = window;

	return true;
}

void GuiManager::finalize()
{
	flashCount = false;

	this->hide();

	if( mFlashCounterUI )
		mFlashCounterUI.reset();

	if( mFlashCounterEnemyUI )
		mFlashCounterEnemyUI.reset(); 

	if( mFlashGoGo )
		mFlashGoGo.reset();

	if( mHikariManager )
		mHikariManager = NULL;

	mGuiScreenMap.clear();
}

void GuiManager::reset()
{
	this->hide();
}

void GuiManager::hide()
{
	if(mFlashCounterEnemyUI)
		mFlashCounterEnemyUI->hide();

	if(mFlashCounterUI)
		mFlashCounterUI->hide();

	if(mFlashGoGo)
		mFlashGoGo->hide();

	showGui = false;
}

void GuiManager::show()
{
	if(level)
	{
		if(mFlashCounterEnemyUI)
			mFlashCounterEnemyUI->show();

		if(mFlashCounterUI)
			mFlashCounterUI->show();

		if(mFlashGoGo)
			mFlashGoGo->show();
	}

	showGui = true;
}

GuiScreenPtr GuiManager::createScreen(GuiScreenId id, const Ogre::String& name)
{
	if( this->hasScreen(id) )
	{
		return mGuiScreenMap[id];
	}
	else
	{
		GuiScreenPtr guiScreen = GuiScreenPtr( new GuiScreen(mSceneManager, id, name) );
		mGuiScreenMap[id] = guiScreen;
		return guiScreen;
	}
}

bool GuiManager::hasScreen(GuiScreenId id)
{
	return mGuiScreenMap[id] != NULL;
}

void GuiManager::removeScreen(GuiScreenId id)
{
	mGuiScreenMap[id]->removeGui();
	mGuiScreenMap.erase(id);
}

void GuiManager::showScreen(GuiScreenId id)
{
	mGuiScreenMap[id]->show();
}

void GuiManager::hideScreen(GuiScreenId id)
{
	mGuiScreenMap[id]->hide();
}

GuiScreenPtr GuiManager::getScreen(GuiScreenId id)
{
	return mGuiScreenMap[id];
}

//----------------------------------------------------------------//
bool GuiManager::mouseMoved( const OIS::MouseEvent &arg )
{
	return true;
}

//----------------------------------------------------------------//
bool GuiManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return true;
}

//----------------------------------------------------------------//
bool GuiManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return true;
}

//----------------------------------------------------------------//
bool GuiManager::keyPressed( const OIS::KeyEvent &arg )
{
	return true;
}

//----------------------------------------------------------------//
bool GuiManager::keyReleased( const OIS::KeyEvent &arg )
{
	return true;
}

// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(GuiManager)	
	EVENTS_REGISTER_HANDLER(GuiManager, ItemCatch);
	EVENTS_REGISTER_HANDLER(GuiManager, PlayerAttackSpecial);
	EVENTS_REGISTER_HANDLER(GuiManager, EnemyKilled);
	EVENTS_REGISTER_HANDLER(GuiManager, ObjectKilled);
	EVENTS_REGISTER_HANDLER(GuiManager, PlayerHit);
	EVENTS_REGISTER_HANDLER(GuiManager, PlayerStatusUpdate);
	EVENTS_REGISTER_HANDLER(GuiManager, GameAreaFlashCounter);
	EVENTS_REGISTER_HANDLER(GuiManager, GameAreaCleared);
	EVENTS_REGISTER_HANDLER(GuiManager, GameAreaChanged);
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(GuiManager)
	EVENTS_UNREGISTER_HANDLER(GuiManager, ItemCatch);
	EVENTS_UNREGISTER_HANDLER(GuiManager, PlayerAttackSpecial);
	EVENTS_UNREGISTER_HANDLER(GuiManager, EnemyKilled);
	EVENTS_UNREGISTER_HANDLER(GuiManager, ObjectKilled);
	EVENTS_UNREGISTER_HANDLER(GuiManager, PlayerHit);
	EVENTS_UNREGISTER_HANDLER(GuiManager, PlayerStatusUpdate);
	EVENTS_UNREGISTER_HANDLER(GuiManager, GameAreaCleared);
	EVENTS_UNREGISTER_HANDLER(GuiManager, GameAreaChanged);
EVENTS_END_UNREGISTER_HANDLERS()


EVENTS_DEFINE_HANDLER(GuiManager, ItemCatch)
{
	Debug::Out("GuiManager : handleItemCatchEvent");

	PlayerPtr player = evt->getPlayer();

	GuiWidgetPtr ui = mGuiScreenMap[GuiScreenId::PlayGui]->getWidget( player->getGuiId() );

	GuiUserInterface::UserInterfaceData userData;

	userData.life = player->getLife();
	userData.special = player->getSpecial();
	userData.points = player->getPoints();

	ui->setData(&userData);
}

EVENTS_DEFINE_HANDLER(GuiManager, PlayerAttackSpecial)
{
	Debug::Out("GuiManager : handlePlayerAttackSpecialEvent");

	PlayerPtr player = evt->getPlayer();

	GuiWidgetPtr ui = mGuiScreenMap[GuiScreenId::PlayGui]->getWidget( player->getGuiId() );

	GuiUserInterface::UserInterfaceData userData;

	userData.life = player->getLife();
	userData.special = player->getSpecial();
	userData.points = player->getPoints();

	ui->setData(&userData);
}

EVENTS_DEFINE_HANDLER(GuiManager, EnemyKilled)
{
	Debug::Out("GuiManager : handleEnemyKilledEvent");

	PlayerPtr player = evt->getPlayer();

	GuiWidgetPtr ui = mGuiScreenMap[GuiScreenId::PlayGui]->getWidget( player->getGuiId() );

	GuiUserInterface::UserInterfaceData userData;

	userData.life = player->getLife();
	userData.special = player->getSpecial();
	userData.points = player->getPoints();

	ui->setData(&userData);
}

EVENTS_DEFINE_HANDLER(GuiManager, ObjectKilled)
{
	Debug::Out("GuiManager : handleObjectKilledEvent");

	PlayerPtr player = evt->getPlayer();

	GuiWidgetPtr ui = mGuiScreenMap[GuiScreenId::PlayGui]->getWidget( player->getGuiId() );

	GuiUserInterface::UserInterfaceData userData;

	userData.life = player->getLife();
	userData.special = player->getSpecial();
	userData.points = player->getPoints();

	ui->setData(&userData);
}

EVENTS_DEFINE_HANDLER(GuiManager, PlayerHit)
{
	Debug::Out("GuiManager : handlePlayerHitEvent");

	PlayerPtr player = evt->getPlayer();

	GuiWidgetPtr ui = mGuiScreenMap[GuiScreenId::PlayGui]->getWidget( player->getGuiId() );

	GuiUserInterface::UserInterfaceData userData;

	userData.life = player->getLife();
	userData.special = player->getSpecial();
	userData.points = player->getPoints();

	ui->setData(&userData);
}

EVENTS_DEFINE_HANDLER(GuiManager, PlayerStatusUpdate)
{
	Debug::Out("GuiManager : handlePlayerStatusUpdateEvent");

	PlayerPtr player = evt->getPlayer();

	GuiWidgetPtr ui = mGuiScreenMap[GuiScreenId::PlayGui]->getWidget( player->getGuiId() );

	GuiUserInterface::UserInterfaceData userData;

	userData.life = player->getLife();
	userData.special = player->getSpecial();
	userData.points = player->getPoints();

	ui->setData(&userData);
}

EVENTS_DEFINE_HANDLER(GuiManager, GameAreaFlashCounter)
{
	Debug::Out("GuiManager : handleGameAreaFlashCounterEvent");

	int seconds = (int)evt->getSeconds();

	if(!mFlashCounterUI)
		mFlashCounterUI = GuiFlashCounterPtr( new GuiFlashCounter(GuiFlashCounter::CounterTypes::Timer, mWindow->getViewport(0), GuiScreenId::FlashCounterGui, seconds, mHikariManager) );
	else
		if(showGui)
			mFlashCounterUI->setSeconds(seconds);

	flashCount = true;
}

EVENTS_DEFINE_HANDLER(GuiManager, GameAreaChanged)
{
	Debug::Out("GuiManager : handleGameAreaChangedEvent");

	if( evt->getLevel() != 1 )
	{
		if(mFlashGoGo)
			mFlashGoGo->hide();

		if(mFlashCounterEnemyUI)
			if(showGui)
				mFlashCounterEnemyUI->show();

		goGo = false;
	}
	else
	{
		hide();
		level = false;
	}
}

EVENTS_DEFINE_HANDLER(GuiManager, GameAreaCleared)
{
	Debug::Out("GuiManager : handleGameAreaClearedEvent");

	if( evt->getLevel() != 1)
	{
		if(!mFlashGoGo)
			mFlashGoGo = GuiFlashMoviePtr( new GuiFlashMovie(mWindow->getViewport(0), GuiScreenId::FlashMovieGoGoGui, mHikariManager ,"GoGo.swf") );
		else
			if(showGui)
				mFlashGoGo->show();

		if(mFlashCounterEnemyUI)
			mFlashCounterEnemyUI->hide();
		
		goGo = true;
	}
	else
	{
		hide();
		level = false;
	}
}

// --------------------------------
// Lua Gui Lib
// --------------------------------
LUA_BEGIN_BINDING(GuiManager, guilib)
LUA_BIND(GuiManager, showScreen)
LUA_BIND(GuiManager, hideScreen)
LUA_BIND(GuiManager, showFrame)
LUA_BIND(GuiManager, hideFrame)
LUA_BIND(GuiManager, setFrame)
LUA_BIND(GuiManager, showUi)
LUA_BIND(GuiManager, hideUi)
LUA_BIND(GuiManager, showForeground)
LUA_BIND(GuiManager, hideForeground)
LUA_BIND(GuiManager, setForeground)
LUA_END_BINDING()

//
// Load lua scripts that will be used by this manager
//
LUA_BEGIN_LOAD_SCRIPTS(GuiManager)
// 
// TODO : Load scripts if needed
//
LUA_END_LOAD_SCRIPTS()

LUA_DEFINE_FUNCTION(GuiManager,showScreen)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1, the screen id

	int id = luaL_checkint(L, 1);

	GuiManager::getSingleton().showScreen((GuiScreenId)id);

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(GuiManager,hideScreen)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1, the screen id

	int id = luaL_checkint(L, 1);

	GuiManager::getSingleton().hideScreen((GuiScreenId)id);

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(GuiManager,showUi)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1, the screen id

	GuiScreenPtr playScreen = GuiManager::getSingleton().getScreen(GuiScreenId::PlayGui);

	GuiWidgetPtr widget1 = playScreen->getWidget(GuiWidgetPlayId::UserInterface1);

	widget1->show();

	GuiManager::getSingleton().show();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(GuiManager,hideUi)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1, the screen id

	GuiScreenPtr playScreen = GuiManager::getSingleton().getScreen(GuiScreenId::PlayGui);

	GuiWidgetPtr widget1 = playScreen->getWidget(GuiWidgetPlayId::UserInterface1);

	widget1->hide();

	GuiManager::getSingleton().hide();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(GuiManager,showForeground)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1, the screen id

	//int id = luaL_checkint(L, 1);

	GuiScreenPtr playScreen = GuiManager::getSingleton().getScreen(GuiScreenId::PlayGui);

	playScreen->showForeground();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(GuiManager, hideForeground)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1, the screen id

	GuiScreenPtr playScreen = GuiManager::getSingleton().getScreen(GuiScreenId::PlayGui);

	playScreen->hideForeground();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(GuiManager,showFrame)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1, the screen id

	//int id = luaL_checkint(L, 1);

	GuiScreenPtr playScreen = GuiManager::getSingleton().getScreen(GuiScreenId::PlayGui);

	playScreen->showFrame();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(GuiManager, hideFrame)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1, the screen id

	GuiScreenPtr playScreen = GuiManager::getSingleton().getScreen(GuiScreenId::PlayGui);

	playScreen->hideFrame();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(GuiManager, setFrame)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1, the screen id

	Ogre::String materialName = luaL_checkstring(L, 1);

	GuiScreenPtr playScreen = GuiManager::getSingleton().getScreen(GuiScreenId::PlayGui);

	playScreen->changeFrame(materialName+".png",materialName+"Frame","General");

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(GuiManager, setForeground)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1, the screen id

	Ogre::String materialName = luaL_checkstring(L, 1);

	GuiScreenPtr playScreen = GuiManager::getSingleton().getScreen(GuiScreenId::PlayGui);

	playScreen->changeForeground(materialName+".png",materialName+"Foreground","General");

	/* return the number of results */
	return 0;
}
