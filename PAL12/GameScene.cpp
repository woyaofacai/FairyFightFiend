#include "GameScene.h"
#include "GameApplication.h"


BaseScene::BaseScene()
{
	mRoot = NULL;
	mSceneMgr = NULL;
	mCamera = NULL;
	mViewport = NULL;

	m_bResourceLoaded = false;
	m_bSceneCreated = false;

	m_BackgroundColor = Ogre::ColourValue(0.0f, 0.0f, 0.0f);
}

BaseScene::~BaseScene()
{

}

const Ogre::String& BaseScene::GetName() const
{
	return mSceneName;
}

void BaseScene::Init(Ogre::Root* root, Ogre::String sceneName)
{
	mRoot = root;
	mSceneName = sceneName;
	mMouse = GameApplication::GetSingletonPtr()->GetMouse();
	mKeyboard = GameApplication::GetSingletonPtr()->GetKeyboard();

	if(Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(mSceneName))
	{
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(mSceneName);
	}

	CreateSceneManager();
	CreateCamera();
}

void BaseScene::SetActive()
{

}

void BaseScene::_SetActive()
{
	Ogre::RenderWindow* win = GameApplication::GetSingletonPtr()->GetWindow();
	win->removeAllViewports();
	mViewport = win->addViewport(mCamera);
	mViewport->setBackgroundColour(m_BackgroundColor);
	mCamera->setAspectRatio((float)mViewport->getActualWidth() / (float)mViewport->getActualHeight());

	mKeyboard->setEventCallback(this);
	mMouse->setEventCallback(this);
	mRoot->addFrameListener(this);

	if(!m_bSceneCreated)
	{
		CreateScene();
		m_bSceneCreated = true;
	}

	SetActive();
}

void BaseScene::_SetUnactive()
{
	SetUnactive();
	mRoot->removeFrameListener(this);
}

void BaseScene::CreateCamera()
{
	mCamera = mSceneMgr->createCamera(mSceneName + "_MainCamera");
	mCamera->setPosition(0, 0, 5);
	mCamera->lookAt(0, 0, 0);
	mCamera->setNearClipDistance(1.0f);
}

void BaseScene::CreateSceneManager()
{
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
}

void BaseScene::LoadResources()
{
	if(!m_bResourceLoaded)
	{
		if(Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(mSceneName))
			Ogre::ResourceGroupManager::getSingleton().loadResourceGroup(mSceneName);

		m_bResourceLoaded = true;
	}
}

void BaseScene::UnloadResources()
{
	if(Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(mSceneName))
		Ogre::ResourceGroupManager::getSingleton().unloadResourceGroup(mSceneName);

	m_bResourceLoaded = false;
}

void BaseScene::CreateScene()
{
	
}

void BaseScene::SetUnactive()
{
	
}