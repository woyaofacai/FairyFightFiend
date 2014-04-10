#pragma once
#ifndef _GAMEAPPLICATION_IMPL_H
#define _GAMEAPPLICATION_IMPL_H

#include "GameApplication.h"
#include <CEGUI.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include "TestScene.h"
#include "GameScenePool.h"
#include "MonsterInfoManager.h"

class GameApplicationImpl : public GameApplication
{
public:
//	static GameApplicationImpl& GetSingleton();
	GameApplicationImpl(void);
	~GameApplicationImpl(void);
	void StartUp();
	Ogre::Root* GetRoot() const;
	Ogre::RenderWindow* GetWindow() const; 
	OIS::Mouse* GetMouse() const;
	OIS::Keyboard* GetKeyboard() const;
	bool ReplaceScene(Ogre::String& targetSceneName, bool unloadPreSceneResource = true, bool loadTargetSceneSource = true);
	void CreateScene(Ogre::String& sceneName, bool bLoadResource);
	void SendMessageToScene(Ogre::String& targetSceneName, Ogre::uint32 id, Ogre::int32 data, void* lpinfo);
private: 
	void _InitializeInputSystem();
	void _InitializeGUI();
	Ogre::Root* mRoot;
	Ogre::RenderWindow* mWindow;
	OIS::InputManager* mInputMgr;
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;
	bool m_bRunning;

	CEGUI::OgreRenderer* mGuiRenderer;

	BaseScene* m_pCurrentScene;
}; 

#endif