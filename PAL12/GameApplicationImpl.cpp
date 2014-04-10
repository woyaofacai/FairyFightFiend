#include "GameApplicationImpl.h"


GameApplicationImpl::GameApplicationImpl(void)
{
	mRoot = NULL;
	mWindow = NULL;
	mInputMgr = NULL;
	mKeyboard = NULL;
	mMouse = NULL;
	m_bRunning = true;
	m_pCurrentScene = NULL;
}

GameApplicationImpl::~GameApplicationImpl(void)
{
	mInputMgr->destroyInputObject(mMouse);
	mInputMgr->destroyInputObject(mKeyboard);
	OIS::InputManager::destroyInputSystem(mInputMgr);
}

void GameApplicationImpl::StartUp()
{
	mRoot = new Ogre::Root("plugins_d.cfg");
	if(!mRoot->showConfigDialog())
		return;

	mWindow = mRoot->initialise(true, "Fairy Fight Fiend");
	//position(800, 100);

	Ogre::ConfigFile cf;
	cf.load("resources_d.cfg");

	Ogre::ConfigFile::SectionIterator sectionIter = cf.getSectionIterator();
	Ogre::String sectionName, typeName, dataString;
	while(sectionIter.hasMoreElements())
	{
		sectionName = sectionIter.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *setting = sectionIter.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator it;
		for (it = setting->begin(); it != setting->end(); it++)
		{
			typeName = it->first;
			dataString = it->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(dataString, typeName, sectionName);
		}
	}

	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("General");

	_InitializeInputSystem();
	_InitializeGUI();

	MonsterInfoManager::GetSingleton().LoadInformationFromFile("../media/models/monsters.dat");

	ReplaceScene(Ogre::String("GUITest"), true, true);

	while(m_bRunning)
	{
		mMouse->capture();
		mKeyboard->capture();
		m_bRunning = mRoot->renderOneFrame();
	}

	GameScenePool::GetSingleton().RemoveAllScenes();
}

/*
GameApplicationImpl& GameApplicationImpl::GetSingleton()
{
	static GameApplicationImpl app;
	return app;
}
*/

Ogre::RenderWindow* GameApplicationImpl::GetWindow() const
{
	return mWindow;
}

void GameApplicationImpl::_InitializeInputSystem()
{
	size_t windowHnd;
	mWindow->getCustomAttribute("WINDOW", &windowHnd);

	std::stringstream windowHndStr;
	windowHndStr << windowHnd;

	OIS::ParamList pl;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	pl.insert(std::make_pair(std::string("w32_mouse"), "DISCL_FOREGROUND"));    
	pl.insert(std::make_pair(std::string("w32_mouse"), "DISCL_NONEXCLUSIVE"));

	mInputMgr = OIS::InputManager::createInputSystem(pl);

	mKeyboard = static_cast<OIS::Keyboard*>(mInputMgr->createInputObject(OIS::OISKeyboard, true));
	mMouse = static_cast<OIS::Mouse*>(mInputMgr->createInputObject(OIS::OISMouse, true));

	//mMouse->getMouseState().width = 800;
	//mMouse->getMouseState().height = 600;

	unsigned int width, height, depth;
	int top, left;
	mWindow->getMetrics(width, height, depth, left, top);
	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

OIS::Keyboard* GameApplicationImpl::GetKeyboard() const
{
	return mKeyboard;
}

OIS::Mouse* GameApplicationImpl::GetMouse() const
{
	return mMouse;
}

Ogre::Root* GameApplicationImpl::GetRoot() const
{
	return mRoot;
}

void GameApplicationImpl::_InitializeGUI()
{
	mGuiRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Imagesets");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Fonts");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Schemes");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("LookNFeel");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Layouts");

	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	CEGUI::SchemeManager::getSingleton().create("GlossySerpent.scheme");
	CEGUI::System::getSingleton().setDefaultMouseCursor("GlossySerpentCursors", "MouseArrow");
	CEGUI::MouseCursor::getSingleton().setImage( CEGUI::System::getSingleton().getDefaultMouseCursor());
}

bool GameApplicationImpl::ReplaceScene(Ogre::String& targetSceneName, bool unloadPreSceneResource /* = true */, bool loadTargetSceneSource /* = true */)
{
	BaseScene* scene = GameScenePool::GetSingleton().GetScene(targetSceneName);
	if(scene == NULL)
		return false;

	if(m_pCurrentScene)
	{
		if(unloadPreSceneResource)
		{
			m_pCurrentScene->UnloadResources();
		}
		m_pCurrentScene->_SetUnactive();
		//mRoot->removeFrameListener(m_pCurrentScene);
	}

	m_pCurrentScene = scene;
	if(loadTargetSceneSource)
		scene->LoadResources();

	scene->_SetActive();
	return true;
}

void GameApplicationImpl::CreateScene(Ogre::String& sceneName, bool bLoadResource)
{
	BaseScene* scene = GameScenePool::GetSingleton().GetScene(sceneName);
	if(scene != NULL)
	{
		if(bLoadResource)
		{
			scene->LoadResources();
		}
	}
}

void GameApplicationImpl::SendMessageToScene( Ogre::String& targetSceneName, Ogre::uint32 id, Ogre::int32 data, void* lpinfo )
{
	BaseScene* scene = GameScenePool::GetSingleton().GetScene(targetSceneName);
	if(scene == NULL)
		return;

	scene->OnMessage(id, data, lpinfo);
}
