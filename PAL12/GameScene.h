#ifndef _GAMESCENE_H
#define _GAMESCENE_H

#include <ogre/Ogre.h>
#include <ois/OIS.h>
#include <CEGUI.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include "SceneMsgID.h"

class BaseScene : public Ogre::FrameListener, public OIS::MouseListener, public OIS::KeyListener
{
	friend class GameApplication;
public:
	BaseScene();
	const Ogre::String& GetName() const;
	void Init(Ogre::Root* root, Ogre::String sceneName);
	virtual void SetActive();
	virtual void SetUnactive();
	//virtual void Disable();
	virtual void LoadResources();
	virtual void UnloadResources();
	bool IsResourceLoaded() { return m_bResourceLoaded; }
	virtual bool keyPressed (const OIS::KeyEvent &arg) { return true; }
	virtual bool keyReleased (const OIS::KeyEvent &arg) { return true;}
	virtual bool mouseMoved (const OIS::MouseEvent &arg) { return true; }
	virtual bool mousePressed (const OIS::MouseEvent &arg, OIS::MouseButtonID id) { return true; }
	virtual bool mouseReleased (const OIS::MouseEvent &arg, OIS::MouseButtonID id) { return true; }
	virtual void CreateCamera(); 
	virtual void CreateSceneManager(); 
	virtual void CreateScene();
	virtual void OnMessage(Ogre::uint32 id, Ogre::int32 data, void* lpinfo) {}
	~BaseScene(void);
	void _SetActive();
	void _SetUnactive();
protected:
	Ogre::String mSceneName;
	Ogre::SceneManager* mSceneMgr; 
	Ogre::Camera* mCamera;
	Ogre::Viewport* mViewport;
	Ogre::Root* mRoot;
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;
	bool m_bResourceLoaded;
	bool m_bSceneCreated;

	Ogre::ColourValue m_BackgroundColor;
};

#endif

